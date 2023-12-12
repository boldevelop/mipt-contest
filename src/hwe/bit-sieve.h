
typedef unsigned long long ull;
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

struct bit_sieve_t {
    int n;
    unsigned char *s;
};

struct bit_sieve_t init_bsieve(int num);
int is_bprime(struct bit_sieve_t *sv, ull probe);