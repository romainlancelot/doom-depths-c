#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "../entities/player.h"

/**
 * Handles SQLite errors by printing the error message to stderr and exiting the program.
 *
 * @param db The SQLite database connection.
 * @param err_msg The error message to handle.
 */
void _handle_sql_error(sqlite3 *db, char *err_msg, bool exit_program)
{
    fprintf(stderr, "SQL error: %s\n", err_msg);
    if (exit_program == true)
    {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(0);
    }
}

/**
 * Reads SQL statements from a file and returns them as a string.
 *
 * @param sql_filename The name of the file containing the SQL statements.
 * @return A string containing the SQL statements.
 */
char *read_sql(char *sql_filename)
{
    FILE *file = fopen(sql_filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Can't open file: %s\n", sql_filename);
        exit(0);
    }
    char *sql = malloc(1000 * sizeof(char));
    char *line = malloc(100 * sizeof(char));
    while (fgets(line, 100, file) != NULL)
    {
        strcat(sql, line);
    }
    fclose(file);
    return sql;
}

/**
 * Initializes a SQLite database with the given name and applies a list of SQL files to it.
 *
 * @param db_name The name of the database to be initialized.
 */
sqlite3 *init_database(char *db_name)
{
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);

    FILE *file = fopen("utils/sql/list.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Can't open file listing SQL files to apply.\n");
        exit(0);
    }

    char *buffer = malloc(100 * sizeof(char));
    while (fgets(buffer, 100, file))
    {
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove trailing '\n' from buffer
        char *sql = read_sql(buffer);
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK)
            _handle_sql_error(db, err_msg, false);
    }
    fclose(file);
    return db;
}

/**
 * Saves data to a SQLite database.
 *
 * @param db The SQLite database to save the data to.
 * @param sql The SQL statement to execute.
 */
void save(sqlite3 *db, char *sql)
{
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, false);
}

/**
 * Updates the specified SQLite database with the given SQL statement.
 *
 * @param db The SQLite database to update.
 * @param sql The SQL statement to execute.
 */
void update(sqlite3 *db, char *sql)
{
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, false);
}

/**
 * Loads player data from the database query result into a Player struct.
 *
 * @param player Pointer to the Player struct to load the data into.
 * @param argc Number of columns in the query result.
 * @param argv Array of strings containing the values of the columns in the query result.
 * @param azColName Array of strings containing the names of the columns in the query result.
 * @return 0 if successful.
 */
static int _load_player(Player *player, int argc, char **argv, char **columns)
{
    player->id = atoi(argv[0]);
    player->current_health = atoi(argv[1]);
    player->max_health = atoi(argv[2]);
    player->current_mana = atoi(argv[3]);
    player->max_mana = atoi(argv[4]);
    player->gold = atoi(argv[5]);
    player->experience = atoi(argv[6]);
    player->level = atoi(argv[7]);
    player->defense = atoi(argv[8]);
    player->attack_power = atoi(argv[9]);
    player->attack_left = atoi(argv[10]);
    return 0;
}

/**
 * Loads a player from the specified database with the given id.
 *
 * @param db The SQLite database to load the player from.
 * @param id The id of the player to load.
 * @return A pointer to the loaded player.
 */
Player *load_player(sqlite3 *db, int id)
{
    char *err_msg = 0;
    Player *player = malloc(sizeof(Player));
    char *sql = malloc(100 * sizeof(char));
    sprintf(sql, "SELECT * FROM players WHERE id = %d", id);
    int rc = sqlite3_exec(db, sql, _load_player, player, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);
    return player;
}

/**
 * This function is a callback used to load all saved player data from the database.
 * It prints the loaded player's level, health, mana, and gold to the console.
 *
 * @param data A pointer to user data that can be passed to the callback function.
 * @param argc The number of arguments passed to the callback function.
 * @param argv An array of strings containing the values of the arguments passed to the callback function.
 * @param columns An array of strings containing the names of the columns in the result set.
 *
 * @return 0 to continue executing the SQL statement, or non-zero to halt execution.
 */
static int _load_all_save(void *data, int argc, char **argv, char **columns)
{
    printf("%d - Load player level %s: health: %s/%s, mana: %s/%s, gold: %s\n", atoi(argv[0]) + 1, argv[7], argv[1], argv[2], argv[3], argv[4], argv[5]);
    return 0;
}

/**
 * Loads all saved data from the specified SQLite database file.
 *
 * @param db The SQLite database to load the data from.
 */
void load_all_save(sqlite3 *db)
{
    char *err_msg = 0;
    char *sql = malloc(100 * sizeof(char));
    sprintf(sql, "SELECT * FROM players");
    int rc = sqlite3_exec(db, sql, _load_all_save, NULL, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);
}

/**
 * @brief Callback function for loading player count from database query.
 *
 * @param count Pointer to integer to store player count.
 * @param argc Number of columns in result set.
 * @param argv Array of pointers to strings representing column data.
 * @param columns Array of pointers to strings representing column names.
 * @return Always returns 0.
 */
static int _load_player_count(int *count, int argc, char **argv, char **columns)
{
    *count += 1;
    return 0;
}

/**
 * @brief Gets the number of players in the database.
 *
 * @param db Pointer to SQLite database object.
 * @return Number of players in the database.
 */
int get_player_count(sqlite3 *db)
{
    char *err_msg = 0;
    int count = 0;
    char *sql = malloc(100 * sizeof(char));
    sprintf(sql, "SELECT * FROM players");
    int rc = sqlite3_exec(db, sql, _load_player_count, &count, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);
    return count;
}