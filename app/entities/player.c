#include "player.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


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
    player->defense = 10;
    player->gold = 0;
    player->attack_left = 3;
    player->stuff_count = 0;
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
void _print_stat_bar(char *label, int current, int max, char *color, bool newline)
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
    printf("\e[0m %d/%d", current, max);
    if (newline)
        printf("\n");
}

/**
 * Prints the player's health, mana, and gold stats.
 *
 * @param player A pointer to the Player struct containing the player's stats.
 */
void print_player_stats(Player *player)
{
    GOTO_STATS;
    _print_stat_bar("Health", player->current_health, player->max_health, "\e[0;31m", false);
    if (player->stuff_count > 0)
    {
        for (int i = 0; i < player->stuff_count; i++)
        {
            if (strcmp(player->stuff[i]->name, "Shield") == 0)
            {
                printf(" + %d (Shield)", player->stuff[i]->defense);
            }
        }
    }
    printf("\n");
    _print_stat_bar("  Mana", player->current_mana, player->max_mana, "\e[0;34m", true);
    printf("  Gold \e[0;33m%d\e[0m\n", player->gold);
}

/**
 * This function prints the ASCII art of the player by reading from the "resources/player.txt" file.
 * If the file cannot be opened, the function prints an error message and exits the program with status code 1.
 */
void print_player()
{
    FILE *file = fopen("ui/resources/player.txt", "r");
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

void heal_player(Player *player, int amount)
{
    player->current_health += amount;
    if (player->current_health > player->max_health)
        player->current_health = player->max_health;
    GOTO_LOG;
    printf("You healed yourself for %d health points !\n", amount);
    fflush(stdout);
}
