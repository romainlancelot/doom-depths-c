#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>

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
    // Function body
}
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
}