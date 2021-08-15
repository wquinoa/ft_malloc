
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
SOURCE :=	malloc.c free.c	realloc.c\
 			show_alloc_mem.c zone_create.c\
 			show_alloc_mem_ex.c malloc_global.c\
 			mem_utils.c printing_utils.c

CC := gcc
CFLAGS := -I/src/libmalloc.h -Wall -Werror -Wextra -Wcast-align
SRC := $(addprefix src/, $(SOURCE))
OBJ := $(SRC:.c=.o)

ENVARS := DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=/Users/user/ft_malloc/$(NAME)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(CFLAGS) -shared -o $@ -fPIC $^

clean:
	rm -rf a.out* $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

test: all
	 $(ENVARS) ls
