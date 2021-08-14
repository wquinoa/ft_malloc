//
// DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=/Users/user/ft_malloc/libft_malloc_x86_64_Darwin.dylib
// unset DYLD_INSERT_LIBRARIES; make re
#include "libmalloc.h"

int main()
{
	for (int i = 0; i < 90; i++)
		malloc(1);
	for (int i = 0; i < 90; i++)
		malloc(65);
	show_alloc_mem_ex();
	show_alloc_mem();
	write(1, "done\n", 5);
	return 0;
}