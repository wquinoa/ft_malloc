
#include "libmalloc.h"

static	void ft_memcpy(void *dst, void *src, size_t n)
{
	unsigned char *s;
	unsigned char *d;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	while (1)
	{
		d[n] = s[n];
		if (n == 0)
			break ;
		--n;
	}
}

//void 	print_block(t_block *block)
//{
//	printf("block on addr %p\n", block);
//	printf("size %zu\n", block->this_size);
//	printf("block in use %d\n", block->node_in_use);
//	printf("prev size %zu\n", block->prev_size);
//	printf("prev in use %d\n", block->prev_in_use);
//	printf("magic %d %d\n", block->magic, block->magic_2);
//}

void 	realloc_set(t_block *start, t_block *mid, size_t sz)
{
	t_block *old_end;
	off_t 	diff;

	diff = sz - start->this_size;
	old_end = advance_aligned(mid, mid->this_size);
	old_end->prev_size = mid->this_size - diff;
	mid = advance_aligned(mid, diff);
	*mid = (t_block){ sz, MAGIC, 1, old_end->prev_size, MAGIC, 0 };
	start->this_size = sz;
	start->node_in_use = 1;
	g_heap->total_occupied += diff;
//pthread_mutex_unlock(&g_mallock);
}

void 	*realloc(void *ptr, size_t sz)
{
	t_block		*current;
	t_block		*next;
	size_t 		total_bytes;

	if (!ptr)
		return (malloc(sz));
	if (sz == 0)
		return (malloc(MAX_TINY));
	pthread_mutex_lock(&g_mallock);
	current = ((t_block *)ptr) - 1;
	next = get_next_block(current);
	total_bytes = subtract_addr(next, ptr) + align(next->this_size, ALIGNMENT);
	if (next->node_in_use == 0 && total_bytes >= sz)
	{
		realloc_set(current, next, sz);
		pthread_mutex_unlock(&g_mallock);
		return (current + 1);
	}
	pthread_mutex_unlock(&g_mallock);
	next = malloc(sz);
	if (!next)
		return (NULL);
	ft_memcpy(next, ptr, sz);
	free(ptr);
	return (next);
}
