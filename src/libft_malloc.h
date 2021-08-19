#ifndef LIBMALLOC_H
#define LIBMALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>
// https://my.eng.utah.edu/~cs4400/malloc.pdf

# define OVERHEAD sizeof(t_block)
# define ALIGNMENT 16

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
	struct s_zone	*prev;
	struct s_block	*end;
	size_t			leftover_mem: 32;
	size_t 			total_mem: 32;
}					t_zone;

typedef struct	s_heap {
	t_zone 		*zones[3];
	size_t		total_size;
	size_t 		total_occupied;
}				t_heap;

void			*malloc(size_t size);
void			free(void *ptr);
void			*realloc(void *ptr, size_t size);

void			*malloc_internal(size_t sz);
void			free_internal(void* ptr);

void			show_alloc_mem(void);
void			show_alloc_mem_ex(void);

t_heap 			*init_heap(void);
t_heap			*get_heap(void);
void 			lock_main(void);
void 			unlock_main(void);
int 			zone_create(size_t sz, size_t zone_idx, t_zone **mem);
void			push_zone(t_zone **victim, t_zone *new);
void			del_zone(t_zone *victim);
void			iterate_zone(t_zone *zone, const char *type, void (*f)(t_block *block));

size_t 			align(size_t size, size_t bound);
off_t 			subtract_addr(const void *x, const void *y);
void 			*advance_aligned(const void *x, off_t offset);
void 			*get_next_block(t_block const *x);
void 			*get_prev_block(t_block const *x);
int 			check_magic(t_block const *b);


void 			str_copy(char **buf, char const *src);
void			ft_puthex(char **buf, unsigned long long n, int base);

#endif //LIBMALLOC_H
