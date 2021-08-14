
#include "libmalloc.h"

size_t ft_pow(int n, size_t pow)
{
	size_t res;

	res = 1;
	while (pow--)
		res *= n;
	return res;
}

size_t ft_nlen(size_t n, int base)
{
	int len = (n == 0);
	while (n != 0)
	{
		n /= base;
		len++;
	}
	return len;
}

size_t ft_puthex(char *buf, unsigned long long n, int base)
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

static void 	iterate_zone(t_zone *zone, const char *fmt)
{
	t_block *block;
	char	*ptr;

	while (zone)
	{
		ptr = (char *)(g_heap + 1);
		ptr += str_copy(ptr, fmt);
		ptr += ft_puthex(ptr, (unsigned long long)zone, 16);
		ptr += str_copy(ptr, "\n");
		block = (t_block *)(zone + 1);
		while (block < zone->end)
		{
			if (block->node_in_use) {
				ptr += ft_puthex(ptr, (unsigned long long)(block + 1), 16);
				ptr += str_copy(ptr, " - ");
				ptr += ft_puthex(ptr, (unsigned long long)(get_next_block(block)), 16);
				ptr += str_copy(ptr, " : ");
				ptr += ft_puthex(ptr, (unsigned long long)(block->this_size), 10);
				ptr += str_copy(ptr, " bytes\n");
			}
			block = get_next_block(block);
		}
		write(1, (char *)(g_heap + 1), subtract_addr(ptr, g_heap + 1));
		zone = zone->next;
	}
}

void	show_alloc_mem()
{
	if (!g_heap)
		return;
	iterate_zone(g_heap->zones[TINY], "TINY : ");
	iterate_zone(g_heap->zones[SMALL], "SMALL : ");
	iterate_zone(g_heap->zones[LARGE], "LARGE : ");
	//printf("total: %zu bytes\n", g_heap.total_occupied);
}

