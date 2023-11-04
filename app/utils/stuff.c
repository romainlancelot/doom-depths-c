#include "stuff.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


Stuff *create_stuff(char *name, int attack, int defense, int health, int price)
{
    Stuff *stuff = malloc(sizeof(Stuff));
    char *name_copy = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(name_copy, name);
    stuff->name = name_copy;
    stuff->attack = attack;
    stuff->defense = defense;
    stuff->health = health;
    stuff->price = price;
    return stuff;
}

Stuff **create_stuff_list()
{
    Stuff **stuff_list = malloc(sizeof(Stuff *) * STUFF_NUMBER);
    stuff_list[0] = create_stuff("Sword", 10, 0, 0, 10);
    stuff_list[1] = create_stuff("Shield", 0, 10, 0, 10);
    stuff_list[2] = create_stuff("Potion", 0, 0, 10, 10);
    // stuff_list[3] = create_stuff("Shield", 0, 20, 0, 20);
    // stuff_list[4] = create_stuff("Sword", 20, 0, 0, 20);
    return stuff_list;
}

void print_stuff(Stuff *stuff)
{
    printf("Attack  : %6d\n", stuff->attack);
    printf("Defense : %6d\n", stuff->defense);
    printf("Health  : %6d\n", stuff->health);
    printf("Price   : %6d\n", stuff->price);
    printf("\n");
}

void print_stuff_list(Stuff **stuff_list)
{
    for (int i = 0; i < STUFF_NUMBER; i++)
    {
        printf("%d - %s\n", i, stuff_list[i]->name);
        print_stuff(stuff_list[i]);
    }
}

void print_player_stuff(Player *player)
{

    if (player->stuff_count == 0)
    {
        GOTO_LOG;
        printf("You don't have any stuff !\n");
        return;
    }

    for (int i = 0; i < player->stuff_count; i++)
    {
        printf("%d - %s\n", i, player->stuff[i]->name);
        print_stuff(player->stuff[i]);
    }

    printf("Press any key to back to the menu\n");
    char user_input;
    if (read(STDIN_FILENO, &user_input, 1) == 1)
    {
        GOTO_LOG;
        printf("You backed to the menu !\n");
    }
}

void free_stuff(Stuff *stuff)
{
    free(stuff->name);
    free(stuff);
}

void free_stuff_list(Stuff **stuff_list)
{
    for (int i = 0; i < 5; i++)
    {
        free_stuff(stuff_list[i]);
    }
    free(stuff_list);
}

void buy_stuff(Player *player, Stuff **stuff_list)
{
    printf("Which stuff do you want to buy ?\nTo back to the menu, press 'b'\n");
    char user_input;
    if (read(STDIN_FILENO, &user_input, 1) == 1)
    {
        if (user_input == 'b')
        {
            GOTO_LOG;
            printf("You backed to the menu !\n");
            return;
        }
        int stuff_index = atoi(&user_input);
        if (stuff_index < 0 || stuff_index > STUFF_NUMBER)
        {
            GOTO_LOG;
            printf("This stuff doesn't exist !\n");
            return;
        }
        else
        {
            Stuff *stuff = stuff_list[stuff_index];
            if (player->gold >= stuff->price)
            {
                player->gold -= stuff->price;
                player->stuff_count++;
                player->stuff = realloc(player->stuff, sizeof(Stuff *) * player->stuff_count);
                player->stuff[player->stuff_count - 1] = stuff;
                GOTO_LOG;
                printf("You bought %s !\n", stuff->name);
            }
            else
            {
                GOTO_LOG;
                printf("You don't have enough gold to buy this stuff !\n");
                return;
            }
        }
    }
}
