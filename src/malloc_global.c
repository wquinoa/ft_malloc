
#include "libft_malloc.h"

static t_heap			*g_heap;
static pthread_mutex_t	g_mallock;

t_heap __attribute__ ((visibility ("hidden")))	*get_heap(void)
{
	return (g_heap);
}

t_heap __attribute__ ((visibility ("hidden")))	*init_heap(void)
{
	g_heap = mmap(NULL, getpagesize(), FT_PROT_FLAGS, FT_MAP_FLAGS, -1, 0);
	if (!g_heap)
		return (NULL);
	private_lock_init();
	g_heap->zones[TINY] = NULL;
	g_heap->zones[SMALL] = NULL;
	g_heap->zones[LARGE] = NULL;
	g_heap->total_occupied = 0;
	g_heap->total_size = 0;
	return (g_heap);
}

void __attribute__ ((visibility ("hidden")))	private_lock_init(void)
{
	pthread_mutex_init(&g_mallock, NULL);
}

void __attribute__ ((visibility ("hidden")))	lock_main(void)
{
	pthread_mutex_lock(&g_mallock);
}

void __attribute__ ((visibility ("hidden")))	unlock_main(void)
{
	pthread_mutex_unlock(&g_mallock);
}