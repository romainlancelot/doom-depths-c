#include "include/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void startGame() {
    srand(time(NULL));
    
    for (int i = 0; i < rand() % 4 + 1; i++) {
        Monster *monster = createRandomMonster();
        printf("Monster %d: %s\n", i + 1, monster->name);
        printf("Health: %d\n", monster->health);
        printf("Attack: %d\n", monster->attack);
        printf("Defense: %d\n", monster->defense);
        printf("\n");
    }
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
            startGame();
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
}
