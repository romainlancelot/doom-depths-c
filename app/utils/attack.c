#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "attack.h"

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
    if (player->stuff_count > 0)
    {
        for (int i = 0; i < player->stuff_count; i++)
        {
            if (player->stuff[i]->equipped && player->stuff[i]->type == ATTACK)
            {
                damage += player->stuff[i]->bonus;
                printf("Your %s dealt %d damage ! ", player->stuff[i]->name, player->stuff[i]->bonus);
                player->stuff[i]->bonus -= damage;
                if (player->stuff[i]->bonus <= 0)
                {
                    printf("Your %s broke. ", player->stuff[i]->name);
                    remove_stuff(player, i);
                }
            }
        }
    }
    int total_damage = damage - monster->defense;
    if (total_damage < 0)
        total_damage = 0;
    monster->current_health -= total_damage;
    player->attack_left--;
    printf("You dealt %d damage to %s ! ", total_damage, monster->name);
}

/**
 * Manages the player's attack on monsters.
 *
 * @param monsters The Monsters struct containing the array of monsters.
 * @param player The Player struct containing the player's information.
 */
void manage_player_attack(Monsters *monsters, Player *player, bool *print_entities)
{
    // Reads user input to select a monster to attack.
    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            int choice = atoi(&user_input);
            if (choice == monsters->count + 1)
                return;
            else if (choice < 1 || choice > monsters->count)
                continue;
            GOTO_LOG;
            // Checks if the player has any attack left.
            if (player->attack_left == 0)
            {
                printf("You have no attack left !");
                return;
            }
            // Attacks the selected monster.
            Monster *monster = monsters->monsters[choice - 1];
            attack_monster(monster, player);
            if (monster->current_health <= 0)
            {
                remove_monster(monsters, monster);
                player->gold += GOLD_ON_MONSTER_DEATH;
                give_mana_potion_stuff(player);
                give_exp(player, monster->max_health);
                *print_entities = true;
            }
            break;
        }
    }
}

void attack_player(Player *player, Monster *monster)
{
    int damage = rand() % (monster->max_attack + 1) + monster->min_attack;
    int total_damage = damage;
    player->current_health -= total_damage;
    if (player->current_health < 0)
        player->current_health = 0;
    SAVE_CURSOR;
    GOTO_LOG;
    if (player->stuff_count > 0)
    {
        for (int i = 0; i < player->stuff_count; i++)
        {
            if (player->stuff[i]->equipped && player->stuff[i]->type == DEFENSE)
            {
                total_damage -= player->stuff[i]->bonus;
                if (total_damage < 0)
                    total_damage = 0;
                printf("Your %s absorbed %d damage ! ", player->stuff[i]->name, player->stuff[i]->bonus);
                player->stuff[i]->bonus -= damage;
                if (player->stuff[i]->bonus <= 0)
                {
                    printf("Your %s broke. ", player->stuff[i]->name);
                    remove_stuff(player, i);
                }
            }
        }
    }
    player->current_health -= total_damage;
    printf("%c%s dealt %d damage to you ! Press a key to continue", toupper(monster->name[0]), monster->name+1, total_damage);
    print_player_stats(player);
    RESTORE_CURSOR;
    fflush(stdout);

    char user_input;
    if (read(STDIN_FILENO, &user_input, 1) == 1)
        return;
}
