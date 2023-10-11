#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "headers.h"

/**
 * Prints the start menu of the game, including the game's logo and options to start or quit the game.
 * If the logo file cannot be opened, an error message is printed and the program exits.
 */
void _print_menu()
{
    FILE *file = fopen("resources/logo.txt", "r");
    if (file == NULL)
    {
        printf("Error opening player ASCII file!\n");
        exit(1);
    }
    printf("\e[3;H");
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    printf("\n\n1 - Start game\n\n0 - Quit game\n");
}

/**
 * @brief Handles the start menu and waits for user input.
 *
 * @return true if user selects option 1, false if user selects option 0.
 */
bool handle_start_menu()
{
    _print_menu();
    char user_input;
    while (true)
    {
        if (read(STDIN_FILENO, &user_input, 1) == 1)
        {
            switch (user_input)
            {
            case '1':
                CLEAR_SCREEN;
                return true;
            case '0':
                return false;
            }
        }
    }
}