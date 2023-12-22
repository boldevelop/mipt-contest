/* длина петли в списке */

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
struct node_t {
    struct node_t *next;
    int data;
};
#endif
typedef struct node_t Node;

int brent_impl(Node * top)
{
    int pow = 1;
    int lam = 1;
    Node *turtle = top;
    Node *hare = top;
    if (hare == NULL) {
        return 0;
    }
    hare = hare->next;

    while (hare != turtle) {
        if (hare == NULL) {
            return 0;
        }
        if (pow == lam) {
            turtle = hare;
            pow *= 2;
            lam = 0;
        }
        hare = hare->next;
        lam++;
    }

    return lam;
}

// определяет длину петли в односвязном списке
// возвращает длину если есть, 0 если петли нет
int loop_len(struct node_t *top)
{
    return brent_impl(top);
}

#ifdef DEBUG

void free_list(Node * top);
Node *alloc_node(int data);
void push_to_list(Node ** l, int data);
void dump_list(const Node * top);

int f(int x)
{
    int s = 5;
    static int arr[7] = { 0, 1, 2, 3, 4 };
    int next = (x + 1) % s;
    return arr[next];
}

int brent(int x0)
{
    int power = 1, lam = 1;
    int tu = x0;
    int hare = f(x0);

    while (hare != tu) {
        if (power == lam) {
            printf("h: %d t: %d power: %d\n", hare, tu, power);
            tu = hare;
            power *= 2;
            lam = 0;
        } else {
            printf("h: %d lam: %d\n", hare, lam);
        }
        hare = f(hare);
        lam += 1;
    }
    return lam;
}

int main()
{
    Node *l = NULL;

    // {
    //     int x0 = 0;
    //     printf("%d \n", brent(x0));
    // }

    {
        /* 2 → 4 → 6 → 1 */
        int arr[4] = { 2, 4, 6, 1 };
        for (int i = 0; i < 4; ++i) {
            push_to_list(&l, arr[i]);
        }
        printf("%d\n", loop_len(l));
        free_list(l);
        l = NULL;
    }

    {
        /*  2 → 4 → 6 → 2 → 4 → 6 → ... */
        int arr[3] = { 2, 4, 6 };
        for (int i = 0; i < 3; ++i) {
            push_to_list(&l, arr[i]);
        }
        l->next->next->next = l;
        printf("%d\n", loop_len(l));
        // dump_list(l);
        // free_list(l);
        l = NULL;
    }

    {
        /* 2 → 4 → 6 → 4 → 6 → ... */
        int arr[3] = { 2, 4, 6 };
        for (int i = 0; i < 3; ++i) {
            push_to_list(&l, arr[i]);
        }
        l->next->next->next = l->next;
        printf("%d\n", loop_len(l));
        // dump_list(l);
        // free_list(l);
        l = NULL;
    }

    {
        /* 2 → 4 → 6 → 6 → 6 → ... */
        int arr[3] = { 2, 4, 6 };
        for (int i = 0; i < 3; ++i) {
            push_to_list(&l, arr[i]);
        }
        l->next->next->next = l->next->next;
        printf("%d\n", loop_len(l));
        // dump_list(l);
        // free_list(l);
        l = NULL;
    }
    {
        push_to_list(&l, 1);
        l->next = l;
        printf("%d\n", loop_len(l));
        l = NULL;
    }
    {
        int is_loop;
        push_to_list(&l, 1);
        is_loop = loop_len(l);
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
