#ifndef LIBMALLOC_H
#define LIBMALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>
// https://my.eng.utah.edu/~cs4400/malloc.pdf

# define OVERHEAD sizeof(t_block)
# define ALIGNMENT 8

# define TINY 0
# define SMALL 1
# define LARGE 2
# define MAGIC 42

# define ZONE_TINY 4096
# define ZONE_SMALL 4096 * 4
# define ZONE_LARGE 0

# define MAX_TINY 64 - OVERHEAD
# define MAX_SMALL 128 - OVERHEAD
# define FT_PROT_FLAGS (PROT_READ | PROT_WRITE)
# define FT_MAP_FLAGS (MAP_ANON | MAP_PRIVATE)

typedef struct      s_block {
	size_t 			prev_size: 56;
	size_t 			magic: 7;
	size_t 			prev_in_use: 1;
	size_t 			this_size: 56;
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

extern	t_heap			*g_heap;
extern	pthread_mutex_t	g_mallock;

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);

size_t 	align(size_t size, size_t bound);
off_t 	subtract_addr(const void *x, const void *y);
void 	*advance_aligned(const void *x, off_t offset);
void 	*get_next_block(t_block const *x);

int 	zone_create(size_t sz, size_t zone_idx, t_zone **mem);
void	push_zone(t_zone **victim, t_zone *new);

int 	str_copy(char *buf, char const *src);
int		ft_puthex(char *buf, unsigned long long n, int base);
void	show_alloc_mem();
void	show_alloc_mem_ex();

#endif //LIBMALLOC_H
