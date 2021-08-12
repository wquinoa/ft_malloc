#ifndef LIBMALLOC_H
#define LIBMALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>
// https://my.eng.utah.edu/~cs4400/malloc.pdf

// Overhead cost-related
# define OVERHEAD sizeof(t_block)
# define ALIGN(size, bound) (((size) + (bound - 1)) & ~(bound - 1))
# define SIZE_TO_CELLS(size) (((ALIGN(sz, 16) + 1) >> 4) + 1)

# define TINY 0
# define SMALL 1
# define LARGE 2
# define MAGIC 42

# define ZONE_TINY 4096
# define ZONE_SMALL 4096 * 4
# define ZONE_LARGE 0

# define MAX_TINY 64 - OVERHEAD
# define MAX_SMALL 4096 - OVERHEAD
# define FT_PROT_FLAGS (PROT_READ | PROT_WRITE)
# define FT_MAP_FLAGS (MAP_ANON | MAP_PRIVATE)

void			ft_free(void *ptr);
void			*ft_malloc(size_t size);
//void			*realloc(void *ptr, size_t size);

typedef struct      s_block {
	size_t 			prev_size: 56;
	size_t 			magic: 7;
	size_t 			prev_in_use: 1;
	size_t 			node_size: 56;
	size_t 			magic_2: 7;
	size_t			node_in_use: 1;
}				    t_block;

typedef struct		s_zone {
	struct s_zone	*next;
	struct s_block	*end;
	size_t			leftover_mem;
	size_t			padding;
}					t_zone;

typedef struct	s_heap {
	t_zone 		*zones[3];
	size_t		total_size;
	size_t 		total_occupied;
}				t_heap;

extern t_heap g_heap;

size_t 	align(size_t size, size_t bound);
size_t	substract_addr(const void *x, const void *y);
void 	*advance_aligned(const void *x, off_t offset);

void	show_alloc_mem();

#endif //LIBMALLOC_H
