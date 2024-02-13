#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"

char *calloc_c(size_t s)
{
    char *b = calloc(s, sizeof(char));
    if (!b) {
        abort();
    }
    return b;
}

#define WRD_S 121
#define WRDS_COUNT WRD_S - 1
#define ALPH_S 26

static char *get_word(char *words, int num)
{
    return words + (num * WRD_S);
}

#define REHASH_1 2500
#define REHASH_2 5000
#define REHASH_3 50000
#define REHASH_4 150000

static void init_words(char *words)
{
    char *word;
    for (int i = 0; i < WRDS_COUNT; ++i) {
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            int word_num = ch - 'a';
            word = get_word(words, word_num);
            word[i] = ch;
        }
    }
}

static void insert_words(HashMap hm, char *words)
{
    char *word;
    for (int i = 0; i < WRDS_COUNT; ++i) {
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            int word_num = ch - 'a';
            word = get_word(words, word_num);
            word[i + 1] = '\0';
            hm_insert(hm, word);
            word[i + 1] = ch;
        }
    }
}

static void assert_words(HashMap hm, char *words, int exp)
{
    char *word;
    int act = 0;
    for (int i = 0; i < WRDS_COUNT; ++i) {
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            int word_num = ch - 'a';
            word = get_word(words, word_num);
            word[i + 1] = '\0';
            act = hm_find(hm, word);
            if (act != exp) {
                printf("%d != %d: %s\n", act, exp, word);
                abort();
            }
            word[i + 1] = ch;
        }
    }
}

int main()
{
    HashMap hm = hm_ctor(0);
    char *words = calloc_c(ALPH_S * WRD_S);
    int exp = 0;

    init_words(words);

    insert_words(hm, words);
    exp++;
    assert_words(hm, words, exp);

    hm_rehash(hm, REHASH_1);
    assert_words(hm, words, exp);

    insert_words(hm, words);
    exp++;
    assert_words(hm, words, exp);
    hm_rehash(hm, REHASH_2);
    assert_words(hm, words, exp);

    insert_words(hm, words);
    exp++;
    assert_words(hm, words, exp);
    hm_rehash(hm, REHASH_3);
    assert_words(hm, words, exp);

    insert_words(hm, words);
    exp++;
    assert_words(hm, words, exp);
    hm_rehash(hm, REHASH_4);
    assert_words(hm, words, exp);

    printf("test\n");

    free(words);
    hm_dctor(hm);
}
