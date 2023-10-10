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

Monster *create_monster(char *name, int health, int min_attack, int max_attack, int defense);
Monsters *create_random_monster(int count);
void destroy_monsters(Monsters *monsters);
void print_monsters_list(Monsters *monsters);
void manage_player_attack(Monsters *monsters, Player *player);