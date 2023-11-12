#ifndef SPELL_H
#define SPELL_H

#include <stdbool.h>


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

#include "headers.h"
#include "../entities/monsters.h"
#include "../entities/player.h"

extern Spell *create_spell(char *name, int power, int mana_cost, int cooldown, Spelltype type);
extern Spell *create_random_spell();
extern void recharge_spells(Spells *spells);
extern void manage_spell_choice(Spells *spells, Monsters *monsters, Player *player, bool *print_entities);
extern void manage_spell_casting(Spell *spell, Monsters *monsters, Player *player, bool *print_entities);
extern void manage_spell_damage(Monsters *monsters, Spell *spell, bool *print_entities);
extern void manage_spell_aoe_damage(Monsters *monsters, Spell *spell, bool *print_entities);
extern Spells *create_random_spell_list(int count);
extern void destroy_spells(Spells *spells);
extern void print_spell_list(Spells *spells);
extern char *save_spells(Spells *spells, int id);

#endif
