#include "libft_malloc.h"

//static void		print_addr(void *addr)
//{
//	char *ptr = (char *)(get_heap() + 1);
//
//	str_copy(&ptr, "Addr passed to free     : ");
//	ft_puthex(&ptr, (unsigned long long)addr, 16);
//	*ptr++ = '\n';
//	write(1, get_heap() + 1, subtract_addr(ptr, get_heap() + 1));
//}

static t_zone 		*get_zone_by_addr(t_block *mem)
{
	int const			zone_idx = (mem->this_size > MAX_TINY) + (mem->this_size > MAX_SMALL);
	static const size_t zone_sizes[3] = { ZONE_TINY, ZONE_SMALL, 0 };
	off_t				offset;

	if (zone_idx != LARGE)
	{
		offset = (unsigned long long)mem % zone_sizes[zone_idx];
		return (advance_aligned(mem, -offset));
	}
	while (mem->prev_in_use == 0)
		mem = get_prev_block(mem);
	return ((t_zone *)mem - 1);
}

/*
**	finds the leftmost and the rightmost free blocks
**	and sets the span between them as available.
*/

static void 		try_defrag(t_block *header, t_block *next, t_zone *zone)
{
	if (zone->leftover_mem == zone->total_mem)
	{
		del_zone(zone);
		return ;
	}
	while (header->prev_in_use == 0)
		header = get_prev_block(header);
	while (next->node_in_use == 0)
		next = get_next_block(next);
	header->this_size = subtract_addr(next, header + 1);
	next->prev_in_use = 0;
	next->prev_size = header->this_size;

}

void 				free_internal(void *mem)
{
	t_block *header;
	t_block *next;
	t_zone	*zone;

	header = (t_block *)mem - 1;
	if (!check_magic(header) || !header->node_in_use)
		return;
	zone = get_zone_by_addr(header);
	get_heap()->total_occupied -= header->this_size;
	zone->leftover_mem += header->this_size;
	next = get_next_block(header);
	header->node_in_use = 0;
	header->this_size = subtract_addr(next, header + 1);
	next->prev_in_use = 0;
	next->prev_size = header->this_size;
	try_defrag(header, next, zone);
}

void				free(void *mem)
{
	if (!mem || !get_heap())
		return;
	lock_main();
	free_internal(mem);
	unlock_main();
}
