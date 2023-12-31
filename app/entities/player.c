#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "player.h"

/**
 * Creates a new player with default values for health, mana, stats, experience, level, and gold.
 *
 * @return A pointer to the newly created player.
 */
Player *create_player(int id)
{
    Player *player = malloc(sizeof(Player));
    player->id = id;

    // Health
    player->current_health = DEFAULT_HEALTH;
    player->max_health = DEFAULT_HEALTH;

    // Mana
    player->current_mana = DEFAULT_MANA;
    player->max_mana = DEFAULT_MANA;

    // Stats
    player->attack_power = DEFAULT_ATTACK_POWER;
    player->experience = 0;
    player->level = 1;
    player->level_up_experience = DEFAULT_LEVEL_UP_EXPERIENCE;
    player->defense = DEFAULT_DEFENSE;
    player->gold = 0;
    player->attack_left = ATTACK_NUMBER;
    player->potion_counter = WAIT_FOR_POTION;
    player->stage = 1;
    player->stuff_count = 0;
    player->stuff = malloc(sizeof(Stuff *));
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
    BLANK_LINE;
    _print_stat_bar("Health", player->current_health, player->max_health, "\e[0;31m", false);
    if (player->stuff_count > 0)
    {
        for (int i = 0; i < player->stuff_count; i++)
        {
            if (player->stuff[i]->equipped)
            {
                char *str = malloc(sizeof(char) * 100);
                switch (player->stuff[i]->type)
                {
                case DEFENSE:
                    sprintf(str, " (\e[34mDefense\e[0m + %d)", player->stuff[i]->bonus);
                    printf("%s", str);
                    break;
                case ATTACK:
                    sprintf(str, " (\e[31mAttack\e[0m + %d)", player->stuff[i]->bonus);
                    printf("%s", str);
                    break;
                }
                free(str);
            }
        }
    }
    printf("\n");
    _print_stat_bar("  Mana", player->current_mana, player->max_mana, "\e[0;34m", true);
    printf("  Gold \e[0;33m%d\e[0m", player->gold);
    printf("    |  ");
    printf("  Level \e[0;32m%d\e[0m", player->level);
    printf("    |  ");
    printf("  Experience \e[0;35m%d/%d\e[0m", player->experience, player->level_up_experience);
    printf("    |  ");
    printf("  Stage \e[0;36m%d\e[0m", player->stage);
    printf("\n");
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

char *save_player(Player *player)
{
    char *sql = malloc(1000 * sizeof(char));
    char *base = "INSERT INTO players (current_health, max_health, current_mana, max_mana, gold, experience, level, level_up_experience, defense, attack_power, attack_left, potion_counter, stage) VALUES";
    sprintf(sql, "%s (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d);", base, player->current_health, player->max_health, player->current_mana, player->max_mana, player->gold, player->experience, player->level, player->level_up_experience, player->defense, player->attack_power, player->attack_left, player->potion_counter, player->stage);
    return sql;
}

char *update_player(Player *player)
{
    char *sql = malloc(1000 * sizeof(char));
    char *base = "UPDATE players SET current_health = %d, max_health = %d, current_mana = %d, max_mana = %d, gold = %d, experience = %d, level = %d, level_up_experience = %d, defense = %d, attack_power = %d, attack_left = %d, potion_counter = %d, stage = %d WHERE id = %d;";
    sprintf(sql, base, player->current_health, player->max_health, player->current_mana, player->max_mana, player->gold, player->experience, player->level, player->level_up_experience, player->defense, player->attack_power, player->attack_left, player->potion_counter, player->stage, player->id);
    return sql;
}

/**
 * @brief Manages the level of the player.
 * 
 * @param player The player to manage the level of.
 */
void manage_player_level(Player *player)
{
    if (player->experience >= player->level_up_experience)
    {
        player->experience -= player->level_up_experience;
        player->level++;
        player->level_up_experience += 10;
        player->max_health += 10;
        player->current_health = player->max_health;
        player->max_mana += 5;
        player->current_mana = player->max_mana;
        player->attack_power += 5;
        player->defense += 5;
        printf("You leveled up !\n");
    }
}

/**
 * @brief Gives experience points to the player based on the maximum health of a defeated monster.
 * 
 * @param player Pointer to the player struct.
 * @param monster_max_health Maximum health of the defeated monster.
 */
void give_exp(Player *player, int monster_max_health)
{
    int exp = monster_max_health / 2;
    player->experience += exp;
    printf("You gained %d experience points ! ", exp);
    manage_player_level(player);
}
