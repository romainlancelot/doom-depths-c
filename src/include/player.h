#ifndef DEF_HEADER_PLAYER
#define DEF_HEADER_PLAYER

#include "display_standars.h"

typedef struct {
    int health;
    int maxHealth;
    int attack;
    int maxAttack;
    int defense;
} Player;


extern Player player;
extern Player *createPlayer();
extern Player *createRandomPlayer();
extern Player *removeAttack(Player *player);
extern Player *removeHealth(Player *player, int health);
extern void displayHealth(Player *player);

#endif
