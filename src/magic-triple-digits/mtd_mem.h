#ifndef MTD_MEM
#define MTD_MEM

#define At(arr, i) arr[(i)]

struct mtd_arr {
    int* buf;
    int s;
    int c;
};

struct mtd_arr mtd_alloc(int capacity);
void mtd_push(struct mtd_arr arr, int v);
void mtd_free(struct mtd_arr arr);

#endif
