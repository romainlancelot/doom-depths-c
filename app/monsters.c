#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "player.h"
#include "monsters.h"
#include "headers.h"

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
 * Manages the player's attack on monsters.
 *
 * @param monsters The Monsters struct containing the array of monsters.
 * @param player The Player struct containing the player's information.
 */
void manage_player_attack(Monsters *monsters, Player *player)
{
    // Reads user input to select a monster to attack.
    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            GOTO_LOG;
            // if (choice == monsters->count + 1)
            //     return;
            // if (choice < 1 || choice > monsters->count)
            //     continue;

            if (user_input == '0' + monsters->count + 1)
                return;
            else if (user_input < '1' || user_input > '0' + monsters->count)
                continue;
            else
            {
                // Checks if the player has any attack left.
                if (player->attack_left == 0)
                {
                    printf("You have no attack left !");
                    return;
                }
                // Attacks the selected monster.
                int choice = atoi(&user_input);
                Monster *monster = monsters->monsters[choice - 1];
                attack_monster(monster, player);
                if (monster->current_health <= 0)
                    _remove_monster(monsters, monster);
                break;
            }
        }
    }
}

/**
 * Removes a monster from the list of monsters and frees its memory.
 *
 * @param monsters The list of monsters.
 * @param monster The monster to be removed.
 */
void _remove_monster(Monsters *monsters, Monster *monster)
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

/**
 * Attacks a monster with a random amount of damage based on the player's attack power.
 * The damage is then reduced by the monster's defense and subtracted from the monster's current health.
 * The player's attack count is also decremented by 1.
 *
 * @param monster A pointer to the monster being attacked.
 * @param player A pointer to the player attacking the monster.
 */
void attack_monster(Monster *monster, Player *player)
{
    int damage = rand() % (player->attack_power + 1) + 50;
    int total_damage = damage - monster->defense;
    if (total_damage < 0)
        total_damage = 0;
    monster->current_health -= total_damage;
    player->attack_left--;
    printf("You dealt %d damage to %s !", total_damage, monster->name);
}

#endif // MONSTERS