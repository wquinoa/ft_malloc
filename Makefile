
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
SOURCE :=	malloc.c free.c	realloc.c\
 			show_alloc_mem.c zone_create.c\
 			show_alloc_mem_ex.c malloc_global.c\
 			mem_utils.c printing_utils.c

BIN := ./bin/
CFLAGS := -I/src/libft_malloc.h -Wall -Werror -Wextra -Wcast-align
SRC := $(addprefix src/, $(SOURCE))
OBJECTS := $(SOURCE:c=o)
OBJ := $(addprefix $(BIN), $(SOURCE:c=o))

ENVARS := DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=/Users/user/ft_malloc/$(NAME)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(CFLAGS) -shared -o $@ -fPIC $^
	ln -sf $(NAME) libft_malloc.so

$(BIN):
	mkdir $@

$(BIN)%.o: src/%.c | $(BIN)
	gcc $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) libft_malloc.so a.out*

re: fclean all

test: all
	$(CC) main.c libft_malloc_x86_64_Darwin.so && $(ENVARS) ./a.out

test_clear: all
	$(ENVARS) clear

test_ls: all
	$(ENVARS) ls

test_man: all
	$(ENVARS) man ascii