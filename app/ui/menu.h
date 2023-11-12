#ifndef DEF_HEADER_MENU
#define DEF_HEADER_MENU

#include <sqlite3.h>

#include "../entities/player.h"
#include "../entities/monsters.h"
#include "../utils/db.h"
#include "../utils/headers.h"

extern int handle_start_menu(sqlite3 *db);
extern int handle_death_menu();
extern int handle_delete_save_menu(sqlite3 *db);
extern int handle_win_menu(Player *player);
extern void _log(char *message, int line);
extern void clear(int line);
extern void print_entity(char *filename, int line, int column);

#endif // MENU_H
