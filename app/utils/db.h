#ifndef DB_H
#define DB_H

void init_database(char *db_name);
void save_in_database(char *db_name, char *sql);
Player *load_player(char *db_name, int id);
void load_all_save();

#endif // DB_H