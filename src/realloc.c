
#include "libft_malloc.h"

static void		ft_memcpy(void *dst, void *src, size_t n)
{
	unsigned char *s;
	unsigned char *d;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	while (n)
	{
		d[n] = s[n];
		--n;
	}
}

void 			realloc_set(t_block *start, t_block *mid, size_t sz)
{
	t_block 	*old_end;
	off_t 		diff;

	diff = sz - start->this_size;
	old_end = get_next_block(mid);
	old_end->prev_size = mid->this_size - diff;
	mid = advance_aligned(mid, diff);
	*mid = (t_block){ sz, MAGIC, 1, old_end->prev_size, MAGIC, 0 };
	start->this_size = sz;
	start->node_in_use = 1;
	get_heap()->total_occupied += diff;
}


static void 	*realloc_internal(void *ptr, size_t sz)
{
	t_block		*current;
	t_block		*next;
	size_t 		total_bytes;
	const int 	is_my_block = check_magic((t_block *)ptr - 1);

	current = (t_block *)ptr - 1;
	if (is_my_block)
	{
		next = get_next_block(current);
		total_bytes = subtract_addr(next, current) + next->this_size;
		if (next->node_in_use == 0 && total_bytes >= sz)
		{
			realloc_set(current, next, sz);
			return (current + 1);
		}
	}
	next = malloc_internal(sz);
	if (!next)
		return (NULL);
	ft_memcpy(next, ptr, sz);
	free_internal(ptr);
	return (next);
}

void 		*realloc(void *ptr, size_t sz)
{
	if (!ptr)
		return (malloc(sz));
	if (sz == 0)
		return (malloc(MAX_SMALL));
	lock_main();
	ptr = realloc_internal(ptr, sz);
	unlock_main();
	return (ptr);
}
