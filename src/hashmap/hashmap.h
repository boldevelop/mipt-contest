#ifndef HASHMAP_H
#define HASHMAP_H

typedef void *HashMap;

HashMap hm_ctor(int strl);
int hm_find(HashMap hm, char *key);
void hm_insert(HashMap hm, char *key);
void hm_dctor(HashMap hm);
int hm_rehash(HashMap hm, int desired_size);

#endif
