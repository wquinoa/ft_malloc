//
// Created by user on 12.08.2021.
//
#include "libmalloc.h"

void	push_zone(t_zone **victim, t_zone *new)
{
	t_zone *tmp;

	if (!*victim)
	{
		*victim = new;
		return ;
	}
	tmp = *victim;
	*victim = new;
	new->next = tmp;
}

static inline void 	set_sentinel_blocks(t_block *first, t_block* last, size_t sz)
{
	*first = (t_block){0, MAGIC, 1, sz, MAGIC,0};
	*last = (t_block){sz, MAGIC, 0, 0, MAGIC, 0};
}

t_zone 	*zone_create(size_t sz, size_t zone_idx)
{
	t_zone					*new;
	static const size_t 	zone_sizes[3] = { ZONE_TINY, ZONE_SMALL, ZONE_LARGE };
	size_t 					effective_mem;

	if (zone_idx == LARGE)
		effective_mem = align(sz, getpagesize());
	else
		effective_mem = zone_sizes[zone_idx];
	new = mmap(NULL, effective_mem, FT_PROT_FLAGS, FT_MAP_FLAGS, -1, 0);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->end = advance_aligned(new, effective_mem);
	new->leftover_mem = effective_mem - OVERHEAD * 2 - sizeof(t_zone);
	g_heap.total_size += new->leftover_mem;
	set_sentinel_blocks((t_block *)(new + 1), new->end - 1, new->leftover_mem);
	return (new);
}
