#include <stdint.h>

/* return 1 if l < r */
typedef int (*Less)(const void *l, const void *r);

void timsort(uintptr_t * d, const int s, Less less);

#ifdef TEST
#define MAX_STACK_SIZE 85
struct run_t {
    uintptr_t *d;
    int s;
};
typedef struct run_t Run;

struct stack_t {
    Run d[MAX_STACK_SIZE];
    int s;
};
typedef struct stack_t Stack;

struct ts_state_t {
    Less less;
    Stack st;
};
typedef struct ts_state_t TsState;
void merge(TsState * state);
#endif
