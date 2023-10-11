#include "include/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int startGame() {
    srand(time(NULL));

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

    do {
		printf("Press enter to start the game\n");
		getchar();
	} while (getchar() != '\n');

    while (player->health > 0) {
        if (player->attack == 0) {
            CLEAR_SCREEN;
            displayHealth(player);
            printf("\n");
            for (int i = 0; i < monsterCount; i++) {
                Monster *monster = &monsterList[i];
                int damage = monster->damage - player->defense;
                if (damage < 0) {
                    damage = 0;
                }
                printf("%s attacked you for %d damage\n", monster->name, damage);
                player->health -= damage;
                if (player->health < 0) {
                    player->health = 0;
                }
                if (player->health == 0) {
                    printf("You died\n");
                    printf("\n");
                    return 1;
                }
            }
            do {
                printf("\nPress enter to continue\n");
                getchar();
            } while (getchar() != '\n');
            player->attack = player->maxAttack;
        }
        CLEAR_SCREEN;
        displayHealth(player);
        printf("Player attack: %d/%d\n", player->attack, player->maxAttack);
        printf("\n");

        if (monsterCount == 0) {
            printf("You killed all the monsters\n");
            printf("\n");
            return 1;
        }
        for (int i = 0; i < monsterCount; i++) {
            printf("%d - %s (%d/%d)\n", i, monsterList[i].name, monsterList[i].health, monsterList[i].maxHealth);
        }

        printf("\n%d - Back\n", monsterCount+1);
        printf("\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == monsterCount+1) {
            return 1;
        } else if (choice >= 0 && choice < monsterCount) {
            Monster *monster = &monsterList[choice];
            int damage = rand() % (monster->health - 15 - monster->defense + 1) + monster->defense;
            if (damage < 0) {
                damage = 0;
            }
            monster->health -= damage;
            if (monster->health < 0) {
                monster->health = 0;
            }
            if (monster->health == 0) {
                printf("You killed the monster %s\n", monster->name);
                printf("\n");
                monsterList[choice] = monsterList[monsterCount-1];
                monsterCount--;
            }
        } else {
            printf("Invalid choice\n");
            printf("\n");
        }
        removeAttack(player);
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
    printf("\n");

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
