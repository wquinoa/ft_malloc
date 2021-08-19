
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME :=		libft_malloc_$(HOSTTYPE).so
SYMLINK :=	libft_malloc.so
SOURCE :=	malloc.c free.c	realloc.c\
 			show_alloc_mem.c zone_create.c\
 			show_alloc_mem_ex.c malloc_global.c\
 			mem_utils.c printing_utils.c

WHT = \033[0m
GRN = \033[32m
RED = \033[31m
WHT1 = \033[0;1m
GRN1 = \033[32;1m
RED1 = \033[31;1m
DRK = \033[2m
REPLACE =  2>&1| awk '{sub(/.\//,"  $(WHT)	$(DRK)removed $(RED)")}1'

BIN := ./bin/
CFLAGS := -I/src/libft_malloc.h -Wall -Werror -Wextra -Wcast-align
SRC := $(addprefix src/, $(SOURCE))
OBJECTS := $(SOURCE:c=o)
OBJ := $(addprefix $(BIN), $(SOURCE:c=o))

ENVARS := DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=/Users/user/ft_malloc/$(NAME)

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJ)
	@gcc $(CFLAGS) -shared -o $@ -fPIC $^
	@ln -sf $(NAME) $(SYMLINK)
	@echo "\n\n	Created $(DRK)$(GRN)$(NAME)$(WHT)\n"

$(BIN):
	@mkdir $@

$(BIN)%.o: src/%.c | $(BIN)
	@gcc $(CFLAGS) -o $@ -c $<
	@printf "	Adding $(DRK)$(GRN)%-42s$(WHT)\r" "$@..."

clean:
	@printf "$(WHT)"
	@rm -rfv ./$(OBJ) $(REPLACE)
	@printf "$(WHT)"

fclean: clean
	@rm -rfv ./$(NAME) ./$(SYMLINK) ./$(BIN) a.out* $(REPLACE)
	@printf "$(WHT)"

re: fclean all

test: all
	$(CC) main.c libft_malloc_x86_64_Darwin.so && $(ENVARS) ./a.out
