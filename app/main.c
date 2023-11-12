#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "config.h"
#include "utils/headers.h"
#include "entities/monsters.h"
#include "entities/player.h"
#include "utils/attack.h"
#include "utils/spell.h"
#include "utils/stuff.h"
#include "ui/menu.h"
#include "utils/db.h"

/**
 * Sets up the terminal by clearing it and modifying its attributes.
 */
void setup_terminal()
{
    system("clear"); // Clear the terminal
    struct termios original;
    tcgetattr(STDIN_FILENO, &original);

    struct termios modified = original;
    modified.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &modified);
}

/**
 * Displays the title of the game along with the current time in hours, minutes, and seconds.
 * The function runs indefinitely until the program is terminated.
 */
void display_title()
{
    for (;;)
    {
        SAVE_CURSOR;
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("\e[1HDoomdepths - %02d:%02d:%02d", timeinfo->tm_hour + 2, timeinfo->tm_min, timeinfo->tm_sec);
        RESTORE_CURSOR;
        fflush(stdout);
        sleep(1);
    }
}

/**
 * Displays the game menu with options for the player to choose from.
 */
void display_menu(Player *player)
{
    clear(GAME_MENU_LINE);
    printf("What do you want to do ?\n\n");
    printf("1 - Attack (%d/%d)\n", player->attack_left, ATTACK_NUMBER);
    if (player->potion_counter == WAIT_FOR_POTION)
        printf("2 - Use healing potion (%d)\n", HEALING_AMOUNT);
    else
        printf("2 - Use healing potion (%d turns left)\n", WAIT_FOR_POTION - player->potion_counter);
    printf("3 - Inventory\n");
    printf("4 - Buy stuff\n");
    printf("5 - Cast spell\n\n");
    printf("8 - End turn\n\n");
    printf("0 - Quit game\n");
}

/**
 * Main function that runs the game.
 */
int main()
{
    sqlite3 *db = init_database(DB_NAME);
    setup_terminal();
    pthread_t tid;
    pthread_create(&tid, NULL, display_title, NULL);
    srand(time(NULL));

    Player *player = NULL;
    Monsters *monsters = NULL;
    StuffList *stuff_list = create_random_stuff_list(SHOP_STUFF_NUMBER);
    Spells *spells = NULL;

choice:
    clear(MAIN_MENU_LINE);
    int user_choice = handle_start_menu(db);
    int player_count = get_player_count(db);

    switch (user_choice)
    {
    case 0:
        printf("\nQuitting game, thanks for playing !\n");
        return 0;
    case 1:
        if (player_count == MAX_SAVE)
        {
            _log("You already have 3 saves, load one to continue.", MENU_LOG_LINE);
            goto choice;
        }
        player = create_player(player_count + 1);
        monsters = create_random_monster(player, rand() % 3 + 1);
        spells = create_random_spell_list(DEFAULT_SPELL_NUMBER);
        break;
    case 9:
        clear(MAIN_MENU_LINE);
        user_choice = handle_delete_save_menu(db);
        if (user_choice == 0)
            goto choice;
        delete_save(db, user_choice - 1);
        goto choice;
    default:
        if (user_choice - 1 > player_count)
        {
            _log("This save does not exist.", MENU_LOG_LINE);
            goto choice;
        }
        player = load_player(db, user_choice - 1);
        monsters = load_monsters(db, player->id);
        spells = load_spells(db, player->id);
        break;
    }
    CLEAR_SCREEN;

game:
    bool print_entities = true;


    char user_input;
    while (player->current_health > 0)
    {

        if(monsters->count < 1){
            player->attack_left = ATTACK_NUMBER;
            player->stage++;
            print_entities = true;
            CLEAR_SCREEN;
            handle_win_menu(player);
            CLEAR_SCREEN;
            if(player->stage%5)
                monsters = create_random_monster(player, rand() % 3 + 1);
            else
                monsters = create_random_champion(1);
        }

        print_player_stats(player);
        if (print_entities)
        {
            clear(ENTITY_LINE);
            print_entity("ui/resources/player.txt", PLAYER_LINE, 0);
            for (int i = 0; i < monsters->count; i++)
            {
                char *filename = malloc(sizeof(char) * 100);
                sprintf(filename, "ui/resources/%s.txt", monsters->monsters[i]->name);
                int random_row = rand() % 7 + 10;
                print_entity(filename, random_row, i + 1);
            }
            print_entities = false;
        }
        display_menu(player);

        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            switch (user_input)
            {
            case '1': // Attack
                clear(GAME_MENU_LINE);
                print_monsters_list(monsters);
                manage_player_attack(monsters, player, &print_entities);
                break;
            case '2': // Use potion
                if (player->potion_counter == WAIT_FOR_POTION)
                {
                    player->potion_counter = 0;
                    heal_player(player, HEALING_AMOUNT);
                }
                break;
            case '3':
                clear(GAME_MENU_LINE);
                equip_stuff(player);
                break;
            case '4':
                clear(GAME_MENU_LINE);
                buy_stuff(player, stuff_list);
                break;
            case '5':
                clear(GAME_MENU_LINE);
                print_spell_list(spells);
                manage_spell_choice(spells, monsters, player, &print_entities);
                break;
            case '8': // End turn
                if (player->potion_counter != WAIT_FOR_POTION)
                    player->potion_counter++;
                for (int i = 0; i < monsters->count; i++)
                {
                    attack_player(player, monsters->monsters[i]);
                    if (player->current_health <= 0) // Player died
                    {
                        CLEAR_SCREEN;
                        if (handle_death_menu())
                        {
                            int id = player->id;
                            free(player);
                            destroy_monsters(monsters);
                            player = create_player(id);
                            goto game;
                        }
                        else
                            goto end;
                    }
                }
                player->attack_left = ATTACK_NUMBER;
                recharge_spells(spells);
                GOTO_LOG;
                break;
            case '0':
                CLEAR_SCREEN;
                if (user_choice == 1)
                {
                    save(db, save_player(player));
                    save(db, save_spells(spells, player->id));
                }
                else
                    save(db, update_player(player));

                // Save monsters in db
                clear_monsters(db, player->id);
                save(db, save_monsters(monsters, player->id));

                // Save stuff in db
                clear_stuff(db, player->id);
                save(db, save_stuff(player));
                goto choice;
            }
        }


    }

end:
    printf("\nQuitting game, thanks for playing !\n");
    free(player);
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    destroy_monsters(monsters);
    return 0;
}
