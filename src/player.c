#include "include/player.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Player *createPlayer(int health, int attack) {
    Player *player = malloc(sizeof(Player));
    player->health = health;
    player->maxHealth = 100;
    player->attack = attack;
    player->maxAttack = attack;
    
    return player;
}

Player *createRandomPlayer() {
    int health = (rand() % 100) + 1;
    int attack = (rand() % 10) + 1;

    return createPlayer(health, attack);
}

Player *removeAttack(Player *player) {
    player->attack -= 1;
    if (player->attack < 0) {
        player->attack = 0;
    }

    return player;
}

Player *removeHealth(Player *player, int damage) {
    player->health -= damage;
    if (player->health < 0) {
        player->health = 0;
    }

    return player;
}
