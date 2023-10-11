#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "monsters.h"
#include "player.h"
#include "headers.h"

#if !defined(PLAYER)
#define PLAYER

/**
 * Creates a new player with default values for health, mana, stats, experience, level, and gold.
 *
 * @return A pointer to the newly created player.
 */
Player *create_player()
{
    Player *player = malloc(sizeof(Player));

    // Health
    player->current_health = 100;
    player->max_health = 100;

    // Mana
    player->current_mana = 100;
    player->max_mana = 100;

    // Stats
    player->attack_power = 20;
    player->experience = 0;
    player->level = 1;
    player->gold = 0;
    player->attack_left = 3;
    return player;
}

/**
 * Prints a status bar with a label, current and maximum values, and a color.
 *
 * @param label The label to display before the status bar.
 * @param current The current value to display in the status bar.
 * @param max The maximum value to display in the status bar.
 * @param color The color to use for the status bar.
 */
void _print_stat_bar(char *label, int current, int max, char *color)
{
    printf("%s ", label);
    printf("%s", color); // Set color
    int pourcentage = (current * 100 / max);
    for (int i = 0; i < 100; i++)
    {
        if (i < pourcentage)
            printf("#");
        else
            printf("-");
    }
    printf("\e[0m %d/%d\n", current, max);
}

/**
 * Prints the player's health, mana, and gold stats.
 *
 * @param player A pointer to the Player struct containing the player's stats.
 */
void print_player_stats(Player *player)
{
    GOTO_STATS;
    _print_stat_bar("Health", player->current_health, player->max_health, "\e[0;31m");
    _print_stat_bar("  Mana", player->current_mana, player->max_mana, "\e[0;34m");
    printf("  Gold \e[0;33m%d\e[0m\n", player->gold);
}

/**
 * This function prints the ASCII art of the player by reading from the "resources/player.txt" file.
 * If the file cannot be opened, the function prints an error message and exits the program with status code 1.
 */
void print_player()
{
    FILE *file = fopen("resources/player.txt", "r");
    if (file == NULL)
    {
        printf("Error opening player ASCII file!\n");
        exit(1);
    }
    printf("\033[20;H");
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    printf("\n\n");
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
                    remove_monster(monsters, monster);
                break;
            }
        }
    }
}

#endif // PLAYER
