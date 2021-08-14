
#include "libmalloc.h"

static size_t ft_pow(int n, size_t pow)
{
	size_t res;

	res = 1;
	while (pow--)
		res *= n;
	return res;
}

static size_t ft_nlen(size_t n, int base)
{
	int len = (n == 0);
	while (n != 0)
	{
		n /= base;
		len++;
	}
	return len;
}

int ft_puthex(char *buf, unsigned long long n, int base)
{
	char const *base_s = "01234567890abcdef";
	size_t div;
	size_t i;
	size_t len = ft_nlen(n, base);

	i = 0;
	div = ft_pow(base, len - 1);
	while (i < len)
	{
		buf[i] = base_s[n / div];
		n %= div;
		div /= base;
		i++;
	}
	return len;
}

int 			str_copy(char *buf, char const *src)
{
	int i = 0;
	while (src[i])
	{
		buf[i] = src[i];
		i++;
	}
	return (i);
}

static void 	ft_hexchar(char **buf, unsigned char *mem, size_t i)
{
	if (((i >> 4) << 4) == i)
	{
		*buf += ft_puthex(*buf, (unsigned long long)(mem), 16);
		*buf += str_copy(*buf, " ");
	}
	if (*mem < 0x10)
		*buf += ft_puthex(*buf, 0, 16);
	*buf += ft_puthex(*buf, *mem, 16);
	*buf += str_copy(*buf, " ");
	if ((i & 15) == 15)
		*buf += str_copy(*buf, "\n");
}

static void 	ft_hex_dump(t_block *block)
{
	unsigned char	*mem;
	char 			*buf;

	buf = (char *)(g_heap + 1);
	mem = (unsigned char *)(block + 1);
	for (size_t i = 0; i < block->this_size; i++)
	{
		ft_hexchar(&buf, mem++, i);
		if (subtract_addr(buf, g_heap + 1) >= 2048)
		{
			write(1, g_heap + 1, subtract_addr(buf, g_heap + 1));
			buf = (char *)(g_heap + 1);
		}
	}
	buf += str_copy(buf, "\nblock size : ");
	buf += ft_puthex(buf, block->this_size, 10);
	buf += str_copy(buf, " bytes\n\n");
	write(1, g_heap + 1, subtract_addr(buf, g_heap + 1));
}

void 			print_normal(t_block *block)
{
	char	*ptr;

	ptr = (char *)(g_heap + 1);
	ptr += ft_puthex(ptr, (unsigned long long)(block + 1), 16);
	ptr += str_copy(ptr, " - ");
	ptr += ft_puthex(ptr, (unsigned long long)(get_next_block(block)), 16);
	ptr += str_copy(ptr, " : ");
	ptr += ft_puthex(ptr, (unsigned long long)(block->this_size), 10);
	ptr += str_copy(ptr, " bytes\n");
	write(1, (char *)(g_heap + 1), subtract_addr(ptr, g_heap + 1));
}

static void 	iterate_zone(t_zone *zone, const char *fmt, void (*f)(t_block *block))
{
	char	*ptr;
	t_block *block;

	while (zone)
	{
		ptr = (char *)(g_heap + 1);
		ptr += str_copy(ptr, fmt);
		ptr += ft_puthex(ptr, (unsigned long long)zone, 16);
		ptr += str_copy(ptr, "\n");
		write(1, (char *)(g_heap + 1), subtract_addr(ptr, g_heap + 1));
		block = (t_block *)(zone + 1);
		while (block < zone->end)
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
	if (!g_heap)
		return;
	iterate_zone(g_heap->zones[TINY], "TINY : ", print_normal);
	iterate_zone(g_heap->zones[SMALL], "SMALL : ", print_normal);
	iterate_zone(g_heap->zones[LARGE], "LARGE : ", print_normal);
	//printf("total: %zu bytes\n", g_heap.total_occupied);
}

void 	show_alloc_mem_ex()
{
	iterate_zone(g_heap->zones[TINY], "TINY : ", ft_hex_dump);
	iterate_zone(g_heap->zones[SMALL], "SMALL : ", ft_hex_dump);
	iterate_zone(g_heap->zones[LARGE], "LARGE : ", ft_hex_dump);
}