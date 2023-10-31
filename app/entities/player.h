#if !defined(PLAYER)
#define PLAYER

typedef struct
{
    int id;
    int current_health;
    int max_health;
    int current_mana;
    int max_mana;
    int gold;
    int experience;
    int level;
    int defense;
    int attack_power;
    int attack_left;
} Player;
Player *create_player();
void print_player_stats(Player *player);
void print_player();
void heal_player(Player *player, int amount);
char *save_player(Player *player);
char *update_player(Player *player);

#endif // PLAYER