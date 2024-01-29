/*  натуральный порядок
    Считывает поисковое дерево
    из файла как количество вершин + последовательность данных
    Выводит preorder обход дерева */

#include <stdio.h>
#include <stdlib.h>

struct tree_t {
    struct tree_t *left;
    struct tree_t *right;
    int data;
};
typedef struct tree_t Tree;

int read_int()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

Tree *alloc_tree(int data)
{
    Tree *buf = calloc(1, sizeof(Tree));
    if (!buf) {
        abort();
    }
    buf->data = data;
    return buf;
}

Tree *tr_add(Tree * tr, int data)
{
    if (tr == NULL) {
        return alloc_tree(data);
    }
    if (data < tr->data) {
        tr->left = tr_add(tr->left, data);
    } else {
        tr->right = tr_add(tr->right, data);
    }

    return tr;
}

void tr_preorder(Tree * tr, void (*visit)(int data))
{
    if (tr == NULL) {
        return;
    }
    visit(tr->data);
    tr_preorder(tr->left, visit);
    tr_preorder(tr->right, visit);
}

void tr_free(Tree * tr)
{
    if (tr == NULL) {
        return;
    }
    tr_free(tr->left);
    tr_free(tr->right);
    free(tr);
}

void print_visiter(int data)
{
    printf("%d ", data);
}

int main()
{
    int s;
    Tree *tr = NULL;
    s = read_int();
    if (s == 0) {
        return 0;
    }
    for (int i = 0; i < s; ++i) {
        int tmp = read_int();
        tr = tr_add(tr, tmp);
    }
    tr_preorder(tr, print_visiter);
    printf("\n");
    tr_free(tr);
}
