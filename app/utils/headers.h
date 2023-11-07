#ifndef DEF_HEADER_DISPLAY_STANDARS
#define DEF_HEADER_DISPLAY_STANDARS

#define CLEAR_MENU printf("\e[31;H\e[J");
#define GOTO_STATS printf("\e[3;H");
#define GOTO_LOG printf("\e[7;H\e[2K\r");
#define SAVE_CURSOR printf("\e7");
#define RESTORE_CURSOR printf("\e8");
#define CLEAR_SCREEN printf("\e[2H\e[J\e[H");
#define CLEAR_ENTITIES printf("\e[8;H\e[J");
#define RESET_COLOR printf("\e[0m");

#define DB_NAME "db.sqlite3"

#define DEFAULT_HEALTH 100
#define DEFAULT_MANA 100
#define DEFAULT_ATTACK_POWER 20
#define DEFAULT_DEFENSE 10
#define ATTACK_NUMBER 3
#define WAIT_FOR_POTION 6
#define HEALING_AMOUNT 20
#define MAX_SAVE 3

#endif // DEF_HEADER_DISPLAY_STANDARS