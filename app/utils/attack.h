#ifndef DEF_HEADER_ATTACK
#define DEF_HEADER_ATTACK

#include "headers.h"
#include "../config.h"
#include "../entities/monsters.h"
#include "../entities/player.h"
#include "../utils/stuff.h"


extern void attack_player(Player *player, Monster *monster);
extern void manage_player_attack(Monsters *monsters, Player *player, bool *print_entities);
extern void attack_monster(Monster *monster, Player *player);

#endif // ATTACK_H
