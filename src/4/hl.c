/* петля в списке */

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
struct node_t {
    struct node_t *next;
    int data;
};
#endif
typedef struct node_t Node;

int floyds_impl(Node * top)
{
    Node *turtle = top;
    Node *hare = top;

    while (hare && hare->next) {
        turtle = turtle->next;
        hare = hare->next->next;
        if (turtle == hare) {
            return 1;
        }
    }
    return 0;
}

int brent_impl(Node * top)
{
    int pow = 1;
    int c = 1;
    Node *turtle = top;
    Node *hare = top;
    if (hare == NULL) {
        return 0;
    }
    hare = hare->next;

    while (hare != NULL) {
        if (hare == turtle) {
            return 1;
        }
        if (pow == c) {
            pow *= 2;
            turtle = hare;
            c = 0;
        }
        hare = hare->next;
        c++;
    }

    return 0;
}

// определяет есть ли петля в односвязном списке
// возвращает 0 если нет и 1 если есть
int list_is_a_loop(struct node_t *top)
{
    return brent_impl(top);
}

#ifdef DEBUG

void free_list(Node * top);
Node *alloc_node(int data);
void push_to_list(Node ** l, int data);
void dump_list(const Node * top);

int main()
{
    Node *l = NULL;

    {
        int arr[4] = { 2, 4, 6, 1 };
        for (int i = 0; i < 4; ++i) {
            push_to_list(&l, arr[i]);
        }
        printf("%d\n", list_is_a_loop(l));
        free_list(l);
        l = NULL;
    }

    {
        int arr[3] = { 2, 4, 6 };
        for (int i = 0; i < 3; ++i) {
            push_to_list(&l, arr[i]);
        }
        l->next->next->next = l;
        printf("%d\n", list_is_a_loop(l));
        // dump_list(l);
        // free_list(l);
        l = NULL;
    }

    {
        int arr[3] = { 2, 4, 6 };
        for (int i = 0; i < 3; ++i) {
            push_to_list(&l, arr[i]);
        }
        l->next->next->next = l->next->next;
        printf("%d\n", list_is_a_loop(l));
        // dump_list(l);
        // free_list(l);
        l = NULL;
    }
    {
        push_to_list(&l, 1);
        l->next = l;
        printf("%d\n", list_is_a_loop(l));
        l = NULL;
    }
    {
        int is_loop;
        push_to_list(&l, 1);
        is_loop = list_is_a_loop(l);
        printf("%d\n", is_loop);
        l = NULL;
    }
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
