#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "monsters.h"
#include "player.h"

/**
 * Sets up the terminal by clearing it and modifying its attributes.
 */
void setup_terminal()
{
    system("clear"); // Clear the terminal
    struct termios original;
    tcgetattr(STDIN_FILENO, &original);

    struct termios modified = original;
    modified.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &modified);
}

/**
 * Displays the title of the game along with the current time in hours, minutes, and seconds.
 * The function runs indefinitely until the program is terminated.
 */
void display_title()
{
    while (1)
    {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("\033[HDoomdepths - %02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        printf("\033[38;H");
        fflush(stdout);
        sleep(1);
    }
}

/**
 * Displays the game menu with options for the player to choose from.
 */
void display_menu()
{
    printf("\033[J"); // Clear the screen from the current line to the bottom
    printf("1 - Attack\n");
    printf("2 - Use healing potion (20)\n");
    printf("3 - Inventory\n\n");
    printf("8 - End turn\n\n");
    printf("0. Quit\n");
}

int main()
{
    setup_terminal();
    pthread_t tid;
    pthread_create(&tid, NULL, display_title, NULL);

    Monster **monsters = create_random_monster(4);
    Player *player = create_player();
    bool show_menu = true;

    char user_input;
    while (player->current_health > 0)
    {
        print_player_stats(player);
        print_player();
        display_menu();

        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            switch (user_input)
            {
            case '1':
                print_monsters_list(monsters, 4);
                break;
            case 2:
                break;
            case '0':
                goto end;
            }
        }
    }

end:
    printf("\nQuitting game, thanks for playing !\n");
    free(player);
    pthread_cancel(tid);
    pthread_join(tid, NULL);

    return 0;
}