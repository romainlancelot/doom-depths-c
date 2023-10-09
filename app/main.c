#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
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
        printf("\033[0;0HDoomdepths - %02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        printf("\033[7;0H");
        fflush(stdout);
        sleep(1);
    }
}

int main()
{
    setup_terminal();
    pthread_t tid;
    pthread_create(&tid, NULL, display_title, NULL);

    Monster **monsters = create_random_monster(10);
    Player *player = create_player();

    char user_input;
    while (1)
    {

        print_player_stats(player);
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            if (user_input == 'q')
            {
                break;
            }
            printf("Vous avez appuyé sur : %c\n", user_input);
        }
    }

    pthread_cancel(tid);
    pthread_join(tid, NULL);

    return 0;
}