#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "stuff.h"

int stuff_attacks_count = 4;
char *stuff_attacks_names[] = {
    "Sword",
    "Axe",
    "Bow",
    "Dagger"
};

int stuff_defenses_count = 3;
char *stuff_defenses_names[] = {
    "Shield",
    "Armor",
    "Helmet"
};


/**
 * @brief Creates a new Stuff object with the given parameters.
 * 
 * @param type The type of the Stuff.
 * @param name The name of the Stuff.
 * @param bonus The bonus of the Stuff.
 * @param price The price of the Stuff.
 * @return Stuff* A pointer to the newly created Stuff object.
 */
Stuff *create_stuff(StuffType type, char *name, int bonus, int price)
{
    Stuff *stuff = malloc(sizeof(Stuff));
    stuff->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(stuff->name, name);
    stuff->bonus = bonus;
    stuff->price = price;
    stuff->equipped = false;
    stuff->type = type;
    return stuff;
}

/**
 * @brief Creates a list of random stuff.
 * 
 * @param count The number of stuff to generate.
 * @return StuffList* A pointer to the generated list of stuff.
 * 
 * @note The caller is responsible for freeing the memory allocated for the list.
 */
StuffList *create_random_stuff_list(int count)
{
    StuffList *stuff_list = malloc(sizeof(StuffList));
    if (stuff_list == NULL)
    {
        return NULL;
    }

    stuff_list->stuff = malloc(sizeof(Stuff *) * count);
    if (stuff_list->stuff == NULL)
    {
        free(stuff_list);
        return NULL;
    }

    for (int i = 0; i < count; i++)
    {
        int type = rand() % 2;
        char *name;
        int bonus;
        int price;
        switch (type)
        {
        case ATTACK:
            name = stuff_attacks_names[rand() % stuff_attacks_count];
            bonus = rand() % 10 + 10;
            price = rand() % 10 + 10;
            break;
        case DEFENSE:
            name = stuff_defenses_names[rand() % stuff_defenses_count];
            bonus = rand() % 10 + 10;
            price = rand() % 10 + 10;
            break;
        }
        stuff_list->stuff[i] = create_stuff(type, name, bonus, price);

        if (stuff_list->stuff[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(stuff_list->stuff[j]->name);
                free(stuff_list->stuff[j]);
            }
            free(stuff_list->stuff);
            free(stuff_list);
            return NULL;
        }
    }
    stuff_list->stuff_count = count;
    return stuff_list;
}

/**
 * @brief Prints an array of Stuff objects.
 * 
 * @param stuff An array of pointers to Stuff objects.
 * @param count The number of elements in the array.
 * @param show_price A boolean indicating whether to print the price of each Stuff object.
 */
void print_stuff(Stuff **stuff, int count, bool show_price)
{
    char *str = malloc(sizeof(char) * 100);
    char *type = malloc(sizeof(char) * 100);
    char *equipped = malloc(sizeof(char) * 100);

    for (int i = 0; i < count; i++)
    {
        switch (stuff[i]->type)
        {
        case ATTACK:
            strcpy(type, "Attack");
            break;
        case DEFENSE:
            strcpy(type, "Defense");
            break;
        case MANA:
            strcpy(type, "Mana");
            break;
        }

        if (stuff[i]->equipped && stuff[i]->type != MANA)
            strcpy(equipped, "(equiped)");
        else
            strcpy(equipped, "");

        if (show_price)
            sprintf(str, "%d - %s : %s + %d (%d gold)\n", i + 1, stuff[i]->name, type, stuff[i]->bonus, stuff[i]->price);
        else
            sprintf(str, "%d - %s : %s + %d %s\n", i + 1, stuff[i]->name, type, stuff[i]->bonus, equipped);
        printf("%s", str);
    }
}

/**
 * @brief Frees the memory allocated for a Stuff struct.
 * 
 * @param stuff Pointer to the Stuff struct to be destroyed.
 */
void destroy_stuff(Stuff *stuff)
{
    free(stuff->name);
    free(stuff);
}

/**
 * @brief Removes an item from the player's inventory.
 * 
 * @param player Pointer to the player struct.
 * @param index Index of the item to remove.
 */
void remove_stuff(Player *player, int index)
{
    destroy_stuff(player->stuff[index]);
    for (int i = index; i < player->stuff_count - 1; i++)
    {
        player->stuff[i] = player->stuff[i + 1];
    }
    player->stuff_count--;
    player->stuff = realloc(player->stuff, sizeof(Stuff *) * player->stuff_count);
}

/**
 * @brief Allows the player to buy stuff from the stuff list.
 * 
 * @param player Pointer to the player struct.
 * @param stuff_list Pointer to the stuff list struct.
 */
void buy_stuff(Player *player, StuffList *stuff_list)
{
    printf("Which stuff do you want to buy ?\n\n");
    print_stuff(stuff_list->stuff, stuff_list->stuff_count, 1);
    printf("\n%d - Back \n", stuff_list->stuff_count + 1);

    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            int choice = atoi(&user_input);
            if (choice == stuff_list->stuff_count + 1)
                return;
            else if (choice < 1 || choice > stuff_list->stuff_count)
                continue;
            choice--;

            if (player->stuff_count >= MAX_STUFF)
            {
                _log("You can't carry more stuff !\n", GAME_LOG_LINE);
                return;
            }

            Stuff *stuff = stuff_list->stuff[choice];
            if (player->gold >= stuff->price)
            {
                player->gold -= stuff->price;
                player->stuff_count++;
                player->stuff = realloc(player->stuff, sizeof(Stuff *) * player->stuff_count);
                player->stuff[player->stuff_count - 1] = stuff;

                char *str = malloc(sizeof(char) * 100);
                sprintf(str, "You bought %s !\n", stuff->name);
                _log(str, GAME_LOG_LINE);
                return;
            }
            else
            {
                _log("You don't have enough gold to buy this stuff !\n", GAME_LOG_LINE);
                return;
            }
        }
    }
}


/**
 * Equip the player with various items and equipment.
 *
 * @param player A pointer to the player object to equip.
 */
void equip_stuff(Player *player)
{
    if (player->stuff_count == 0)
    {
        _log("You don't have any stuff !", GAME_LOG_LINE);
        return;
    }

    printf("Which stuff do you want to equip/use ?\n\n");
    print_stuff(player->stuff, player->stuff_count, 0);
    printf("\n%d - Delete stuff\n\n", player->stuff_count + 1);
    printf("%d - Back\n", player->stuff_count + 2);

    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            int choice = atoi(&user_input);
            if (choice == player->stuff_count + 2)
                return;
            else if (choice == player->stuff_count + 1)
            {
                delete_stuff(player);
                return;
            }
            else if (choice < 1 || choice > player->stuff_count)
                continue;
            choice--;

            Stuff *stuff = player->stuff[choice];
            if (stuff->type == MANA)
            {
                if (player->current_mana == player->max_mana)
                {
                    _log("You already have full mana !", GAME_LOG_LINE);
                    return;
                }
                player->current_mana += stuff->bonus;
                if (player->current_mana > player->max_mana)
                    player->current_mana = player->max_mana;
                char *str = malloc(sizeof(char) * 100);
                sprintf(str, "You drank a mana potion and restored %d mana points !", stuff->bonus);
                _log(str, GAME_LOG_LINE);
                remove_stuff(player, choice);
                return;
            }

            if (stuff->equipped)
            {
                _log("This stuff is already equipped !\n", GAME_LOG_LINE);
                continue;
            }
            for (int i = 0; i < player->stuff_count; i++)
            {
                if (player->stuff[i]->equipped && player->stuff[i]->type == stuff->type)
                {
                    player->stuff[i]->equipped = false;
                    char *str = malloc(sizeof(char) * 100);
                    sprintf(str, "You unequipped %s. ", player->stuff[i]->name);
                    _log(str, GAME_LOG_LINE);
                    break;
                }
            }
            stuff->equipped = true;
            char *str = malloc(sizeof(char) * 100);
            sprintf(str, "You equipped %s !", stuff->name);
            _log(str, GAME_LOG_LINE);
            return;
        }
    }
}

void delete_stuff(Player *player)
{
    clear(GAME_MENU_LINE);
    printf("Which stuff do you want to delete ?\n\n");
    print_stuff(player->stuff, player->stuff_count, 0);
    printf("\n%d - Back\n", player->stuff_count + 1);

    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            int choice = atoi(&user_input);
            if (choice == player->stuff_count + 1)
                return;
            else if (choice < 1 || choice > player->stuff_count)
                continue;
            choice--;

            Stuff *stuff = player->stuff[choice];
            char *str = malloc(sizeof(char) * 100);
            sprintf(str, "You deleted %s !", stuff->name);
            _log(str, GAME_LOG_LINE);
            remove_stuff(player, choice);
            return;
        }
    }
}

/**
 * @brief Gives mana to a player.
 * 
 * @param player The player to give mana to.
 */
void give_mana_potion_stuff(Player *player)
{
    if (player->stuff_count >= MAX_STUFF)
    {
        printf("You have found a mana potion but you can't carry it ! ");
        return;
    }
    int mana_value = rand() % 10 + 10;
    Stuff *mana_stuff = create_stuff(MANA, "Mana potion", mana_value, 0);
    player->stuff_count++;
    player->stuff = realloc(player->stuff, sizeof(Stuff *) * player->stuff_count);
    player->stuff[player->stuff_count - 1] = mana_stuff;
    printf("You found a mana potion ! ");
}

/**
 * Saves the player's stuff.
 * 
 * @param player The player whose stuff will be saved.
 * @return A pointer to the saved stuff.
 */
char *save_stuff(Player *player)
{
    char *sql = malloc(sizeof(char) * 1000);
    strcpy(sql, "INSERT INTO stuffs (name, bonus, price, equipped, type, player_id) VALUES ");
    for (int i = 0; i < player->stuff_count; i++)
    {
        char *values = malloc(sizeof(char) * 100);
        sprintf(values, "('%s', %d, %d, %d, %d, %d)", player->stuff[i]->name, player->stuff[i]->bonus, player->stuff[i]->price, player->stuff[i]->equipped, player->stuff[i]->type, player->id);
        strcat(sql, values);
        if (i < player->stuff_count - 1)
            strcat(sql, ",");
        free(values);
    }
    strcat(sql, ";");
    return sql;
}
