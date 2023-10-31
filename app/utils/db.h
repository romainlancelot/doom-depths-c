#ifndef DB_H
#define DB_H

void init_database(char *db_name);
void save(char *db_name, char *sql);
void update(char *db_name, char *sql);
Player *load_player(char *db_name, int id);
void load_all_save(char *db_name);

#endif // DB_H