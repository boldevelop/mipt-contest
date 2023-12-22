/*  все чётные числа
    должны быть строго в начале списка
    (а нечётные в конце) в том же порядке,
    в каком они идут в файле */

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
struct node_t {
    struct node_t *next;
    int data;
};
#endif

struct node_t *alloc_lnode(int data)
{
    struct node_t *buf = calloc(1, sizeof(struct node_t));
    if (buf == NULL) {
        abort();
    }
    buf->data = data;
    return buf;
}

// считывает список из входного файла, так, чтобы все чётные числа были в начале
struct node_t *read_list(FILE * inp)
{
    int buf;
    int count;
    struct node_t *even_list = NULL;    /* четные */
    struct node_t *even_cur = NULL;
    struct node_t *odd_list = NULL;
    struct node_t *odd_cur = NULL;

    while ((count = fscanf(inp, "%d", &buf)) != EOF) {
        struct node_t *tmp = NULL;
        if (buf % 2 == 0) {
            if (even_list == NULL) {
                even_list = alloc_lnode(buf);
                even_cur = even_list;
            } else {
                tmp = alloc_lnode(buf);
                even_cur->next = tmp;
                even_cur = even_cur->next;
            }
            continue;
        }

        if (odd_list == NULL) {
            odd_list = alloc_lnode(buf);
            odd_cur = odd_list;
        } else {
            tmp = alloc_lnode(buf);
            odd_cur->next = tmp;
            odd_cur = odd_cur->next;
        }
    }
    if (ferror(inp)) {
        abort();
    }
    if (even_list == NULL && odd_list == NULL) {
        return NULL;
    }
    if (even_list == NULL) {
        return odd_list;
    }
    if (odd_list == NULL) {
        return even_list;
    }
    even_cur->next = odd_list;
    return even_list;
}

// удаляет список элемент за элементом
void delete_list(struct node_t *top)
{
    while (top != NULL) {
        struct node_t *tmp = top;
        top = top->next;
        free(tmp);
    }
}

#ifdef DEBUG
void dump_list(const struct node_t *top);
FILE *open_file(const char *path);

int main()
{
    struct node_t *l;
    FILE *inp;

    {
        inp = open_file("./in/al/1");
        l = read_list(inp);
        dump_list(l);
        fclose(inp);
        delete_list(l);
    }
    {
        inp = open_file("./in/al/2");
        l = read_list(inp);
        dump_list(l);
        fclose(inp);
        delete_list(l);
    }
    {
        inp = open_file("./in/al/3");
        l = read_list(inp);
        dump_list(l);
        fclose(inp);
        delete_list(l);
    }
}

FILE *open_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror(path);
        abort();
    }
    return fp;
}

void dump_list(const struct node_t *top)
{
    const struct node_t *tmp = top;
    if (tmp == NULL) {
        return;
    }
    while (tmp != NULL) {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}
#endif
