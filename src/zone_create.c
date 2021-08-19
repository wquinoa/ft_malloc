#include "libft_malloc.h"

void __attribute__ ((visibility ("hidden")))	push_zone(t_zone **victim, t_zone *new)
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
	if (new->next)
		new->next->prev = new;
}

void __attribute__ ((visibility ("hidden")))	del_zone(t_zone *victim)
{
	size_t const	unmap_sz = align(victim->total_mem, getpagesize());
	int 			i;

	if ((victim)->prev)
		(victim)->prev->next = (victim)->next;
	if ((victim)->next)
		(victim)->next->prev = (victim)->prev;
	munmap(victim, unmap_sz);
	i = 0;
	while (i < 3)
	{
		if (get_heap()->zones[i] == victim)
			get_heap()->zones[i] = NULL;
		i++;
	}
}

/*
**	[zone][prev=0;this=sz]_______free mem:sz_______[prev=sz;this=0]
*/

static inline void 	set_sentinel_blocks(t_block *first, t_block* last, size_t sz)
{
	*first = (t_block){0, MAGIC, 1, sz, MAGIC,0};
	*last = (t_block){sz, MAGIC, 0, 0, MAGIC, 1};
}

/*
**	Create a new zone of standard size based on 2nd parameter.
**	If the requested size is not TINY_ZONE or SMALL_ZONE, creates a large zone,
**	aligned to getpagesize().
**	Next, if the mmap call is successful, initialize the beginning and the end of zone.
**
**	Returns 0 if mmap() fails, 1 on success.
*/

int __attribute__ ((visibility ("hidden")))	zone_create(size_t sz, size_t zone_idx, t_zone **mem)
{
	t_zone					*new;
	static const size_t 	zone_sizes[3] = { ZONE_TINY, ZONE_SMALL, ZONE_LARGE };
	size_t 					effective_mem;

	if (zone_idx == LARGE)
		effective_mem = align(sz + sizeof(t_zone) + OVERHEAD * 2, getpagesize());
	else
		effective_mem = zone_sizes[zone_idx];
	new = mmap(NULL, effective_mem, FT_PROT_FLAGS, FT_MAP_FLAGS, -1, 0);
	*mem = new;
	if (new == MAP_FAILED)
		return (0);
	new->prev = NULL;
	new->end = advance_aligned(new, effective_mem);
	new->leftover_mem = effective_mem - OVERHEAD * 2 - sizeof(t_zone);
	new->total_mem = new->leftover_mem;
	get_heap()->total_size += new->leftover_mem;
	set_sentinel_blocks((t_block *)(new + 1), new->end - 1, new->leftover_mem);
	return (1);
}
