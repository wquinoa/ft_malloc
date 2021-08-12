#include "libmalloc.h"

inline size_t 	align(size_t size, size_t bound)
{
	return (((size) + (bound - 1)) & ~(bound - 1));
}

inline size_t	substract_addr(void *x, void *y)
{
	return (char *)x - (char *)y;
}

inline void		*advance_aligned(void *x, off_t offset)
{
	return ((char *)x + align(offset, 16));
}