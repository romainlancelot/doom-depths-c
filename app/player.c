#include <stdlib.h>
#include <stdio.h>
#include "player.h"

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
    player->attack_power = 10;
    player->experience = 0;
    player->level = 1;
    player->gold = 0;
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
    printf("\033[0m %d/%d\n", current, max);
}

/**
 * Prints the player's health, mana, and gold stats.
 *
 * @param player A pointer to the Player struct containing the player's stats.
 */
void print_player_stats(Player *player)
{
    printf("\033[3;0H");
    _print_stat_bar("Health", player->current_health, player->max_health, "\033[0;31m");
    _print_stat_bar("  Mana", player->current_mana, player->max_mana, "\033[0;34m");
    printf("  Gold \033[0;33m%d\033[0m\n", player->gold);
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

#endif // PLAYER
