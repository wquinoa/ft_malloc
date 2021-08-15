//
// DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=/Users/user/ft_malloc/libft_malloc_x86_64_Darwin.dylib
// unset DYLD_INSERT_LIBRARIES; make re
#include "src/libft_malloc.h"

void func2(void **ptr)
{
	for (int i = 0; i < 5; i++)
		*ptr = malloc(65);
}

void func1(void **ptr)
{
	for (int i = 0; i < 5; i++)
		*ptr = malloc(1);
	func2(ptr);
}

int main()
{
	void *ptr;

	private_lock_init();
	func1(&ptr);
	show_alloc_mem();
	write(1, "done\n", 5);
	return 0;
}