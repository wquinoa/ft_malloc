#include "libmalloc.h"

inline size_t 	align(size_t size, size_t bound)
{
	return (((size) + (bound - 1)) & ~(bound - 1));
}

inline size_t	to_cell_size(size_t size)
{
	return (((align(size, 16) + 1) >> 4) + 1);
}

inline size_t	substract_addr(void *x, void *y)
{
	printf("%zu sub addr\n", (char *)x - (char *)y);
	return (char *)x - (char *)y;
}

inline void		*advance_addr(void *x, off_t offset)
{
	return (char *)x + offset;
}