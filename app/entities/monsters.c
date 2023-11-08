#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "monsters.h"
#include "../utils/headers.h"

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
    char *names[] = {"darkangel", "griffon", "grimreaper"};
    Monster **monsters = malloc(sizeof(Monster *) * count);
    for (int i = 0; i < count; i++)
    {
        char *name = names[rand() % 3];
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
        printf("%d - %c%s (%d/%d)\n", i + 1, toupper(name[0]), name + 1, health, max_health);
    }
    printf("\n%d - Back\n", monsters->count + 1);
}

/**
 * Prints the ASCII art representation of a monster to the console.
 *
 * @param monster Pointer to the Monster struct to be printed.
 * @param number The number of the monster in the list of monsters to be printed.
 */
void print_monster(Monster *monster, int number)
{
    int random_row = rand() % 7 + 10;
    char *file_path = malloc(sizeof(char) * 100);
    sprintf(file_path, "ui/resources/%s.txt", monster->name);
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("Error opening player ASCII file!\n");
        exit(1);
    }
    printf("\033[%d;H", random_row);
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("\e[%dC", (number * 25) * 2);
        printf("%s", line);
    }
    printf("\n\n");
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

char *save_monsters(Monsters *monsters, int id)
{
    char *sql = malloc(sizeof(char) * 1000);
    strcpy(sql, "INSERT INTO monsters (name, current_health, max_health, min_attack, max_attack, defense, player_id) VALUES ");
    for (int i = 0; i < monsters->count; i++)
    {
        char *name = monsters->monsters[i]->name;
        int current_health = monsters->monsters[i]->current_health;
        int max_health = monsters->monsters[i]->max_health;
        int min_attack = monsters->monsters[i]->min_attack;
        int max_attack = monsters->monsters[i]->max_attack;
        int defense = monsters->monsters[i]->defense;
        char *values = malloc(sizeof(char) * 100);
        sprintf(values, "('%s', %d, %d, %d, %d, %d, %d)", name, current_health, max_health, min_attack, max_attack, defense, id);
        strcat(sql, values);
        if (i < monsters->count - 1)
            strcat(sql, ", ");
        free(values);
    }
    strcat(sql, ";");
    return sql;
}