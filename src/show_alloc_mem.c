
#include "libft_malloc.h"

void 			print_normal(t_block *block)
{
	char	*ptr;

	ptr = (char *)(get_heap() + 1);
	ft_puthex(&ptr, (unsigned long long)(block + 1), 16);
	str_copy(&ptr, " - ");
	ft_puthex(&ptr, (unsigned long long)(get_next_block(block)), 16);
	str_copy(&ptr, " : ");
	ft_puthex(&ptr, (unsigned long long)(block->this_size), 10);
	str_copy(&ptr, " bytes\n");
	write(1, (char *)(get_heap() + 1), subtract_addr(ptr, get_heap() + 1));
}

void			iterate_zone(t_zone *zone,
					const char *type, void (*f)(t_block *block))
{
	char	*ptr;
	t_block *block;

	while (zone)
	{
		ptr = (char *)(get_heap() + 1);
		str_copy(&ptr, type);
		ft_puthex(&ptr, (unsigned long long)zone, 16);
		str_copy(&ptr, "\n");
		write(1, (char *)(get_heap() + 1), subtract_addr(ptr, get_heap() + 1));
		block = (t_block *)(zone + 1);
		while (block < zone->end - 1)
		{
			if (block->node_in_use)
				f(block);
			block = get_next_block(block);
		}
		zone = zone->next;
	}
}

void	show_alloc_mem()
{
	char	*ptr;

	if (!get_heap())
		return;
	lock_main();
	iterate_zone(get_heap()->zones[TINY], "TINY : ", print_normal);
	iterate_zone(get_heap()->zones[SMALL], "SMALL : ", print_normal);
	iterate_zone(get_heap()->zones[LARGE], "LARGE : ", print_normal);
	ptr = (char *)(get_heap() + 1);
	str_copy(&ptr, "Total : ");
	ft_puthex(&ptr, get_heap()->total_occupied, 10);
	str_copy(&ptr, " bytes\n");
	write(1, (char *)(get_heap() + 1), subtract_addr(ptr, get_heap() + 1));
	unlock_main();
}
