#include "libmalloc.h"
#include <stdlib.h>
#include <stdio.h>

t_heap g_heap;

void 	assert(int x) {
	if (x)
		abort();
}

void 	set_sentinel_blocks(t_block *first, t_block* last, size_t sz)
{
	*first = (t_block){0, MAGIC, 1, sz, MAGIC,0};
	*last = (t_block){sz, MAGIC, 0, 0, MAGIC, 0};
}

t_zone 	*zone_create(size_t sz, size_t zone_idx)
{
	t_zone	*new;
	static const size_t 	zone_sizes[3] = { ZONE_TINY, ZONE_SMALL, ZONE_LARGE };
	size_t 	effective_mem;

	if (zone_idx == LARGE)
		effective_mem = align(sz, getpagesize());
	else
		effective_mem = zone_sizes[zone_idx];
	printf("eff mem: %zu\n", effective_mem);
	new = mmap(NULL, effective_mem, FT_PROT_FLAGS, FT_MAP_FLAGS, -1, 0);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->end = advance_aligned(new, effective_mem);
	new->leftover_mem = effective_mem - OVERHEAD * 2 - sizeof(t_zone);
	g_heap.total_size += new->leftover_mem;
	set_sentinel_blocks((t_block *)(new + 1), new->end - 1, new->leftover_mem);
	return (new);
}

void	push_zone(t_zone **victim, t_zone *new)
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
}

void 	set_block_in_use(t_block *block, size_t sz)
{
	t_block *mid;
	t_block *old_end;

//	printf("block size %zu\n", block->node_size);
	mid = advance_aligned(block + 1, sz);
	old_end = advance_aligned(block + 1, block->node_size);
//	printf("old p_size %zu, old n_size %zu\n", old_end->prev_size, old_end->node_size);
	mid->prev_in_use = 1;
	mid->prev_size = sz;
	mid->node_size = substract_addr(old_end, mid + 1);
	mid->node_in_use = 0;
	mid->magic = MAGIC;
	mid->magic_2 = MAGIC;
	g_heap.total_occupied += sz;

	old_end->prev_size = mid->node_size;
	old_end->prev_in_use = 0;

	block->node_in_use = 1;
	block->node_size = sz;
}

void 	*find_suitable_block(t_zone *tmp, size_t sz)
{
	t_block	*block;

	block = (t_block *)(tmp + 1);
	while (block < tmp->end)
	{
		if (!block->node_in_use && block->node_size >= sz)
		{
			set_block_in_use(block, sz);
//			printf("%zu\n", substract_addr(block, tmp));
			tmp->leftover_mem -= (sz + sizeof(t_block));
			return (block + 1);
		}
		block = advance_aligned(block, block->node_size + sizeof(t_block));
	}
	return (NULL);
}

void	*ft_malloc(size_t sz)
{
	int const	zone_idx = (sz > MAX_TINY) + (sz > MAX_SMALL);
	t_zone		*tmp;
	t_block 	*mem;

	tmp = g_heap.zones[zone_idx];
	while (tmp)
	{
		if (tmp->leftover_mem >= sz)
		{
			mem = find_suitable_block(tmp, sz);
			if (!mem)
				continue ;
			return mem;
		}
		tmp = tmp->next;
	}
	tmp = zone_create(sz, zone_idx);
	if (!tmp)
		return (NULL);
	push_zone(&(g_heap.zones[zone_idx]), tmp);
	return (ft_malloc(sz));
}

static inline int 	is_freeable(t_block *block)
{
	block -= 1;
	return (block->magic == block->magic_2 && !block->node_in_use);
}

void	ft_free(void *mem)
{
	t_block *header;
	t_block *next;

	header = advance_aligned(mem, -sizeof(t_block));
	if (!mem || is_freeable((t_block *)mem))
	{
//		printf("double free\n");
		return;
	}
	header->node_in_use = 0;
	g_heap.total_occupied -= header->node_size;
	header->node_size = align(header->node_size, 16);
	next = advance_aligned(header + 1, header->node_size);
	next->prev_in_use = 0;
}

int main() {
	void *ptr;
//	printf("%zu\n", SIZE_MAX >> 40);
//	printf("%zu\n", 16711568UL);
//	printf("%zu\n", 0x4a - 0x20);
	for (int i = 1; i <= 1000; ++i) {
		ptr = ft_malloc(1);
		if (i % 500 == 0) {
			printf("===============\n");
			printf("=== iter %d ===\n", i + 1);
			printf("===============\n");
			show_alloc_mem();
		}
		ft_free(ptr);
	}
}
