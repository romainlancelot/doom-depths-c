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
void init_database(char *db_name)
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
    sqlite3_close(db);
}

void save(char *db_name, char *sql)
{
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, false);
    sqlite3_close(db);
}

void update(char *db_name, char *sql)
{
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, false);
    sqlite3_close(db);
}

static int _load_player(Player *player, int argc, char **argv, char **azColName)
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

Player *load_player(char *db_name, int id)
{
    sqlite3 *db;
    char *err_msg = 0;
    Player *player = malloc(sizeof(Player));

    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);

    char *sql = malloc(100 * sizeof(char));
    sprintf(sql, "SELECT * FROM players WHERE id = %d", id);
    rc = sqlite3_exec(db, sql, _load_player, player, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);

    sqlite3_close(db);
    return player;
}

static int _load_all_save(void *data, int argc, char **argv, char **azColName)
{
    printf("%d - Load player level %s: health: %s/%s, mana: %s/%s, gold: %s\n", atoi(argv[0]) + 1, argv[7], argv[1], argv[2], argv[3], argv[4], argv[5]);
    return 0;
}

void load_all_save(char *db_name)
{
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);

    char *sql = malloc(100 * sizeof(char));
    sprintf(sql, "SELECT * FROM players");
    rc = sqlite3_exec(db, sql, _load_all_save, NULL, &err_msg);
    if (rc != SQLITE_OK)
        _handle_sql_error(db, err_msg, true);
    sqlite3_close(db);
}