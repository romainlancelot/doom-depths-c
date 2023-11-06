#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include "../entities/player.h"
#include "../entities/monsters.h"
#include "../utils/headers.h"
#include "../utils/db.h"

/**
 * Prints the start menu of the game, including the game's logo and options to start or quit the game.
 * If the logo file cannot be opened, an error message is printed and the program exits.
 */
void _print_menu(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("%s", filename);
        printf("Error opening player ASCII file!\n");
        exit(1);
    }
    printf("\e[3;H");
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
}

/**
 * Handles user input from the command line.
 *
 * @return true if the screen should be cleared, false if the program should exit.
 */
int _handle_user_input()
{
    char user_input;
    if (read(STDIN_FILENO, &user_input, 1) == 1)
    {
        CLEAR_SCREEN;
        return user_input - '0';
    }
}

/**
 * Handles the start menu and waits for user input.
 *
 * @param db The SQLite database connection.
 * @return true if user selects option 1, false if user selects option 0.
 */
int handle_start_menu(sqlite3 *db)
{
    _print_menu("ui/resources/logo.txt");
    printf("\n\n\n1 - Start game\n");
    load_all_save(db);
    printf("0 - Quit game\n");
    return _handle_user_input();
}

/**
 * Handles the death menu.
 *
 * @return true if the user chooses to restart the game, false if the user chooses to quit the game.
 */
int handle_death_menu()
{
    _print_menu("ui/resources/dead.txt");
    printf("\n\n\n1 - Restart game\n0 - Quit game\n");
    return _handle_user_input();
}

/**
 * Handles the win state of the game by printing the win menu and prompting the user for input.
 * @return true if the user chooses to take loot and go to the next room, false if the user chooses to quit.
 */
int handle_win_menu()
{
    _print_menu("ui/resources/win.txt");
    printf("\n\n\n1 - Take loot and go to next room\n0 - Quit\n");
    return _handle_user_input();
}