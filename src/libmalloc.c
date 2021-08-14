#include "libmalloc.h"

t_heap			*g_heap;
pthread_mutex_t g_mallock;

static inline int 	is_freeable(t_block *block)
{
	block -= 1;
	return (block->magic == block->magic_2 && !block->node_in_use);
}

void	free(void *mem)
{
	t_block *header;
	t_block *next;

	header = advance_aligned(mem, -sizeof(t_block));
	if (!mem || is_freeable((t_block *)mem))
	{
		//printf("double free\n");
		return;
	}
	pthread_mutex_lock(&g_mallock);
	next = advance_aligned(header + 1, header->this_size);
	header->node_in_use = 0;
	g_heap->total_occupied -= header->this_size;
	header->this_size = subtract_addr(next, header + 1);
	next->prev_in_use = 0;
	next->prev_size = header->this_size;
	pthread_mutex_unlock(&g_mallock);
}
