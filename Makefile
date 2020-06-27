FLAGS=-O3
CC=gcc
EXES=opening selfplay userselfplay
SRC=util.c search.c eval.c ui.c init.c movegen.c

all: clean opening selfplay userselfplay

opening: $(SRC) opening.c
	$(CC) $(FLAGS) $? -o opening

selfplay: $(SRC) selfplay.c
	$(CC) $(FLAGS) $? -o selfplay

userselfplay: $(SRC) userselfplay.c
	$(CC) $(FLAGS) $? -o userselfplay

clean:
	rm -f $(EXES) *.o
