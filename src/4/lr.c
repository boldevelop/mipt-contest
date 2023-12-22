/*  разворот списка (итеративно)
    в списке нет петли */

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
struct node_t {
    struct node_t *next;
    int data;
};
#endif
typedef struct node_t Node;

Node *reverse(Node * top)
{
    Node *cur = top;
    Node *prev = NULL;
    Node *next = NULL;

    if (top == NULL) {
        return top;
    }

    while (cur != NULL) {
        next = cur->next;

        cur->next = prev;

        prev = cur;
        cur = next;
        next = NULL;
    }

    return prev;
}

#ifdef DEBUG
Node *r_reverse(Node * top)
{
    Node *xs;
    if (NULL == top)
        return NULL;
    if (NULL == top->next)
        return top;
    xs = r_reverse(top->next);
    top->next->next = top;
    top->next = NULL;
    return xs;
}

Node *alloc_list(const int *arr, const int s);
void free_list(Node * top);
Node *alloc_node(int data);
void push_to_list(Node ** l, int data);
void dump_list(const Node * top);
int is_equal(Node * l, Node * r);

void test(const int *arr, const int s, const int i)
{
    Node *expected = alloc_list(arr, s);
    Node *test = alloc_list(arr, s);
    expected = r_reverse(expected);
    test = reverse(test);
    if (is_equal(test, expected)) {
        printf("%d: OK\n", i);
    } else {
        printf("%d: NOT OK\n", i);
        printf("Exp: ");
        dump_list(expected);
        printf("Got: ");
        dump_list(test);
    }
    free_list(test);
    free_list(expected);
    printf("\n");
}

int main()
{
    int i = 1;
    {
        int s = 4;
        int arr[4] = { 2, 4, 6, 1 };
        test(arr, s, i);
        i++;
    }
    {
        int s = 1;
        int arr[1] = { 2 };
        test(arr, s, i);
        i++;
    }
    {
        int s = 2;
        int arr[2] = { 1, 2 };
        test(arr, s, i);
        i++;
    }
    {
        int s = 3;
        int arr[3] = { 1, 2, 3 };
        test(arr, s, i);
        i++;
    }
    {
        int s = 0;
        int arr[3] = { 1, 2, 3 };
        test(arr, s, i);
        i++;
    }
}

int is_equal(Node * l, Node * r)
{
    while (l != NULL && r != NULL) {
        if (l->data != r->data) {
            return 0;
        }
        l = l->next;
        r = r->next;
    }
    return l == NULL && r == NULL;
}

Node *alloc_list(const int *arr, const int s)
{
    Node *l = NULL;
    for (int i = 0; i < s; i++) {
        push_to_list(&l, arr[i]);
    }
    return l;
}

void free_list(Node * top)
{
    while (top != NULL) {
        Node *tmp = top;
        top = top->next;
        free(tmp);
        tmp = NULL;
    }
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

void push_to_list(Node ** l, int data)
{
    Node *top = *l;
    if (*l == NULL) {
        *l = alloc_node(data);
        return;
    }
    while (top->next != NULL) {
        top = top->next;
    }
    top->next = alloc_node(data);
}

void dump_list(const Node * top)
{
    const Node *tmp = top;
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
