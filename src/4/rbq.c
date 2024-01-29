#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char uchar;

static int readint()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

typedef struct node_t Node;
void printf_tree(Node * r, int level);
void free_tree(Node * r);
void insert(Node ** root, int k);
typedef void (*Visitor)(int d);
void visit_range(Node * n, int l, int r, Visitor v);
static int c = 0;

void print_int(int d)
{
    c++;
    printf("%d ", d);
}

void count_visit( __attribute__((unused))
                 int d)
{
    c++;
}

int main()
{
#if 1
    int cc;                     /* city count */
    int rq;                     /* request count */
    Node *tree = NULL;

    cc = readint();
    for (int i = 0; i < cc; ++i) {
        int dist = readint();
        insert(&tree, dist);
    }
    rq = readint();
    for (int i = 0; i < rq; ++i) {
        int l, r;
        l = readint();
        r = readint();
        visit_range(tree, l, r, count_visit);
        printf("%d ", c);
        c = 0;
    }
    printf("\n");
#else
    Node *tree = NULL;
    for (int i = 1; i <= 12; ++i) {
        insert(&tree, i * 5);
    }
    printf_tree(tree, 0);
    visit_range(tree, 1, 3, print_int);
    printf("\n");
    visit_range(tree, 5, 21, print_int);
    printf("\n");
    visit_range(tree, 3, 100, print_int);
    printf("\n");

    free_tree(tree);
    return 0;
#endif
}

typedef struct node_t Node;
typedef struct node_t {
    int d;
    uchar h;
    Node *l;
    Node *r;
} Node;

typedef void (*Visitor)(int d);

void visit_range(Node * n, int l, int r, Visitor v)
{
    int d;
    if (!n)
        return;
    d = n->d;
    if (l <= d) {
        visit_range(n->l, l, r, v);
    }
    if (l <= d && r >= d) {
        v(d);
    }
    if (r >= d) {
        visit_range(n->r, l, r, v);
    }
}

Node *alloc_node(int v)
{
    Node *n;
    n = calloc(1, sizeof(Node));
    if (!n) {
        abort();
    }
    n->h = 1;
    n->d = v;
    return n;
}

uchar height_tree(Node * n)
{
    return n ? n->h : 0;
}

int bfactor_tree(Node * n)
{
    return height_tree(n->r) - height_tree(n->l);
}

void set_height_tree(Node * n)
{
    uchar hl, hr;
    assert(n);
    hl = height_tree(n->l);
    hr = height_tree(n->r);
    n->h = (hl > hr ? hl : hr) + 1;
}

Node *rotate_right(Node * n)
{
    Node *new_root = n->l;
    n->l = new_root->r;
    new_root->r = n;
    set_height_tree(n);
    set_height_tree(new_root);
    return new_root;
}

Node *rotate_left(Node * n)
{
    Node *new_root = n->r;
    n->r = new_root->l;
    new_root->l = n;
    set_height_tree(n);
    set_height_tree(new_root);
    return new_root;
}

Node *balance_tree(Node * n)
{
    set_height_tree(n);

    if (bfactor_tree(n) == 2) {
        if (bfactor_tree(n->r) < 0) {
            n->r = rotate_right(n->r);
        }
        return rotate_left(n);
    }

    if (bfactor_tree(n) == -2) {
        if (bfactor_tree(n->l) > 0) {
            n->l = rotate_left(n->l);
        }
        return rotate_right(n);
    }

    return n;
}

void printf_tree(Node * r, int level)
{
    for (int i = 0; i < level; ++i) {
        // if (i == 0)
        //     printf("*");
        // else
        printf(" ");
    }
    if (!r) {
        printf("*\n");
        return;
    }
    printf("%d\n", r->d);
    if (!r->l && !r->r)
        return;
    printf_tree(r->l, level + 2);
    printf_tree(r->r, level + 2);
}

void free_tree(Node * r)
{
    if (!r)
        return;
    free_tree(r->l);
    free_tree(r->r);
    free(r);
}

Node *add_val(Node * r, int k)
{
    if (!r) {
        return alloc_node(k);
    }
    if (k < r->d) {
        r->l = add_val(r->l, k);
    } else {
        r->r = add_val(r->r, k);
    }
    return balance_tree(r);
}

void insert(Node ** root, int k)
{
    *root = add_val(*root, k);
}
