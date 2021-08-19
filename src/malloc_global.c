
#include "libft_malloc.h"

static t_heap			*g_heap;
static pthread_mutex_t	g_mallock;

t_heap __attribute__ ((visibility ("hidden")))	*get_heap(void)
{
	return (g_heap);
}

t_heap __attribute__ ((visibility ("hidden")))	*init_heap(void)
{
	if (g_heap)
		return (g_heap);
	g_heap = mmap(NULL, getpagesize(), FT_PROT_FLAGS, FT_MAP_FLAGS, -1, 0);
	if (g_heap == MAP_FAILED)
		return (NULL);
	*g_heap = (t_heap){ {0}, 0, 0};
	pthread_mutex_init(&g_mallock, NULL);
	return (g_heap);
}

void __attribute__ ((visibility ("hidden")))	lock_main(void)
{
	pthread_mutex_lock(&g_mallock);
}

void __attribute__ ((visibility ("hidden")))	unlock_main(void)
{
	pthread_mutex_unlock(&g_mallock);
}