#ifndef DEF_HEADER_DISPLAY_STANDARS
#define DEF_HEADER_DISPLAY_STANDARS

#define CLEAR_MENU printf("\e[31;H\e[J");
#define GOTO_LOG printf("\e[7;H\e[2K\r");
#define LINE_BREAK printf("\e[1B");
#define SAVE_CURSOR printf("\e7");
#define RESTORE_CURSOR printf("\e8");
#define CLEAR_LINE printf("\e[2K\r");

#define SEPARATEUR printf(" -------------------------------------------- \n\n");

#endif