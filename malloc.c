#include "libmalloc.h"

void 			set_block_in_use(t_block *block, size_t sz)
{
	t_block		*mid;
	t_block		*old_end;

	mid = advance_aligned(block + 1, sz);
	old_end = get_next_block(block);
	*mid = (t_block){sz, MAGIC, 1, subtract_addr(old_end, mid + 1), MAGIC, 0 };
	g_heap.total_occupied += sz;

	old_end->prev_size = mid->node_size;
	old_end->prev_in_use = 0;

	block->node_in_use = 1;
	block->node_size = sz;
}

static void 	*find_suitable_block(t_zone *tmp, size_t sz)
{
	t_block		*block;

	block = (t_block *)(tmp + 1);
	while (block < tmp->end)
	{
		if (!block->node_in_use && block->node_size >= sz)
		{
			set_block_in_use(block, sz);
			tmp->leftover_mem -= (sz + sizeof(t_block)); // @todo this is prone to errors
			return (block + 1);
		}
		block = get_next_block(block);
	}
	return (NULL);
}

void			*ft_malloc(size_t sz)
{
	int const	zone_idx = (sz > MAX_TINY) + (sz > MAX_SMALL);
	t_zone		*tmp;
	t_block 	*mem;

	tmp = g_heap.zones[zone_idx];
	while (tmp)
	{
		if (tmp->leftover_mem >= sz)
		{
			mem = find_suitable_block(tmp, sz);
			if (!mem)
				continue ;
			return (mem);
		}
		tmp = tmp->next;
	}
	tmp = zone_create(sz, zone_idx);
	if (!tmp)
		return (NULL);
	push_zone(g_heap.zones + zone_idx, tmp);
	return (ft_malloc(sz));
}
