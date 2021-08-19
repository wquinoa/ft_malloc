#include "libft_malloc.h"

static void 	ft_hex_char(char **buf, unsigned char *mem, size_t i)
{
	if (((i >> 4) << 4) == i)
	{
		ft_puthex(buf, (unsigned long long)(mem), 16);
		str_copy(buf, " ");
	}
	if (*mem < 16)
		ft_puthex(buf, 0, 16);
	ft_puthex(buf, *mem | 0ULL, 16);
	str_copy(buf, " ");
	if ((i & 15) == 15)
		str_copy(buf, "\n");
}

static void 	ft_hex_dump(t_block *block)
{
	unsigned char	*mem;
	char 			*buf;

	buf = (char *)(get_heap() + 1);
	mem = (unsigned char *)(block + 1);
	for (size_t i = 0; i < block->this_size; i++)
	{
		ft_hex_char(&buf, mem++, i);
		if (subtract_addr(buf, get_heap() + 1) >= 2048)
		{
			write(1, get_heap() + 1, subtract_addr(buf, get_heap() + 1));
			buf = (char *)(get_heap() + 1);
		}
	}
	str_copy(&buf, "\nblock size : ");
	ft_puthex(&buf, block->this_size, 10);
	str_copy(&buf, " bytes\n\n");
	write(1, get_heap() + 1, subtract_addr(buf, get_heap() + 1));
}

void 	show_alloc_mem_ex()
{
	if (!get_heap())
		return ;
	lock_main();
	iterate_zone(get_heap()->zones[TINY], "TINY : ", ft_hex_dump);
	iterate_zone(get_heap()->zones[SMALL], "SMALL : ", ft_hex_dump);
	iterate_zone(get_heap()->zones[LARGE], "LARGE : ", ft_hex_dump);
	unlock_main();
}