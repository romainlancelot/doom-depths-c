#ifndef DEF_HEADER_STUFF
#define DEF_HEADER_STUFF

#include "headers.h"
#include "../config.h"
#include "../entities/player.h"


extern Stuff *create_stuff(char *name, int attack, int defense, int health, int price);
extern Stuff **create_stuff_list();
extern void print_stuff(Stuff *stuff);
extern void print_stuff_list(Stuff **stuff_list);
extern void print_player_stuff(Player *player);
extern void free_stuff(Stuff *stuff);
extern void free_stuff_list(Stuff **stuff_list);
extern void buy_stuff(Player *player, Stuff **stuff_list);

#endif
