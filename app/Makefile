EXE=prog
CC=gcc

$(EXE): main.o entities/monsters.o entities/player.o ui/menu.o utils/attack.o utils/db.o utils/stuff.o utils/spell.o
	$(CC)  $^ -lpthread -lsqlite3 -ldl -lm -o $@
	find -type f -name "*.o" | xargs rm 

main.o: main.c
entities/monsters.o: entities/monsters.c entities/monsters.h
entities/player.o: entities/player.c entities/player.h
ui/menu.o: ui/menu.c ui/menu.h
utils/stuff.o: utils/stuff.c utils/stuff.h
utils/attack.o: utils/attack.c utils/attack.h
utils/db.o: utils/db.c utils/db.h
utils/spell.o: utils/spell.c utils/spell.h

%.o: %.c %.h
	$(CC) -c $< -o $@

install:
	apt install gcc sqlite-dev sqlite -y

clean:
	rm -rf *.o $(EXE)