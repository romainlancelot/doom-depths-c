#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    char *name;
    int health;
    int attack;
    int defense;
} Monster;

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

int main() {
    srand(time(NULL));
    for (int i = 0; i < rand() % 4 + 1; i++) {
        Monster *monster = createRandomMonster();
        printf("Monster %d: %s\n", i + 1, monster->name);
        printf("Health: %d\n", monster->health);
        printf("Attack: %d\n", monster->attack);
        printf("Defense: %d\n", monster->defense);
        printf("\n");
    }

    return EXIT_SUCCESS;
}