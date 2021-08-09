#ifndef LIBMALLOC_H
#define LIBMALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>
# include <stdatomic.h>
# include <stdalign.h>
// https://my.eng.utah.edu/~cs4400/malloc.pdf

# define BREAKPOINT { \
                      \
	read(0, 0, 1);                      \
};

# define TINY 0
# define SMALL 1
# define LARGE 2

// Overhead cost-related
# define HDR_SIZE sizeof(t_block_hdr)
# define FTR_SIZE sizeof(t_block_ftr)
# define OVERHEAD HDR_SIZE + FTR_SIZE

# define ALIGN(size, bound) (((size) + (bound - 1)) & ~(bound - 1))

# define TINY_ALLOC 16
# define SMALL_ALLOC 64
# define SMALL_ZONE_SIZE (OVERHEAD + SMALL_ALLOC) * 127 + sizeof(t_zone)
# define TINY_MAX_ZONES 255
# define SMALL_MAX_ZONES 127

# define FT_PROT_FLAGS (PROT_READ | PROT_WRITE)
# define FT_MAP_FLAGS (MAP_ANON | MAP_PRIVATE)

# define SMALL_MASK 0x40
# define TINY_MASK 0xF

void			ft_free(void *ptr);
void			*ft_malloc(size_t size);
//void			*realloc(void *ptr, size_t size);

typedef struct s_node {
	struct s_node	*next;
	size_t 			is_free;
	//char			*memory;
}				t_node;

typedef struct {
	size_t			size;
	size_t			allocated;
}					t_block_hdr;

typedef struct		s_zone {
	struct s_zone 	*next;
	struct s_node	*free_stack;
	// padding 8
	// padding 8
	//char			*memory;
}					t_zone;

typedef struct {
	size_t 			size;
	struct s_zone	*zone_start;
}					t_block_ftr;

static struct s_zone *g_zones[3];

#endif //LIBMALLOC_H
