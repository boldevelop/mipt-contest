/* preorder обход */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef DEBUG
struct tree_t {
    struct tree_t *left;
    struct tree_t *right;
    int data;
};
#endif
typedef struct tree_t Tree;

struct stack_t {
    struct stack_t *next;
    struct tree_t *data;
};
typedef struct stack_t Stack;

void init_st(Stack ** stack)
{
    *stack = calloc(1, sizeof(Stack));
    if (*stack == NULL) {
        abort();
    }
}

Stack *alloc_st_node(Tree * data)
{
    Stack *stack = calloc(1, sizeof(Stack));
    if (stack == NULL) {
        abort();
    }
    stack->data = data;
    return stack;
}

void st_push(Stack ** stack, Tree * data)
{
    Stack *node;
    if (stack == NULL) {
        init_st(stack);
        (*stack)->data = data;
        return;
    }
    node = alloc_st_node(data);
    node->next = *stack;
    *stack = node;
}

void st_pop(Stack ** stack)
{
    Stack *tmp;
    assert(stack != NULL);
    tmp = (*stack)->next;
    free(*stack);
    *stack = tmp;
}

Tree *st_top(const Stack * const stack)
{
    if (stack == NULL) {
        return NULL;
    }
    return stack->data;
}

Tree *alloc_tree(int data)
{
    Tree *tr = calloc(1, sizeof(Tree));
    if (tr == NULL) {
        abort();
    }
    tr->data = data;
    return tr;
}

void print_pre(struct tree_t *top)
{
    Stack *stack = NULL;
    Tree *tmp;
    if (top == NULL)
        return;
    st_push(&stack, top);
    tmp = st_top(stack);

    while (tmp) {
        printf("%d ", tmp->data);
        st_pop(&stack);
        if (tmp->right) {
            st_push(&stack, tmp->right);
        }
        if (tmp->left) {
            st_push(&stack, tmp->left);
        }
        tmp = st_top(stack);
    }
}

#ifdef DEBUG

void print_r_pre(Tree * top)
{
    if (top == NULL)
        return;
    printf("%d ", top->data);
    print_r_pre(top->left);
    print_r_pre(top->right);
}

void print_r_in(Tree * top)
{
    if (top == NULL)
        return;
    print_r_in(top->left);
    printf("%d ", top->data);
    print_r_in(top->right);
}

void tr_free(Tree * top)
{
    if (top == NULL)
        return;
    tr_free(top->left);
    tr_free(top->right);
    free(top);
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

int main()
{
    Tree *tr = NULL;

    tr = tr_add(tr, 4);
    tr_add(tr, 3);
    tr_add(tr, 2);
    tr_add(tr, 1);
    tr_add(tr, 5);
    tr_add(tr, 6);
    tr_add(tr, 9);
    tr_add(tr, 8);
    tr_add(tr, 7);

    print_r_pre(tr);
    printf("\n");

    print_pre(tr);
    printf("\n");

    tr_free(tr);

}
#endif
