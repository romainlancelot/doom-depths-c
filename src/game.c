#include "include/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int startGame() {
    // srand(time(NULL));

    Player *player = createRandomPlayer();
    
    for (int i = 0; i < rand() % 4 + 1; i++) {
        Monster *monster = createRandomMonster();
    }

    for (int i = 0; i < monsterCount; i++) {
        printf("Monster: %s\n", monsterList[i].name);
        printf("Health: %d\n", monsterList[i].health);
        printf("Attack: %d\n", monsterList[i].attack);
        printf("Defense: %d\n", monsterList[i].defense);
        printf("\n");
    }

    // char c;
    // printf("Press enter to continue\n");
    // do {
        // c = getchar();
    // } while (c != '\n');

    while (player->health > 0) {
        CLEAR_SCREEN;
        printf("Player health: %d\n", player->health);
        printf("Player attack: %d\n", player->attack);
        printf("\n");

        if (monsterCount == 0) {
            printf("You killed all the monsters\n");
            printf("\n");
            return 1;
        }
        for (int i = 0; i < monsterCount; i++) {
            printf("Monster: %s\n", monsterList[i].name);
            printf("Health: %d\n", monsterList[i].health);
            printf("Attack: %d\n", monsterList[i].attack);
            printf("Defense: %d\n", monsterList[i].defense);
            printf("\n");
        }

        printf("1. Attack\n");
        printf("2. Run\n");
        printf("\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (monsterCount > 0) {
                    int randomIndex = rand() % monsterCount;
                    Monster *monster = &monsterList[randomIndex];
                    printf("Monster: %s\n", monster->name);
                    printf("Health: %d\n", monster->health);
                    printf("Attack: %d\n", monster->attack);
                    printf("Defense: %d\n", monster->defense);
                    printf("\n");

                    int damage = player->attack - monster->defense;
                    if (damage < 0) {
                        damage = 0;
                    }
                    monster->health -= damage;
                    if (monster->health < 0) {
                        monster->health = 0;
                    }
                    printf("You dealt %d damage\n", damage);
                    printf("\n");

                    if (monster->health <= 0) {
                        printf("You killed the monster\n");
                        printf("\n");

                        for (int i = randomIndex; i < monsterCount - 1; i++) {
                            monsterList[i] = monsterList[i + 1];
                        }
                        monsterCount--;
                    } else {
                        damage = monster->attack - player->health;
                        if (damage < 0) {
                            damage = 0;
                        }
                        player->health -= damage;
                        if (player->health < 0) {
                            player->health = 0;
                        }
                        printf("The monster dealt %d damage\n", damage);
                        printf("\n");
                    }
                } else {
                    printf("There are no monsters\n");
                    printf("\n");
                }
                break;
            case 2:
                printf("You ran away\n");
                printf("\n");
                break;
            default:
                printf("Invalid choice\n");
                printf("\n");
                break;
        }
    }

    return 1;
}

void displayMenu() {
    CLEAR_SCREEN;

    printf("1. Start\n");
    printf("0. Quit\n");
    printf("\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 0:
            printf("Goodbye!\n");
            exit(EXIT_SUCCESS);
            break;
        case 1:
            int game = startGame();
            if (game == 1) {
                displayMenu();
            } else {
                printf("Error\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
}
