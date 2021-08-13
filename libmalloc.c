#include "libmalloc.h"
#include <stdio.h>

t_heap g_heap;

static inline int 	is_freeable(t_block *block)
{
	block -= 1;
	return (block->magic == block->magic_2 && !block->node_in_use);
}

void	ft_free(void *mem)
{
	t_block *header;
	t_block *next;

	header = advance_aligned(mem, -sizeof(t_block));
	if (!mem || is_freeable((t_block *)mem))
	{
		printf("double free\n");
		return;
	}
	next = advance_aligned(header + 1, header->node_size);
	header->node_in_use = 0;
	g_heap.total_occupied -= header->node_size;
	header->node_size = subtract_addr(next, header + 1);
	next->prev_in_use = 0;
	next->prev_size = header->node_size;
}

int main() {
	void *ptr;

	for (int i = 0; i < 100; i++)
		ptr = ft_malloc(65);
	show_alloc_mem();
	ft_free(ptr);
}
