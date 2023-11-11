#ifndef DEF_HEADER_STUFF
#define DEF_HEADER_STUFF

#include "headers.h"
#include "../config.h"
#include "../entities/player.h"
#include "../ui/menu.h"

typedef struct
{
    Stuff **stuff;
    int stuff_count;
} StuffList;

extern StuffList *create_random_stuff_list(int count);
extern void remove_stuff(Player *player, int index);
extern void buy_stuff(Player *player, StuffList *stuff_list);
extern void equip_stuff(Player *player);
extern void give_mana_potion_stuff(Player *player);
extern char *save_stuff(Player *player);

#endif
