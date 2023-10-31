#ifndef DEF_HEADER_DISPLAY_STANDARS
#define DEF_HEADER_DISPLAY_STANDARS

#define CLEAR_MENU printf("\e[31;H\e[J");
#define GOTO_STATS printf("\e[3;H");
#define GOTO_LOG printf("\e[7;H\e[2K\r");
#define SAVE_CURSOR printf("\e7");
#define RESTORE_CURSOR printf("\e8");
#define CLEAR_SCREEN printf("\e[2H\e[J\e[H");

#define DB_NAME "db.sqlite3"

#endif // DEF_HEADER_DISPLAY_STANDARS