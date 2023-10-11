typedef struct
{
    int current_health;
    int max_health;
    int current_mana;
    int max_mana;
    int gold;
    int experience;
    int level;
    int attack_power;
    int attack_left;
} Player;
Player *create_player();
void print_player_stats(Player *player);
void print_player();
void manage_player_attack(Monsters *monsters, Player *player);