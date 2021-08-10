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
# define HDR_SIZE sizeof(t_node)
# define FTR_SIZE sizeof(t_node)
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

enum    e_nodeflags {
    m_zone_type = (1 << 1) - 1,
    f_free_bit = (1 << 2),
    m_magic = ~((1 << 3) - 1),
};

typedef struct      s_node {
	struct s_node	*next;
	size_t          reserved: 28;
    size_t   		is_free: 1;
    size_t          zone_type: 2;
}				    t_node;

typedef struct {
    struct s_node   *next;
    size_t          metadata;
}                   t_free_node;

typedef struct		s_zone {
	struct s_zone 	*next;
	struct s_node	*free_stack;
    size_t          reserved: 54;
    size_t          free_blocks: 8;
    size_t          type: 2;
}					t_zone;

static struct s_zone *g_zones[3];

#endif //LIBMALLOC_H
