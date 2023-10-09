#ifndef DEF_HEADER_MONSTER
#define DEF_HEADER_MONSTER


typedef struct {
    char *name;
    int health;
    int attack;
    int defense;
} Monster;


extern Monster *createRandomMonster();

#endif