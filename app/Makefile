EXE=prog
CC=gcc

$(EXE): main.o monsters.o
	$(CC) -o $@ $^
	rm -rf *.o

main.o: main.c
monsters.o: monsters.c monsters.h

%.o: %.c %.h
	$(CC) -c $<

install:
	apt install gcc -y

clean:
	rm -rf *.o $(EXE)