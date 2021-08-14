
#include "libmalloc.h"
//#include <stdio.h>


static void 	ft_hex_dump(t_block const *block)
{
	unsigned char	*mem;
	int		n_iterations;

	n_iterations = block->this_size;
	mem = (unsigned char *)(block + 1);
//	for (int i = 0; i < n_iterations; i++)
//	{
//		if (((i >> 4) << 4) == i)
//			printf("%p ", mem);
//		printf("%02x ", (unsigned char)*mem);
//		if ((i & 15) == 15)
//			printf("\n");
//		mem++;
//	}
//	printf("\n");
}

static void 	iterate_zone(t_zone *zone, const char *fmt)
{
	t_block *block;

	(void)fmt;
	while (zone)
	{
//		printf(fmt, zone);
		block = (t_block *)(zone + 1);
		while (block < zone->end)
		{
			if (block->node_in_use)
				ft_hex_dump(block);
			block = get_next_block(block);
		}
		zone = zone->next;
	}
}

void 	show_alloc_mem_ex()
{
	iterate_zone(g_heap->zones[TINY], "TINY : %p\n");
	iterate_zone(g_heap->zones[SMALL], "SMALL : %p\n");
	iterate_zone(g_heap->zones[LARGE], "LARGE : %p\n");
}