
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC := gcc
CFLAGS := -Wall -Werror -Wextra
NAME := libft_malloc_$(HOSTTYPE).so
SOURCE := libmalloc.c malloc.c mem_utils.c show_alloc_mem.c zone_create.c realloc.c show_alloc_mem_ex.c
INCLUDE := libmalloc.h
SRC := $(addprefix src/, $(SOURCE))

OBJ := $(SRC:.c=.o)

ENVARS := DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=/Users/user/ft_malloc/libft_malloc_x86_64_Darwin.dylib

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(CFLAGS) -shared -o $@ -fPIC $^

dylib: $(OBJ)
	gcc $(CFLAGS) -dynamiclib $^ -o libft_malloc_$(HOSTTYPE).dylib

clean:
	rm -rf a.out*
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

run: $(SRC)
	gcc -g  $(SRC)
	./a.out

setvars:
	$(ENVARS)

test: dylib
	 $(ENVARS) ls

testfile:
	gcc garbage.c -o garbage.out && ./garbage.out