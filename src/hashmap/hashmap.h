#ifndef HASHMAP_H
#define HASHMAP_H

struct bucket_t {
    struct bucket_t *next;
    char *key;
    int d;
};
typedef struct bucket_t Bucket;

typedef struct {
    unsigned a;
    unsigned b;
    unsigned p;
    unsigned s;
    Bucket **buckets;
} HashMap;

HashMap *init_hm(int strl);
int find_hm(HashMap *hm, char *key);
void insert_hm(HashMap *hm, char *key);
void destroy_hm(HashMap *hm);

#endif