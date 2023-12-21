#include "mem.h"

#include <stdlib.h>

ull *alloc_arr_ull(const int s)
{
    ull *buf;
    buf = calloc(sizeof(ull), s);
    if (buf == NULL) {
        abort();
    }
    return buf;
}
