#include "libmalloc.h"

void stack_push(t_node **head, t_node *new)
{

	if (!*head) {
		new->next = NULL;
		*head = new;
	}
	new->next = *head;
	*head = new;
}

t_node *stack_pop(t_node **head)
{
	t_node *tmp;

	if (!*head)
		return NULL;
	tmp = *head;
	*head = (*head)->next;
	tmp->next = NULL;
	return tmp;
}

/*
 * Zone anatomy:
 * [zone_head][block_hd][block_mem]
 * [16 bytes] [16 bytes][ Memory ][padding]
 * ( t_zone ) (zone + 1)(zone + 2)
 *
 * Memory anatomy:
 * [16 bytes][size bytes][16 bytes][size bytes]...[padding]
 */

void 	create_block_stack(t_node **zone, size_t size)
{
	t_node			*head;
	t_node			*current;
	size_t const	increment = sizeof(t_node) + size;
	char 			*mem;
	char const		*end = (char *)*zone + SMALL_ZONE_SIZE;

	head = *zone + 1;
	mem = (char *)(*zone + 2);
	while (mem + increment < end)
	{
		current = (t_node *)mem;
		current->is_free = 1;
		stack_push(&head, current);
		mem += increment;
	}
}

/*
 * Decides which type of zone is to be allocated and
 * requests a new zone of approx getpagesize() bytes
 * and splits the zone into TINY or SMALL blocks.
 * Returns NULL if there's no memory left.
 */

void 	*allocate_zone(size_t size)
{
	t_zone *new;
	size_t intermediate_size;
	size_t final_size;

	intermediate_size = size <= SMALL_ALLOC ? SMALL_ZONE_SIZE : size;
	final_size = ALIGN(intermediate_size, getpagesize());
	new = mmap(NULL, final_size, FT_PROT_FLAGS, FT_MAP_FLAGS, -1, 0);
	if (!new)
		return NULL;
	new->next = NULL;
	new->free_stack = NULL;
	return new;
}


/*
 * Tries to find a block of requested size.
 * if there aren't any available, requests a new page.
 * returns:
 *  - a pointer to a suitable memory block on success
 *	- NULL if no memory left @todo refactor
 */

t_node	*find_suitable_block(int type)
{
	t_zone *zone_curr = g_zones[type];
	t_zone *zone_prev;
	t_zone **to_allocate;
	t_node *block;

	zone_prev = NULL;
	while (zone_curr)
	{
		block = stack_pop(&zone_curr->free_stack);
		if (block)
			return block;
		zone_prev = zone_curr;
		zone_curr = zone_curr->next;
	}
	if (!zone_prev)
		to_allocate = &g_zones[type];
	else
		to_allocate = &zone_prev->next;
	*to_allocate = allocate_zone(SMALL_ALLOC); // @todo check alloc
	create_block_stack((t_node **)to_allocate, TINY_ALLOC + type * (SMALL_ALLOC - TINY_ALLOC));
	return find_suitable_block(type);
}

void	*handle_large_alloc(size_t size)
{

	(void)size;
	return NULL;
}

/*
 * finds a suitable block of small or tiny size,
 * sets the block as in use and returns a pointer
 * to memory.
 * Returns NULL if no memory left.
 */

void 	*handle_small_alloc(size_t size)
{
	t_node *block_raw;

	block_raw = find_suitable_block(size > TINY_ALLOC);
	if (block_raw == NULL)
		return NULL;
	block_raw->is_free = 0;
	return block_raw + 1;
}


void	*ft_malloc(size_t size)
{
	if (size == 0)
		return NULL;
	if (size > SMALL_ALLOC) // do large alloc
		return handle_large_alloc(size);
	return handle_small_alloc(size);
}

#include <stdio.h>
int main() {

	char *ptr = ft_malloc(13);

	printf("%s\n", "---");
	printf("sizeof(t_header) = %lu\n", sizeof(t_block_hdr));
	printf("sizeof(t_footer) = %lu\n", sizeof(t_block_ftr));
	printf("sizeof(t_zone) = %lu\n", sizeof(t_zone));
	printf("%s\n", "---");

	printf("%p\n", ptr);
}
