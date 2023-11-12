#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>

#include "menu.h"

/**
 * @brief Logs a message to the console at a specific line.
 *
 * @param message The message to log.
 * @param line The line number to log the message at.
 */
void _log(char *message, int line)
{
    SAVE_CURSOR;
    printf("\e[%d;H\e[2K", line);
    printf("%s", message);
    RESTORE_CURSOR;
}

/**
 * Clears the console screen from the given line number to the bottom of the screen.
 * @param line The line number to start clearing from.
 */
void clear(int line)
{
    printf("\e[%d;H\e[J\r", line);
    fflush(stdout);
}

/**
 * Prints the contents of a file to the console with optional color formatting.
 * @param filename The name of the file to print.
 * @param line The line number to start printing from.
 * @param column The column number to start printing from.
 */
void print_entity(char *filename, int line, int column)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening player ASCII file %s!\n", filename);
        exit(1);
    }
    printf("\e[%d;H", line);
    printf("\e[%dC", (column * 20) * 2);
    char character;
    while ((character = fgetc(file)) != EOF)
    {
        if (character >= '0' && character <= '9')
        {
            char color[4];
            int i = 0;
            for (; character >= '0' && character <= '9'; i++)
            {
                color[i] = character;
                character = fgetc(file);
            }
            color[i] = '\0';

            printf("\e[38;5;%sm", color);
            if (atoi(color) == 0)
                RESET_COLOR;
            printf("%c", character);
        }
        else
        {
            printf("%c", character);
            if (column && (character == '\n' || character == '\r'))
                printf("\e[%dC", (column * 20) * 2);
        }
    }
    RESET_COLOR;
}

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
    char character;
    while ((character = fgetc(file)) != EOF)
    {
        if (character >= '0' && character <= '9')
        {
            char color[4];
            int i = 0;
            for (; character >= '0' && character <= '9'; i++)
            {
                color[i] = character;
                character = fgetc(file);
            }
            color[i] = '\0';

            printf("\e[38;5;%sm", color);
            if (atoi(color) == 0)
                RESET_COLOR;
            printf("%c", character);
        }
        else
        {
            printf("%c", character);
        }
    }
    RESET_COLOR;
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
        return user_input - '0';
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
    printf("\n9 - Delete a save\n\n0 - Quit game\n");
    return _handle_user_input();
}

/**
 * Displays a menu for deleting a save file and handles user input.
 *
 * @param db A pointer to the SQLite database.
 * @return The user's input as an integer.
 */
int handle_delete_save_menu(sqlite3 *db)
{
    _print_menu("ui/resources/logo.txt");
    printf("\n\n\nChoose a save to delete:\n\n");
    load_all_save(db);
    printf("\n0 - Return to main menu\n");
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
int handle_win_menu(Player *player)
{
    _print_menu("ui/resources/win.txt");
    if(rand() % 2)
    {
        int gold = rand() % 29 + 1;
        int lost_hp = rand() % 14 + 1;
        int mana_gain = rand() % 49 + 1;
        switch (rand() % 3) {
            case 0:
                printf("\n\nYou found %d gold on you way to the next stage!", gold);
                player->gold += gold;
                break;
            case 1:
                printf("\n\nYou stumpled on a trap and lost %d HP", lost_hp);
                player->current_health -= lost_hp;
                break;
            case 2:
                printf("\n\nYou stumble accros a mana fountain and gain %d MP", mana_gain);
                player->current_mana += mana_gain;
                break;
        }
    }

    printf("\n\n\nPress any key to go to stage %d.\n", player->stage);
    return _handle_user_input();
}
