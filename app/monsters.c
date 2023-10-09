#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "monsters.h"

#if !defined(MONSTERS)
#define MONSTERS

Monster *create_monster(char *name, int health, int min_attack, int max_attack, int defense)
{
    Monster *monster = malloc(sizeof(Monster));

    monster->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(monster->name, name);

    monster->health = health;
    monster->min_attack = min_attack;
    monster->max_attack = max_attack;
    monster->defense = defense;
    return monster;
}

/**
 * Creates an array of random monsters with the given count.
 *
 * @param count The number of monsters to create.
 * @return An array of pointers to the created monsters.
 * @note The created monsters have random names, health, attack, defense, and speed.
 */
Monster **create_random_monster(int count)
{
    char *names[] = {"Goblin", "Orc", "Troll", "Dragon", "Giant", "Golem", "Ghoul"};
    Monster **monsters = malloc(sizeof(Monster *) * count);
    for (int i = 0; i < count; i++)
    {
        char *name = names[rand() % 7];
        monsters[i] = create_monster(name, rand() % 100, rand() % 10, rand() % 20, rand() % 10);
    }
    return monsters;
}

void destroy_monsters(Monster **monsters, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(monsters[i]->name);
        free(monsters[i]);
    }
}

#endif // MONSTERS