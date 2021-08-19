//
// DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=/Users/user/ft_malloc/libft_malloc_x86_64_Darwin.dylib
// unset DYLD_INSERT_LIBRARIES; make re
//#include <stdlib.h>
#include <unistd.h>
#include "src/libft_malloc.h"

int main()
{
	char *ptr[5];

	write(1, "\nMAIN START\n\n", 13);
	for (int i = 0; i < 5; i++) {
		ptr[i] = malloc(1);
		*ptr[i] = '*';
	}
	show_alloc_mem_ex();
	for (int i = 0; i < 5; i++) {
		free(ptr[i]);
	}
	write(1, "\nMAIN END\n\n", 11);
	return 0;
}