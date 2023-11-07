#ifndef DB_H
#define DB_H

sqlite3 *init_database(char *db_name);
void save(sqlite3 *db, char *sql);
Player *load_player(sqlite3 *db, int id);
void load_all_save(sqlite3 *db);
int get_player_count(sqlite3 *db);
Monsters *load_monsters(sqlite3 *db, int id);
void clear_monsters(sqlite3 *db, int id);
void delete_save(sqlite3 *db, int id);

#endif // DB_H