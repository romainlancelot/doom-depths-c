#ifndef DB_H
#define DB_H

sqlite3 *init_database(char *db_name);
void save(sqlite3 *db, char *sql);
void update(sqlite3 *db, char *sql);
Player *load_player(sqlite3 *db, int id);
void load_all_save(sqlite3 *db);
int get_player_count(sqlite3 *db);

#endif // DB_H