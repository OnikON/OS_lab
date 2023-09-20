CС=gcc
CFLAGS= -pthread
SRC = main.c
OBJ = $(SRC:.c=.o)
NAME = main

all: $(NAME) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

rebuild: all

time: $(NAME) tobj
	$(CC) $(CFLAGS) -DTIME $(OBJ) -o $(NAME)

tobj:
	$(CC) $(CFLAGS) -DTIME -c $(SRC) -o $(OBJ)
	
.o : .c
	$(CC) $(CFLAGS) -c %< -o $@

clean :
	rm $(OBJ)
	rm $(NAME)