#ifndef MTD_MEM
#define MTD_MEM

#define At(arr, i) arr.buf[(i)]

typedef struct {
    int* buf;
    /* size */
    int s;
    /* capacity */
    int c;
} mtd_arr;

mtd_arr mtd_alloc(int capacity);
void mtd_push(mtd_arr* arr, int v);
void mtd_free(mtd_arr arr);

#ifdef RESEARCH
typedef struct {
    int** buf;
    /* size */
    int s;
    /* capacity */
    int c;
} mtd_arr_ptr;

mtd_arr_ptr mtd_alloc_ptr(int capacity);
void mtd_push_ptr(mtd_arr_ptr * arr, mtd_arr val);
void mtd_free_ptr(mtd_arr_ptr arr);
void mtd_clear_ptr(mtd_arr_ptr* arr);
#endif


#endif
