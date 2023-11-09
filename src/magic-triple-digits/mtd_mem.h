#ifndef MTD_MEM
#define MTD_MEM

#define At(arr, i) arr.buf[(i)]

typedef struct
{
    int *buf;
    /* size */
    int s;
    /* capacity */
    int c;
} mtd_arr;

mtd_arr mtd_alloc(const int capacity);
void mtd_push(mtd_arr *arr, const int v);
void mtd_free(mtd_arr arr);

#endif
