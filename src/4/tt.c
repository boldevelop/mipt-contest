/*  tree from traverses
    Принимает со stdin количество вершин,
    а потом preorder и inorder обходы дерева
    Печатает postorder
*/

#include <stdio.h>
#include <stdlib.h>

struct tree_t {
    struct tree_t *left;
    struct tree_t *right;
    int data;
};
typedef struct tree_t Tree;

int read_int();
int *alloc_arr(const int s);
Tree *alloc_node(int d);
void free_tree(Tree * root);
void read_arr(int *arr, const int s);
void print_post(Tree * tree);

int inorder_pos(int data, int l, int r, int *inorder)
{
    for (int i = l; i < r; ++i)
        if (data == inorder[i])
            return i;
    return r;
}

Tree *build(int inl, int inr, int *inorder, int prl, int prr,
            int *preorder, int (*gpos)(int d, int l, int r, int *inord))
{
    Tree *node = NULL;
    int data, pos, ls, rs, pre_pos;
    if (inl >= inr || prl >= prr)
        return NULL;
    data = preorder[prl];
    pos = gpos(data, inl, inr, inorder);
    ls = pos - inl;
    rs = inr - (pos + 1);
    pre_pos = prl + 1;

    node = alloc_node(data);
    node->left =
        build(inl, pos, inorder, pre_pos, pre_pos + ls, preorder, gpos);
    node->right =
        build(pos + 1, inr, inorder, pre_pos + ls, pre_pos + ls + rs,
              preorder, gpos);
    return node;
}

int main()
{
    int s;
    int *inorder, *preorder;
    Tree *tree;
    s = read_int();
    if (s == 0) {
        return 0;
    }
    preorder = alloc_arr(s);
    inorder = alloc_arr(s);
    read_arr(preorder, s);
    read_arr(inorder, s);

    tree = build(0, s, inorder, 0, s, preorder, inorder_pos);

    print_post(tree);
    printf("\n");

    free_tree(tree);
    free(preorder);
    free(inorder);
}

int read_int()
{
    int buf;
    if (scanf("%d", &buf) != 1)
        abort();
    return buf;
}

int *alloc_arr(const int s)
{
    int *buf = calloc(s, sizeof(int));
    if (!buf)
        abort();
    return buf;
}

void read_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i)
        arr[i] = read_int();
}

Tree *alloc_node(int d)
{
    Tree *n = calloc(1, sizeof(Tree));
    if (!n)
        abort();
    n->data = d;
    return n;
}

void free_tree(Tree * root)
{
    if (!root)
        return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void print_post(Tree * tree)
{
    if (!tree)
        return;
    print_post(tree->left);
    print_post(tree->right);
    printf("%d ", tree->data);
}
