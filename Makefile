
CC := gcc
NAME := libmalloc.a
SRC := libmalloc.c
INCLUDE := libmalloc.h

OBJ := $(SRC:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $^
	gcc main.c $(NAME) -o test.out

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

run:
	rm a.out
	gcc $(SRC)