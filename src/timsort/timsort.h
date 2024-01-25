#include <stdint.h>

typedef int (*Comparator)(const void *, const void *);
void timsort(void *const pbase, size_t total_elems, size_t size,
             Comparator cmp);
void galop_merge(unsigned char *data, int ls, int rs, int es,
                 Comparator cmp, unsigned char *tmp,
                 unsigned char *tmp_swap);
