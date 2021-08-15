#include "libft_malloc.h"

/*
**	Divides the empty block in two parts,
**	marks part 1 as in use, marks part 2's size
**	as (old size) - (new_size).
**	Returns 1 if a new header was created, 0 otherwise.
*/

int 			set_block_in_use(t_block *block, size_t sz)
{
	t_block		*mid;
	t_block		*old_end;

	mid = advance_aligned(block + 1, sz);
	old_end = get_next_block(block);
	if (old_end != mid)
	{
		*mid = (t_block){sz, MAGIC, 1,
				subtract_addr(old_end, mid + 1), MAGIC, 0};
		old_end->prev_size = mid->this_size;
		old_end->prev_in_use = 0;
	}
	else
	{
		mid->prev_size = sz,
		mid->prev_in_use = 1;
	}
	block->node_in_use = 1;
	block->this_size = sz;
	get_heap()->total_occupied += sz;
	return (old_end == mid);
}

/*
**	Finds a block of requested size sz and puts it in *mem.
**	If the search was successful, subtracts the block size
**	from the corresponding zone's meta variable.
**	Returns 1 on success, 0 on failure.
*/

static int 		find_suitable_block(t_zone *zone, size_t sz, t_block **mem)
{
	t_block		*block;

	block = (t_block *)(zone + 1);
	while (block < zone->end)
	{
		if (!block->node_in_use && block->this_size >= sz)
		{
			zone->leftover_mem -=
					(sz + sizeof(t_block) * set_block_in_use(block, sz));
			*mem = (block + 1);
			return (1);
		}
		block = get_next_block(block);
	}
	return (0);
}

void			*malloc(size_t sz)
{
	int const	zone_idx = (sz > MAX_TINY) + (sz > MAX_SMALL);
	t_zone		*tmp;
	t_block 	*mem;

	if (!get_heap() && !init_heap())
		return (NULL);
	lock_main();
	tmp = get_heap()->zones[zone_idx];
	while (tmp)
	{
		if (tmp->leftover_mem >= sz && find_suitable_block(tmp, sz, &mem))
		{
			unlock_main();
			return (mem);
		}
		tmp = tmp->next;
	}
	if (!zone_create(sz, zone_idx, &tmp))
		return (NULL);
	push_zone(get_heap()->zones + zone_idx, tmp);
	unlock_main();
	return (malloc(sz));
}
