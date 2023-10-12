EXE=prog
CC=gcc

$(EXE): main.o entities/monsters.o entities/player.o ui/menu.o utils/attack.o
	$(CC) -o $@ $^ -lpthread
	find -type f -name "*.o" | xargs rm 

main.o: main.c
entities/monsters.o: entities/monsters.c entities/monsters.h
entities/player.o: entities/player.c entities/player.h
ui/menu.o: ui/menu.c ui/menu.h
utils/attack.o: utils/attack.c utils/attack.h

%.o: %.c %.h
	$(CC) -c $< -o $@

install:
	apt install gcc -y

clean:
	rm -rf *.o $(EXE)