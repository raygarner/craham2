FLAGS=-O3
CC=gcc
EXES=white black selfplay userselfplay
SRC=rules.c search.c eval.c ui.c board.c movegen.c globals.c

all: clean white black selfplay userselfplay

white: $(SRC) white.c
	$(CC) $(FLAGS) $? -o white

black: $(SRC) black.c
	$(CC) $(FLAGS) $? -o black

selfplay: $(SRC) selfplay.c
	$(CC) $(FLAGS) $? -o selfplay

userselfplay: $(SRC) userselfplay.c
	$(CC) $(FLAGS) $? -o userselfplay

clean:
	rm -f $(EXES) *.o
