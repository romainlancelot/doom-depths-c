#include "spell.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

char *DamageSpellName[] = {"Fireball", "Ice Spike", "Rock Crumble", "Fatal Punch", "Heart Attack"};
char *DamageAoeSpellName[] = {"Meteor", "Tsunami", "Earthquake", "Flaming Tornado", "Massive Destruction"};
char *HealSpellName[] = {"Healbeam", "Spiritual Light", "Holy Blessing", "Ultimate Recovery", "Phoenix Flame"};

/**
 * Creates a new monster with the given attributes.
 *
 * @param name The name of the monster.
 * @param power The health of the monster.
 * @param mana_cost The minimum attack damage of the monster.
 * @param cooldown The maximum attack damage of the monster.
 * @param type The defense of the monster.
 * @return A pointer to the newly created monster.
 */
Spell *create_spell(char *name, int power, int mana_cost, int cooldown, Spelltype type)
{
    Spell *spell = malloc(sizeof(Spell));

    spell->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(spell->name, name);

    spell->type = type;
    spell->power = power;
    spell->mana_cost = mana_cost;
    spell->cooldown = cooldown;
    spell->recharge = cooldown;
    return spell;
}

/**
 *
 * @return A pointer to a completely random spell
 */
Spell *create_random_spell()
{
    Spell *spell = malloc(sizeof(Spell));
    int spelltype = rand() % 3;
    int spellLevel = rand() % 50 + 10;
    double nameArrayChoice = ((double)spellLevel / 50) * 5 - 1;

    char *name = "";
    switch (spelltype)
    {
    case 0:
        spell->type = DAMAGE;
        name = DamageSpellName[(int)nameArrayChoice];
        break;
    case 1:
        spell->type = DAMAGE_AOE;
        name = DamageAoeSpellName[(int)nameArrayChoice];
        break;
    case 2:
        spell->type = HEAL;
        name = HealSpellName[(int)nameArrayChoice];
        break;
    }
    spell->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(spell->name, name);
    int shaker = rand() % 20;
    spell->power = spellLevel + shaker;
    spell->mana_cost = spellLevel / 1.5 + shaker;
    int cooldown = spellLevel / 5 + shaker;
    spell->cooldown = cooldown;
    spell->recharge = cooldown;
    return spell;
}

/**
 * Creates an array of random spells with the given count.
 *
 * @param count The number of spells to create.
 * @return A pointer to the newly created Spells struct.
 * @note The spell list contains 3 basic spells, 1 heal, 1 targeted and 1 area of effect.
 */
Spells *create_random_spell_list(int count)
{
    Spell **spells = malloc(sizeof(Spell *) * count);
    for (int i = 0; i < count; i++)
    {
        Spell *spell = create_random_spell();
        for (int j = 0; j < i; j++)
        {
            if (strcmp(spell->name, spells[j]->name) == 0)
            {
                i--;
                break;
            }
        }
        spells[i] = spell;
    }

    Spells *spell_list = malloc(sizeof(Spells));
    spell_list->spells = spells;
    spell_list->count = count;
    return spell_list;
}

/**
 * Frees the memory allocated for an array of Monster pointers and their names.
 *
 * @param monsters The Monsters struct to free.
 * @param count The number of Monster pointers in the array.
 */
void destroy_spells(Spells *spells)
{
    for (int i = 0; i < spells->count; i++)
    {
        free(spells->spells[i]->name);
        free(spells->spells[i]);
    }
    free(spells->spells);
    free(spells);
}

void print_spell_list(Spells *spells)
{
    printf("Choose a spell to cast:\n\n");
    for (int i = 0; i < spells->count; i++)
    {
        char *name = spells->spells[i]->name;
        int mana_cost = spells->spells[i]->mana_cost;
        int recharge = spells->spells[i]->recharge;
        int cooldown = spells->spells[i]->cooldown;
        printf("%d - %s | Cost: %d MP (%d/%d) - ", i + 1, name, mana_cost, recharge, cooldown);
        // Print a little descriptive of the spell
        switch (spells->spells[i]->type)
        {
        case DAMAGE_AOE:
            printf("Deals %d damage to all monster", spells->spells[i]->power);
            break;
        case DAMAGE:
            printf("Deals %d damage to one monsters", spells->spells[i]->power);
            break;
        case HEAL:
            printf("Heals you for %d HP", spells->spells[i]->power);
            break;
        case BUFF:
            break;
        case DEBUFF:
            break;
        }
        printf("\n");
    }
    printf("\n%d - Back\n", spells->count + 1);
}

void manage_spell_choice(Spells *spells, Monsters *monsters, Player *player, bool *print_entities)
{
    // Reads user input to select a monster to attack.
    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            GOTO_LOG;
            if (user_input == '0' + spells->count + 1)
                return;
            else if (user_input < '1' || user_input > '0' + spells->count)
                continue;
            else
            {
                if (player->current_mana <= 0)
                {
                    printf("You have no mana left !");
                    return;
                }
                int choice = atoi(&user_input);
                Spell *spell = spells->spells[choice - 1];
                if (spell->recharge != spell->cooldown)
                {
                    printf("Spell isn't ready yet !");
                    return;
                }
                if (spell->mana_cost > player->current_mana)
                {
                    printf("Not enough mana !");
                    return;
                }
                // Attacks the selected monster.
                manage_spell_casting(spell, monsters, player, print_entities);
                break;
            }
        }
    }
}

/**
 *
 * @param spell
 * @param monsters
 * @param player
 * @note Deplete player mana and do the calculation of the spell
 */
void manage_spell_casting(Spell *spell, Monsters *monsters, Player *player, bool *print_entities)
{
    GOTO_LOG;
    printf("You used %s for %d points of mana.\n", spell->name, spell->mana_cost);
    player->current_mana -= spell->mana_cost;
    switch (spell->type)
    {
    case DAMAGE_AOE:
        manage_spell_aoe_damage(monsters, spell, print_entities);
        break;
    case DAMAGE:
        clear(GAME_MENU_LINE);
        print_monsters_list(monsters);
        manage_spell_damage(monsters, spell, print_entities);
        break;
    case HEAL:
        heal_player(player, spell->power);
        break;
    case BUFF:
        break;
    case DEBUFF:
        break;
    }
    spell->recharge = 0;
}

/**
 * Manages the player's attack on monsters.
 *
 * @param monsters The Monsters struct containing the array of monsters.
 * @param spell The Player struct containing the player's information.
 */
void manage_spell_damage(Monsters *monsters, Spell *spell, bool *print_entities)
{
    // Reads user input to select a monster to attack.
    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            GOTO_LOG;
            if (user_input == '0' + monsters->count + 1)
                return;
            else if (user_input < '1' || user_input > '0' + monsters->count)
                continue;
            else
            {
                int choice = atoi(&user_input);
                Monster *monster = monsters->monsters[choice - 1];
                int damage = spell->power;
                int total_damage = damage - monster->defense;
                if (total_damage < 0)
                    total_damage = 0;
                monster->current_health -= total_damage;
                printf("You dealt %d damage to %s ! ", total_damage, monster->name);
                if (monster->current_health <= 0)
                {
                    remove_monster(monsters, monster);
                    *print_entities = true;
                }
                break;
            }
        }
    }
}

/**
 *
 * @param monsters
 * @param spell
 * @note Manage the AOE damage of spells.
 */
void manage_spell_aoe_damage(Monsters *monsters, Spell *spell, bool *print_entities)
{
    int damage = spell->power;
    for (int i = 0; i < monsters->count; ++i)
    {
        int total_damage = damage - monsters->monsters[i]->defense;
        if (total_damage < 0)
            total_damage = 0;
        monsters->monsters[i]->current_health -= total_damage;
        printf("You dealt %d damage to %s ! ", total_damage, monsters->monsters[i]->name);
    }
    for (int i = 0; i < monsters->count; ++i)
    {
        if (monsters->monsters[i]->current_health <= 0)
        {
            remove_monster(monsters, monsters->monsters[i]);
            *print_entities = true;
            i--;
        }
    }
}

void recharge_spells(Spells *spells)
{
    for (int i = 0; i < spells->count; ++i)
    {
        Spell *spell = spells->spells[i];
        if (spell->recharge < spell->cooldown)
            spell->recharge++;
    }
}

char *save_spells(Spells *spells, int id)
{
    char *sql = malloc(sizeof(char) * 1000);
    strcpy(sql, "INSERT INTO spells (name, type, power, mana_cost, cooldown, recharge, player_id) VALUES ");
    for (int i = 0; i < spells->count; i++)
    {
        char *values = malloc(sizeof(char) * 100);
        Spell *spell = spells->spells[i];
        sprintf(values, "('%s', %d, %d, %d, %d, %d, %d)", spell->name, spell->type, spell->power, spell->mana_cost, spell->cooldown, spell->recharge, id);
        strcat(sql, values);
        if (i < spells->count - 1)
            strcat(sql, ",");
        free(values);
    }
    strcat(sql, ";");
    return sql;
}
