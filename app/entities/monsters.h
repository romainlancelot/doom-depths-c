#ifndef DEF_HEADER_MONSTERS
#define DEF_HEADER_MONSTERS

typedef struct
{
    char *name;
    int current_health;
    int max_health;
    int min_attack;
    int max_attack;
    int defense;
} Monster;

typedef struct
{
    Monster **monsters;
    int count;
} Monsters;

#include "../utils/headers.h"
#include "../entities/player.h"
#include "../ui/menu.h"

extern Monster *create_monster(char *name, int health, int min_attack, int max_attack, int defense);
extern Monsters *create_random_monster(Player *player, int count);
extern Monsters *create_random_champion(int count);
extern void destroy_monsters(Monsters *monsters);
extern void print_monsters_list(Monsters *monsters);
extern void remove_monster(Monsters *monsters, Monster *monster);
extern char *save_monsters(Monsters *monsters, int id);

#endif // MONSTERS
