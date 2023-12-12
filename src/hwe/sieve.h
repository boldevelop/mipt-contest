struct sieve_t {
  int n;
  unsigned char *mod1;
  unsigned char *mod5;  
};

void fill_sieve(struct sieve_t *sv);
int is_prime(struct sieve_t *sv, unsigned n);
struct sieve_t init_sieve(int num);