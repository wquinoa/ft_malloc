
CC := gcc
NAME := libmalloc.a
SRC := libmalloc.c malloc.c mem_utils.c show_alloc_mem.c zone_create.c realloc.c
INCLUDE := libmalloc.h

OBJ := $(SRC:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $^
	gcc -g main.c $(NAME) -o test.out

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

run: $(SRC)
	rm -rf a.out
	gcc -g  $(SRC)

testfile:
	gcc garbage.c -o garbage.out && ./garbage.out