/*  корзинная сортировка
    Сформируем бакеты разделив максимальное число M на N частей */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

struct node_t {
    struct node_t *next;
    int data;
};

typedef struct node_t Node;

void dump_arr(int *arr, const int s);
int *alloc_arr(const int s);
int *alloc_arr_io(const int s);
int read_int();

int find_max(const int *const arr, const int s)
{
    int max = INT_MIN;
    for (int i = 0; i < s; ++i) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

Node *alloc_node(int data)
{
    Node *buf = calloc(1, sizeof(Node));
    if (buf == NULL) {
        abort();
    }
    buf->data = data;
    return buf;
}

Node **alloc_node_arr(const int s)
{
    Node **node_arr = calloc(s, sizeof(Node **));
    if (node_arr == NULL) {
        abort();
    }
    return node_arr;
}

void free_list(Node * top)
{
    while (top != NULL) {
        Node *tmp = top;
        top = top->next;
        free(tmp);
    }
}

void free_node_arr(Node ** node_arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        free_list(node_arr[i]);
    }
    free(node_arr);
}

void dump_node_arr(Node ** node_arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        Node *top = node_arr[i];
        while (top != NULL) {
            printf("%d ", top->data);
            top = top->next;
        }
        printf("0 ");
    }
    printf("\n");
}

void insert_node(Node ** l, int data)
{
    Node *prev = NULL;
    Node *cur = *l;
    Node *inserted = alloc_node(data);

    if (*l == NULL) {
        *l = inserted;
        return;
    }

    while (cur != NULL) {
        if (data < cur->data) {
            inserted->next = cur;
            if (prev != NULL) {
                prev->next = inserted;
            } else {
                inserted->next = cur;
                *l = inserted;
            }
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    prev->next = inserted;
}

int main()
{
    int s, max, buck_s;
    int *arr;
    Node **node_arr;
    s = read_int();

    if (s == 0) {
        return 0;
    }

    arr = alloc_arr_io(s);
    node_arr = alloc_node_arr(s);
    max = find_max(arr, s);
    buck_s = max / s;
    for (int i = 0; i < s; ++i) {
        int b = arr[i] / buck_s;
        if (b >= s) {
            b = s - 1;
        }

        assert(b < s);
        insert_node(node_arr + b, arr[i]);
    }

    dump_node_arr(node_arr, s);

    free_node_arr(node_arr, s);
    free(arr);
}

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int *alloc_arr(const int s)
{
    int *buf;
    buf = calloc(sizeof(int), s);
    if (buf == NULL) {
        abort();
    }
    return buf;
}

int *alloc_arr_io(const int s)
{
    int *arr = alloc_arr(s);
    for (int i = 0; i < s; ++i) {
        arr[i] = read_int();
    }
    return arr;
}

int read_int()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}
