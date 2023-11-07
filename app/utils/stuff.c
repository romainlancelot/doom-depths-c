#include "stuff.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


int stuff_attacks_count = 2;
char *stuff_attacks_names[] = {
    "Sword",
    "Axe"
};

int stuff_defenses_count = 1;
char *stuff_defenses_names[] = {
    "Shield",
    "Armor"
};


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

StuffList *create_random_stuff_list(int count)
{
    StuffList *stuff_list = malloc(sizeof(StuffList));
    if (stuff_list == NULL) {
        return NULL;
    }

    stuff_list->stuff = malloc(sizeof(Stuff *) * count);
    if (stuff_list->stuff == NULL) {
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

        if (stuff_list->stuff[i] == NULL) {
            for (int j = 0; j < i; j++) {
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

void print_stuff(Stuff *stuff)
{
    printf("%s : ", stuff->name);
    switch (stuff->type)
    {
        case ATTACK:
            printf("Attack + %d", stuff->bonus);
            break;
        case DEFENSE:
            printf("Defense + %d", stuff->bonus);
            break;
    }
}

void print_stuff_list(StuffList *stuff_list)
{
    for (int i = 0; i < STUFF_NUMBER; i++)
    {
        printf("%d - ", i);
        print_stuff(stuff_list->stuff[i]);
        printf("\n");
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
        printf("%d - ", i);
        print_stuff(player->stuff[i]);
        if (player->stuff[i]->equipped)
        {
            printf(" (equipped)");
        }
        printf("\n");
    }

    equip_stuff(player);
}

void destroy_stuff(Stuff *stuff)
{
    free(stuff->name);
    free(stuff);
}

void destroy_stuff_list(StuffList *stuff_list)
{
    for (int i = 0; i < stuff_list->stuff_count; i++)
    {
        destroy_stuff(stuff_list->stuff[i]);
    }
    free(stuff_list->stuff);
    free(stuff_list);
}

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

void buy_stuff(Player *player, StuffList *stuff_list)
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
            Stuff *stuff = stuff_list->stuff[stuff_index];
            if (player->gold >= stuff->price)
            {
                player->gold -= stuff->price;
                player->stuff_count++;
                player->stuff = realloc(player->stuff, sizeof(Stuff *) * player->stuff_count);
                player->stuff[player->stuff_count - 1] = stuff;
                GOTO_LOG;
                printf("You bought %s !\n", stuff->name);
                return;
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
