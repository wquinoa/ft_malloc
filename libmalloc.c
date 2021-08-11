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
	printf("block spans %zu bytes\n", substract_addr(last, first + 1));
	printf("should span: %zu\n", 4096 - OVERHEAD - sizeof(t_zone));
}

t_zone 	*zone_init(t_zone *new, size_t sz)
{
	if (!new)
		return (NULL);
	new->next = NULL;
	new->end = advance_addr(new, sz);
	new->leftover_mem = sz - OVERHEAD - sizeof(t_zone);
	set_sentinel_blocks((t_block *)(new + 1), new->end, new->leftover_mem);
	return (new);
}

t_zone 	*zone_create(size_t sz, size_t zone_idx)
{
	t_zone	*new;
	static const size_t zone_sizes[3] = { ZONE_TINY, ZONE_SMALL, ZONE_LARGE };
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

	old_end = block + to_cell_size(sz) + 1;
	old_end->prev_size = mid->node_size;
	old_end->prev_size = 1;

	mid = block + 1 + to_cell_size(sz);
	mid->prev_in_use = 1;
	mid->prev_size = sz;
	mid->node_size = block->node_size - sizeof(t_block) - sz;

	block->node_in_use = 1;
	block->node_size = sz;
}

void 	*find_suitable_block(size_t sz, int zone_idx)
{
	t_zone	*tmp;
	t_block	*block;

	tmp = g_heap.zones[zone_idx];
	while (tmp)
	{
		block = (t_block *)(tmp + 1);
		while (block < tmp->end)
		{
			if (!block->node_in_use && block->node_size >= sz + sizeof(t_block))
			{
				set_block_in_use(block, sz); // sega
				return (block);
			}
			block += to_cell_size(block->node_size) + 1;
		}
		tmp = tmp->next;
	}
	tmp = zone_create(sz, zone_idx);
	if (!tmp)
		return (NULL);
	add_zone(&g_heap.zones[zone_idx], tmp);
	return (find_suitable_block(sz, zone_idx));
}

void	*ft_malloc(size_t sz)
{
	find_suitable_block(sz, (sz > MAX_TINY) + (sz > MAX_SMALL));
	return NULL;
}

#include <limits.h>
int main() {
	ft_malloc(1);
}
