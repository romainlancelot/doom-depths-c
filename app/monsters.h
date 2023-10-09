typedef struct
{
    char *name;
    int current_health;
    int max_health;
    int min_attack;
    int max_attack;
    int defense;
} Monster;
Monster *create_monster(char *name, int health, int min_attack, int max_attack, int defense);
Monster **create_random_monster(int count);
void destroy_monsters(Monster **monsters, int count);
void print_monsters_list(Monster **monsters, int count);