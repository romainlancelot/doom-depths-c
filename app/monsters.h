typedef struct
{
    char *name;
    int health;
    int min_attack;
    int max_attack;
    int defense;
} Monster;
Monster *create_monster(char *name, int health, int min_attack, int max_attack, int defense);
Monster **create_random_monster(int count);
void destroy_monsters(Monster **monsters, int count);