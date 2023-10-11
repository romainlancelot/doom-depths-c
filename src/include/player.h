#ifndef DEF_HEADER_PLAYER
#define DEF_HEADER_PLAYER


typedef struct {
    int health;
    int maxHealth;
    int attack;
    int maxAttack;
} Player;


extern Player player;
extern Player *createPlayer();
extern Player *createRandomPlayer();
extern Player *removeAttack(Player *player);
extern Player *removeHealth(Player *player, int health);

#endif
