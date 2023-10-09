#include "include/monster.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Monster *createMonster(char *name, int health, int attack, int defense) {
    Monster *monster = malloc(sizeof(Monster));
    monster->name = name;
    monster->health = health;
    monster->attack = attack;
    monster->defense = defense;
    
    return monster;
}

Monster *createRandomMonster() {
    char *names[] = {"Slime", "Goblin", "Orc", "Dragon"};
    int health = (rand() % 100) + 1;
    int attack = (rand() % 10) + 1;
    int defense = (rand() % 10) + 1;
    int randomIndex = (rand() % 4);
    char *name = names[randomIndex];

    return createMonster(name, health, attack, defense);
}
