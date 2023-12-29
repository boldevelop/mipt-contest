/*  обобщённое слияние
    сортировка объектов разных размеров,
    последовательно расположенных в памяти */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
typedef int (*xcmp_t)(void *lhs, int lsz, void *rhs, int rsz);
int xcmp(void *lhs, __attribute__((unused))
         int lsz, void *rhs, __attribute__((unused))
         int rsz)
{
    return (*(int *) lhs) - (*(int *) rhs);
}
#endif

typedef unsigned char uchar;
typedef struct {
    void *d;
    int size;
} Meta;

void meta_swap(Meta * arr, int l, int r)
{
    Meta tmp = arr[l];
    arr[l] = arr[r];
    arr[r] = tmp;
}

void meta_merge(Meta * arr, int l, int m, int r, xcmp_t cmp)
{
    int size = r - l, l1 = l, l2 = m, out = 0;
    Meta *buf;
    if (size < 2) {
        return;
    }
    if (size == 2) {
        if (cmp(arr[l1].d, arr[l1].size, arr[l2].d, arr[l2].size) >= 0) {
            meta_swap(arr, l1, l2);
        }
        return;
    }

    buf = calloc(size, sizeof(Meta));
    if (!buf) {
        abort();
    }
    while (l1 < m || l2 < r) {
        if (l1 == m) {
            buf[out++] = arr[l2++];
            continue;
        }
        if (l2 == r) {
            buf[out++] = arr[l1++];
            continue;
        }
        if (cmp(arr[l1].d, arr[l1].size, arr[l2].d, arr[l2].size) < 0) {
            buf[out++] = arr[l1++];
        } else {
            buf[out++] = arr[l2++];
        }
    }
    memcpy(arr + l, buf, sizeof(Meta) * size);
    free(buf);
}

void meta_msort_impl(Meta * meta, int l, int r, xcmp_t cmp)
{
    int s = r - l;
    int m = l + s / 2;
    if (s < 2) {
        return;
    }
    meta_msort_impl(meta, l, m, cmp);
    meta_msort_impl(meta, m, r, cmp);
    meta_merge(meta, l, m, r, cmp);
}

Meta *alloc_meta(void *mem, int *sizes, int s, int *b_size)
{
    Meta *arr;
    int offset = 0;
    arr = calloc(s, sizeof(Meta));
    if (!arr) {
        abort();
    }
    for (int i = 0; i < s; ++i) {
        Meta m = {
            .d = (uchar *) mem + offset,
            .size = sizes[i]
        };
        arr[i] = m;
        offset += m.size;
    }
    *b_size = offset;
    return arr;
}

void xmsort(void *mem, int *sizes, int nelts, xcmp_t cmp)
{
    uchar *mem_buf;
    int bytes_size = 0;
    int offset = 0;
    Meta *meta = alloc_meta(mem, sizes, nelts, &bytes_size);

    meta_msort_impl(meta, 0, nelts, cmp);

    mem_buf = calloc(bytes_size, sizeof(uchar));
    if (!mem_buf) {
        abort();
    }
    for (int i = 0; i < nelts; ++i) {
        int s = meta[i].size;
        sizes[i] = s;
        memcpy(mem_buf + offset, meta[i].d, s);
        offset += s;
    }
    memcpy(mem, mem_buf, bytes_size);

    free(mem_buf);
    free(meta);
}


#ifdef DEBUG
typedef int (*comp_t)(void *lhs, void *rhs);

void swap_int(void *arr, int l, int r)
{
    int tmp = *((int *) arr + l);
    *((int *) arr + l) = *((int *) arr + r);
    *((int *) arr + r) = tmp;
}

void merge(void *arr, int l, int m, int r, comp_t cmp)
{
    int size = r - l, l1 = l, l2 = m, out = 0;
    int *buf;
    if (size < 2) {
        return;
    }
    if (size == 2) {
        if (cmp((int *) arr + l1, (int *) arr + l2) >= 0) {
            swap_int(arr, l1, l2);
        }
        return;
    }

    buf = calloc(size, sizeof(int));
    if (!buf) {
        abort();
    }
    while (l1 < m || l2 < r) {
        if (l1 == m) {
            buf[out++] = *((int *) arr + l2);
            l2++;
            continue;
        }
        if (l2 == r) {
            buf[out++] = *((int *) arr + l1);
            l1++;
            continue;
        }
        if (cmp((int *) arr + l1, (int *) arr + l2) < 0) {
            buf[out++] = *((int *) arr + l1);
            l1++;
        } else {
            buf[out++] = *((int *) arr + l2);
            l2++;
        }
    }
    memcpy((int *) arr + l, buf, sizeof(int) * size);
    free(buf);
}

void merge_sort_impl(void *mem, int l, int r, comp_t cmp)
{
    int s = r - l;
    int m = l + s / 2;
    if (s < 2) {
        return;
    }
    merge_sort_impl(mem, l, m, cmp);
    merge_sort_impl(mem, m, r, cmp);
    merge(mem, l, m, r, cmp);
}

void merge_sort(void *mem, int s, comp_t cmp)
{
    merge_sort_impl(mem, 0, s, cmp);
}

int int_cmp(void *lhs, void *rhs)
{
    int ilhs = *(int *) lhs;
    int irhs = *(int *) rhs;
    return ilhs - irhs;
}

void dump_arr(int *arr, int s);

void p_ints(void *mem, int bytes)
{
    for (int i = 0; i < bytes / (int) sizeof(int); ++i) {
        int c = *((int *) mem + i);
        printf("%d ", c);
    }
    printf("\n");
}

void *create_dsa(int *data, int *sizes, int s)
{
    int bytes = 0;
    void *mem = NULL;
    int offset = 0;
    for (int i = 0; i < s; ++i) {
        bytes += sizes[i];
    }
    mem = calloc(bytes, sizeof(uchar));
    if (!mem) {
        abort();
    }

    for (int i = 0; i < s; ++i) {
        int *b = (int *) ((uchar *) mem + offset);
        *b = data[i];
        offset += sizes[i];
    }
    return mem;
}

int main()
{
    int int_s = sizeof(int);
    {
        void *mem;
        int s = 5;
        int data[5] = { 7, 4, 3, 5, 1 };
        int sizes[5] = { 2, 3, 1, 4, 2 };
        int bytes = 0;
        for (int i = 0; i < s; ++i) {
            sizes[i] *= int_s;
            bytes += sizes[i];
        }
        mem = create_dsa(data, sizes, s);
        p_ints(mem, bytes);
        xmsort(mem, sizes, s, xcmp);
        p_ints(mem, bytes);
        printf("\n");
        free(mem);
    }
    {
        void *mem;
        int s = 10;
        int data[10] = { 8, 9, 9, 7, 1, 7, 9, 9, 1, 8 };
        int sizes[10] = { 2, 3, 1, 4, 2, 2, 3, 1, 4, 2 };
        int bytes = 0;
        for (int i = 0; i < s; ++i) {
            sizes[i] *= int_s;
            bytes += sizes[i];
        }
        mem = create_dsa(data, sizes, s);
        p_ints(mem, bytes);
        xmsort(mem, sizes, s, xcmp);
        p_ints(mem, bytes);
        printf("\n");
        free(mem);
    }
    {
        void *mem;
        int s = 10;
        int data[10] = { 8, 9, 9, 7, 1, 7, 9, 9, 1, 8 };
        int sizes[10] = { 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 };
        int bytes = 0;
        for (int i = 0; i < s; ++i) {
            sizes[i] *= int_s;
            bytes += sizes[i];
        }
        mem = create_dsa(data, sizes, s);
        p_ints(mem, bytes);
        xmsort(mem, sizes, s, xcmp);
        p_ints(mem, bytes);
        printf("\n");
        free(mem);
    }
    {
        int s = 5;
        int arr[5] = { 7, 4, 3, 5, 0 };
        merge_sort(arr, s, int_cmp);
        dump_arr(arr, s);
    }
    {
        int s = 6;
        int arr[6] = { 7, 4, 3, 5, 0, -5 };
        merge_sort(arr, s, int_cmp);
        dump_arr(arr, s);
    }
    {
        int s = 2;
        int arr[2] = { 7, 4 };
        merge_sort(arr, s, int_cmp);
        dump_arr(arr, s);
    }
    {
        int s = 1;
        int arr[1] = { 7 };
        merge_sort(arr, s, int_cmp);
        dump_arr(arr, s);
    }
}

int is_equal(int *l, int *r, int s)
{
    for (int i = 0; i < s; ++i)
        if (l[i] != r[i])
            return 0;
    return 1;
}

void dump_arr(int *arr, int s)
{
    for (int i = 0; i < s; ++i)
        printf("%d ", arr[i]);
    printf("\n");
}
#endif
