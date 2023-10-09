#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include "monsters.h"
#include "player.h"

struct termios setup_terminal()
{
    system("clear"); // Clear the terminal
    struct termios original;
    tcgetattr(STDIN_FILENO, &original);

    struct termios modified = original;
    modified.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &modified);

    return original;
}

void *display_title()
{
    while (1)
    {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("\033[0;0HDoomdepths - %02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        fflush(stdout);
        sleep(1);
    }
}

int main()
{
    setup_terminal();
    Monster **monsters = create_random_monster(10);

    time_t last_update_time = time(NULL);
    time_t rawtime;
    struct tm *timeinfo;

    Player *player = create_player();

    pthread_t tid;
    pthread_create(&tid, NULL, display_title, NULL);

    char ch;
    while (1)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        print_player_stats(player);
        if (read(STDIN_FILENO, &ch, 1) == 1)
        {
            if (ch == 'q')
            {
                break;
            }
            printf("Vous avez appuyé sur : %c\n", ch);
        }
    }

    pthread_cancel(tid);
    pthread_join(tid, NULL);

    return 0;
}