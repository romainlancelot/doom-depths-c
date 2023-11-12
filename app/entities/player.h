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
    int id;
    int current_health;
    int max_health;
    int current_mana;
    int max_mana;
    int gold;
    int experience;
    int level;
    int level_up_experience;
    int defense;
    int attack_power;
    int attack_left;
    int potion_counter;
    int stage;
    Stuff **stuff;
    int stuff_count;
} Player;


extern Player *create_player(int id);
extern void print_player_stats(Player *player);
extern void heal_player(Player *player, int amount);
extern char *save_player(Player *player);
extern char *update_player(Player *player);
extern void give_exp(Player *player, int monster_max_health);

#endif // PLAYER
