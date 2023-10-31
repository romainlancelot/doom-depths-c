#ifndef DB_H
#define DB_H

sqlite3 *init_database(char *db_name);
void save(sqlite3 *db, char *sql);
void update(sqlite3 *db, char *sql);
Player *load_player(char *db_name, int id);
void load_all_save(char *db_name);

#endif // DB_H