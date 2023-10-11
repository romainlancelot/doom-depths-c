#include "include/player.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Player *createPlayer(int health, int attack, int defense) {
    Player *player = malloc(sizeof(Player));
    player->health = health;
    player->maxHealth = 100;
    player->attack = attack;
    player->maxAttack = attack;
    player->defense = defense;
    
    return player;
}

Player *createRandomPlayer() {
    // int health = (rand() % 100) + 1;
    // int attack = (rand() % 10) + 1;
    int health = 100;
    int attack = 3;
    int defense = (rand() % 10) + 1;

    return createPlayer(health, attack, defense);
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

void displayHealth(Player *player) {
    int hearts = player->health / (player->maxHealth / 100);
    printf("Health ");
    for (int i = 0; i < hearts; i++) {
        printf(RED "#" NORMAL);
    }
    for (int i = 0; i < 100 - hearts; i++) {
        printf(RED "-" NORMAL);
    }
    printf(" %d / 100\n", player->health);
}
