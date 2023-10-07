CC = gcc
# CFLAGS= -O2 -Wall -Wextra -Werror
# CLIBS= -lm
EXE = doomdepth
OBJ = objects/
INCL = include/
FILEC = $(wildcard *.c)
FILEH = $(wildcard $(INCL)*.h)
FILEO:= $(patsubst $(SRC)%.c,$(OBJ)%.o,$(FILEC))

$(EXE) : $(FILEO)
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

$(OBJ)main.o : $(SRC)main.c $(FILEH)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)%.o : $(SRC)%.c $(INCL)%.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -Rf $(OBJ)*.o
	rm -Rf $(EXE)
