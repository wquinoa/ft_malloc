#include "libmalloc.h"
#include <stdlib.h>
#include <stdio.h>

void 	assert(int x) {
	if (x)
		abort();
}

void 	set_sentinel_blocks(t_block *first, t_block* end, size_t sz)
{
	t_block *last = end - 1;

	first->prev_size = 0;
	first->prev_in_use = 1;
	first->node_size = sz;
	first->node_in_use = 0;

	last->prev_size = sz;
	last->prev_in_use = 0;
	last->node_size = 0;
	last->node_in_use = 1;
}

t_zone 	*zone_init(t_zone *new, size_t sz)
{
	new->next = NULL;
	new->end = advance_aligned(new, sz);
	new->leftover_mem = sz - OVERHEAD - sizeof(t_zone);
	set_sentinel_blocks((t_block *)(new + 1), new->end, new->leftover_mem);
	return (new);
}

t_zone 	*zone_create(size_t sz, size_t zone_idx)
{
	t_zone	*new;
	static const size_t 	zone_sizes[3] = { ZONE_TINY, ZONE_SMALL, ZONE_LARGE };
	size_t 	effective_mem;

	if (zone_idx == LARGE)
		effective_mem = align(sz, getpagesize());
	else
		effective_mem = zone_sizes[zone_idx];
	new = mmap(NULL, effective_mem, FT_PROT_FLAGS, FT_MAP_FLAGS, -1, 0);
	return (zone_init(new, effective_mem));
}

void	add_zone(t_zone **victim, t_zone *new)
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

void 	set_block_in_use(t_block *block, size_t sz)
{
	t_block *mid;
	t_block *old_end;

	mid = advance_aligned(block, sz + sizeof(block));
	mid->prev_in_use = 1;
	mid->prev_size = sz;
	mid->node_size = block->node_size - sizeof(t_block) - sz;
	mid->node_in_use = 0;

	old_end = advance_aligned(block, block->node_size + sizeof(block));
	old_end->prev_size = mid->node_size;
	old_end->prev_in_use = 0;

	block->node_in_use = 1;
	block->node_size = sz;
}

void 	*find_suitable_block(t_block *block, size_t sz)
{
	t_zone	*tmp;

	tmp = advance_aligned(block, -sizeof(t_zone));
	while (block < tmp->end)
	{
		if (!block->node_in_use && block->node_size >= sz + sizeof(t_block))
		{
			set_block_in_use(block, sz);
			tmp->leftover_mem -= (sz + sizeof(t_block));
			return (block + 1);
		}
		block = advance_aligned(block, block->node_size + sizeof(t_block));
	}
	return (NULL);
}

void	*ft_malloc(size_t sz)
{
	int const	zone_idx = (sz > MAX_TINY) + (sz > MAX_SMALL);
	t_zone		*tmp;
	t_block		*mem;

	tmp = g_heap.zones[zone_idx];
	while (tmp)
	{
		if (tmp->leftover_mem >= sz)
		{
			mem = find_suitable_block((t_block *)(tmp + 1), sz);
			if (!mem)
				continue ;
			return mem;
		}
		tmp = tmp->next;
	}
	tmp = zone_create(sz, zone_idx);
	if (!tmp)
		return (NULL);
	add_zone(g_heap.zones + zone_idx, tmp);
	return (ft_malloc(sz));
}

int main() {
	ft_malloc(1);
}
