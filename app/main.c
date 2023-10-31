#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "entities/monsters.h"
#include "entities/player.h"
#include "utils/attack.h"
#include "utils/headers.h"
#include "ui/menu.h"
#include "utils/db.h"

#define ATTACK_NUMBER 3
#define WAIT_FOR_POTION 6
#define HEALING_AMOUNT 20

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
void display_menu(Player *player, int potion_counter)
{
    CLEAR_MENU;
    printf("What do you want to do ?\n\n");
    printf("1 - Attack (%d/%d)\n", player->attack_left, ATTACK_NUMBER);
    if (potion_counter == WAIT_FOR_POTION)
        printf("2 - Use healing potion (%d)\n", HEALING_AMOUNT);
    else
        printf("2 - Use healing potion (%d turns left)\n", WAIT_FOR_POTION - potion_counter);
    printf("3 - Inventory\n\n");
    printf("8 - End turn\n\n");
    printf("0 - Quit game\n");
}

/**
 * Main function that runs the game.
 */
int main()
{
    init_database(DB_NAME);
    setup_terminal();
    pthread_t tid;
    pthread_create(&tid, NULL, display_title, NULL);
    srand(time(NULL));

game:
    Player *player = NULL;
    int user_choice = handle_start_menu();
    if (user_choice == 0)
    {
        printf("\nQuitting game, thanks for playing !\n");
        return 0;
    }
    else if (user_choice == 1)
        player = create_player();
    else
        player = load_player(DB_NAME, user_choice - 1);

    Monsters *monsters = create_random_monster(4);
    bool print_entities = true;

    char user_input;
    int potion_counter = WAIT_FOR_POTION;
    while (player->current_health > 0)
    {
        print_player_stats(player);
        if (print_entities)
        {
            print_player();
            print_entities = false;
        }
        display_menu(player, potion_counter);

        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            switch (user_input)
            {
            case '1':
                CLEAR_MENU;
                print_monsters_list(monsters);
                manage_player_attack(monsters, player);
                break;
            case '2':
                if (potion_counter == WAIT_FOR_POTION)
                {
                    potion_counter = 0;
                    heal_player(player, HEALING_AMOUNT);
                }
                break;
            case '8':
                if (potion_counter != WAIT_FOR_POTION)
                    potion_counter++;
                for (int i = 0; i < monsters->count; i++)
                {
                    attack_player(player, monsters->monsters[i]);
                    if (player->current_health <= 0)
                    {
                        CLEAR_SCREEN;
                        if (handle_death_menu())
                        {
                            free(player);
                            destroy_monsters(monsters);
                            goto game;
                        }
                        else
                            goto end;
                    }
                }
                player->attack_left = ATTACK_NUMBER;
                GOTO_LOG;
                break;
            case '0':
                if (user_choice == 1)
                    save(DB_NAME, save_player(player));
                else
                    update(DB_NAME, update_player(player));
                goto end;
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