
#include "libft_malloc.h"

static size_t	ft_pow(int n, size_t pow)
{
	size_t res;

	res = 1;
	while (pow--)
		res *= n;
	return (res);
}

static size_t	ft_nlen(size_t n, int base)
{
	int len = (n == 0);
	while (n != 0)
	{
		n /= base;
		len++;
	}
	return (len);
}

/*
**	Fills the buffer with a representation of n as a number of requested base.
**	Advances the buffer pointer.
*/

void			ft_puthex(char **buf, unsigned long long n, int base)
{
	char const *base_s = "0123456789abcdef";
	size_t div;
	char *dst;

	dst = *buf;
	div = ft_pow(base, ft_nlen(n, base) - 1);
	while (div)
	{
		*dst++ = base_s[n / div];
		n %= div;
		div /= base;
	}
	*buf = dst;
}

/*
**	Copies the source string into the buffer and advances the buffer ptr.
*/

void 			str_copy(char **buf, char const *src)
{
	char *dst;

	dst = *buf;
	while (*src)
		*dst++ = *src++;
	*buf = dst;
}

