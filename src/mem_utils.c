#include "libft_malloc.h"

/*
**	align [size] to be exactly divisible by [bound]
*/

inline size_t 	align(size_t size, size_t bound)
{
	return (((size) + (bound - 1)) & ~(bound - 1));
}

/*
**	returns amount of bytes between x and y
*/

inline off_t 	subtract_addr(const void *x, const void *y)
{
	return (char *)x - (char *)y;
}

/*
**	returns new address + offset aligned by 16 bytes
*/

inline void		*advance_aligned(const void *x, off_t offset)
{
	return ((char *)x + align(offset, ALIGNMENT));
}

/*
**	returns the address of the next t_block
*/

inline void 	*get_next_block(t_block const *x)
{
	return (advance_aligned(x + 1, x->this_size));
}

inline void 	*get_prev_block(t_block const *x)
{
	return (advance_aligned(x - 1, -x->prev_size));
}

inline int 	check_magic(t_block const *current)
{
	return (current->magic == current->magic_2 && current->magic_2 == MAGIC);
}
