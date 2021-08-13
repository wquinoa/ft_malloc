
#include "libmalloc.h"
#include <stdio.h>
void 	iterate_zone(t_zone *zone, const char *fmt)
{
	t_block *block;

	while (zone)
	{
		printf(fmt, zone);
		block = (t_block *)(zone + 1);
		while (block < zone->end)
		{
			if (block->node_in_use)
				printf("\t%p - %p : %zu\n", block + 1, get_next_block(block), block->this_size);
			block = get_next_block(block);
		}
		zone = zone->next;
	}
}

void	show_alloc_mem()
{
	iterate_zone(g_heap.zones[TINY], "TINY : %p\n");
	iterate_zone(g_heap.zones[SMALL], "SMALL : %p\n");
	iterate_zone(g_heap.zones[LARGE], "LARGE : %p\n");
	printf("total: %zu bytes\n", g_heap.total_occupied);
}

