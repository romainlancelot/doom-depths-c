#ifndef DEF_HEADER_PLAYER
#define DEF_HEADER_PLAYER

#include "../utils/headers.h"


typedef enum {
    ATTACK,
    DEFENSE,
    MANA
} StuffType;

typedef struct {
    char *name;
    int bonus;
    int price;
    short equipped;
    StuffType type;
} Stuff;

typedef struct
{
    int current_health;
    int max_health;
    int current_mana;
    int max_mana;
    int gold;
    int experience;
    int level;
    int defense;
    int attack_power;
    int attack_left;
    Stuff **stuff;
    int stuff_count;
} Player;


extern Player *create_player();
extern void print_player_stats(Player *player);
extern void print_player();
extern void heal_player(Player *player, int amount);
extern void equip_stuff(Player *player);

#endif
