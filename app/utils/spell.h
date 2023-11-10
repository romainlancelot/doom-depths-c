#ifndef SPELL_H
#define SPELL_H

#include "headers.h"
#include "../entities/monsters.h"
#include "../entities/player.h"

typedef enum
{
    HEAL,
    DAMAGE,
    DAMAGE_AOE,
    BUFF,
    DEBUFF
} Spelltype;

typedef struct
{
    char *name;
    Spelltype type;
    int power;
    int mana_cost;
    int cooldown;
    int recharge;
} Spell;

typedef struct
{
    Spell **spells;
    int count;
} Spells;

Spell *create_spell(char *name, int power, int mana_cost, int cooldown, Spelltype type);
Spell *create_random_spell();
Spells *create_base_spell_list();
void recharge_spells(Spells *spells);
void manage_spell_choice(Spells *spells, Monsters *monsters, Player *player);
void manage_spell_casting(Spell *spell, Monsters *monsters, Player *player);
void manage_spell_damage(Monsters *monsters, Spell *spell);
void manage_spell_aoe_damage(Monsters *monsters, Spell *spell);
Spells *create_random_spell_list(int count);
void destroy_spells(Spells *spells);
void print_spell_list(Spells *spells);
char *save_spells(Spells *spells, int id);

#endif
