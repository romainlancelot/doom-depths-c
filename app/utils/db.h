#ifndef DB_H
#define DB_H

#include "../entities/player.h"
#include "../entities/monsters.h"
#include "../utils/stuff.h"
#include "../utils/spell.h"

extern sqlite3 *init_database(char *db_name);
extern void save(sqlite3 *db, char *sql);
extern Player *load_player(sqlite3 *db, int id);
extern void clear_stuff(sqlite3 *db, int id);
extern void load_all_save(sqlite3 *db);
extern int get_player_count(sqlite3 *db);
extern Monsters *load_monsters(sqlite3 *db, int id);
extern void clear_monsters(sqlite3 *db, int id);
extern void delete_save(sqlite3 *db, int id);
extern Spells *load_spells(sqlite3 *db, int id);
void clear_spell(sqlite3 *db, int id);

#endif // DB_H
