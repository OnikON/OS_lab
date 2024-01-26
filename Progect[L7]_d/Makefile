SRC = cp-r.c
CС=gcc
CFLAGS= -lpthread
OBJ = $(SRC:.c=.o)
NAME = main

all: $(NAME) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	rm *.o

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

rebuild: all

.o : .c
	$(CC) $(CFLAGS) -c %< -o $@

c : 
	rm *.o

clean :
	rm $(OBJ)
	rm $(NAME)
	rm *.o