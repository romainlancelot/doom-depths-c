EXE=prog
CC=gcc

$(EXE): main.o monsters.o player.o
	$(CC) -o $@ $^ -lpthread
	rm -rf *.o

main.o: main.c
monsters.o: monsters.c monsters.h
player.o: player.c player.h

%.o: %.c %.h
	$(CC) -c $<

install:
	apt install gcc -y

clean:
	rm -rf *.o $(EXE)