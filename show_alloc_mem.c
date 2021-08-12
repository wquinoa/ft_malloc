
#include "libmalloc.h"
#include <stdio.h>
void 	iterate_zone(t_zone *zone, const char *fmt)
{
	t_block const *block = (t_block *)(zone + 1);

	while (zone)
	{
		printf(fmt, zone);
		while (block < zone->end)
		{
			if (block->node_in_use)
				printf("\t%p - %p : %zu\n", block + 1, advance_aligned(block + 1, block->node_size), block->node_size);
			block = advance_aligned(block + 1, block->node_size);
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

