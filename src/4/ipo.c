/* preorder обход */

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
struct tree_t {
    struct tree_t *left;
    struct tree_t *right;
    int data;
};
#endif

void print_pre(struct tree_t *top)
{
    if (top == NULL)
        return;
    printf("%d ", top->data);
    print_pre(top->left);
    print_pre(top->right);
}

int main()
{
    printf("\n");
}
