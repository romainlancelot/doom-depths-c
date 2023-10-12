#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "monsters.h"
#include "../utils/headers.h"

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

    monster->current_health = health;
    monster->max_health = health;
    monster->min_attack = min_attack;
    monster->max_attack = max_attack;
    monster->defense = defense;
    return monster;
}

/**
 * Creates an array of random monsters with the given count.
 *
 * @param count The number of monsters to create.
 * @return A pointer to the newly created Monsters struct.
 * @note The created monsters have random names, health, attack, defense, and speed.
 */
Monsters *create_random_monster(int count)
{
    char *names[] = {"Goblin", "Orc", "Troll", "Dragon", "Giant", "Golem", "Ghoul"};
    Monster **monsters = malloc(sizeof(Monster *) * count);
    for (int i = 0; i < count; i++)
    {
        char *name = names[rand() % 7];
        monsters[i] = create_monster(name, rand() % 100, rand() % 10, rand() % 20, rand() % 10);
    }

    Monsters *monsters_list = malloc(sizeof(Monsters));
    monsters_list->monsters = monsters;
    monsters_list->count = count;
    return monsters_list;
}

/**
 * Frees the memory allocated for an array of Monster pointers and their names.
 *
 * @param monsters The Monsters struct to free.
 * @param count The number of Monster pointers in the array.
 */
void destroy_monsters(Monsters *monsters)
{
    for (int i = 0; i < monsters->count; i++)
    {
        free(monsters->monsters[i]->name);
        free(monsters->monsters[i]);
    }
    free(monsters->monsters);
    free(monsters);
}

/**
 * Prints a list of monsters with their names and health status and get user choice.
 *
 * @param monsters The object containing the array of monsters.
 * @param count The number of monsters in the array.
 */
void print_monsters_list(Monsters *monsters)
{
    printf("Choose a monster to attack:\n\n");
    for (int i = 0; i < monsters->count; i++)
    {
        char *name = monsters->monsters[i]->name;
        int health = monsters->monsters[i]->current_health;
        int max_health = monsters->monsters[i]->max_health;
        printf("%d - %s (%d/%d)\n", i + 1, name, health, max_health);
    }
    printf("\n%d - Back\n", monsters->count + 1);
}

/**
 * Removes a monster from the list of monsters and frees its memory.
 *
 * @param monsters The list of monsters.
 * @param monster The monster to be removed.
 */
void remove_monster(Monsters *monsters, Monster *monster)
{
    printf(" You killed %s !", monster->name);
    monsters->count--;
    Monster **alive_monsters = malloc(sizeof(Monster *) * monsters->count);

    for (int i = 0, j = 0; i < monsters->count + 1; i++)
    {
        if (monsters->monsters[i] != monster)
            alive_monsters[j++] = monsters->monsters[i];
    }
    free(monster->name);
    free(monster);
    free(monsters->monsters);
    monsters->monsters = alive_monsters;
}

#endif // MONSTERS