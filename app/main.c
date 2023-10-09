#include <stdio.h>
#include "monsters.h"

int main()
{
    Monster **monsters = create_random_monster(10);
    for (int i = 0; i < 10; i++)
    {
        printf("%s\n", monsters[i]->name);
    }
    printf("Hello, World!");
    return 0;
}