#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "monsters.h"

#if !defined(MONSTERS)
#define MONSTERS

/**
 * Creates a new monster with the given attributes.
 *
 * @param name The name of the monster.
 * @param health The health of the monster.
 * @param min_attack The minimum attack damage of the monster.
 * @param max_attack The maximum attack damage of the monster.
 * @param defense The defense of the monster.
 * @return A pointer to the newly created monster.
 */
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

/**
 * Frees the memory allocated for an array of Monster pointers and their names.
 *
 * @param monsters The array of Monster pointers to be freed.
 * @param count The number of Monster pointers in the array.
 */
void destroy_monsters(Monster **monsters, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(monsters[i]->name);
        free(monsters[i]);
    }
}

void print_monsters_list(Monster **monsters, int count)
{
    printf("\033[31;H\033[J");
    fflush(stdout);
    int i = 0;
    for (; i < count; i++)
    {
        printf("%d - %s\n", i + 1, monsters[i]->name);
    }
    printf("\n%d - Back\033[J", i + 1);
    fflush(stdout);

    int user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            if (user_input == '0' + i + 1)
                break;
        }
    }
}

#endif // MONSTERS