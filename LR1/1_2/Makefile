CС=gcc
CFLAGS= -g -Wall -lpthread
SRC = d.c
OBJ = $(SRC:.c=.o)
NAME = main

all: $(NAME) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

rebuild: all

.o : .c
	$(CC) $(CFLAGS) -c %< -o $@

clean :
	rm $(OBJ)
	rm $(NAME)
	rm *.o