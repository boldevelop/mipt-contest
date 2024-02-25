/* поиск в строке независимо от регистра */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

char *strstrci(char const *needle, char const *haystack)
{
    char *result = NULL;
    int j = 0;
    int i = 0;

    int n_l = strlen(needle);
    int h_l = strlen(haystack);
    int end = h_l - n_l + 1;

    if (n_l == 0) {
        return (char *) haystack;
    }

    if (h_l == 0) {
        return NULL;
    }

    while (i < end) {
        char const *ch_h = haystack + i + j;
        char const *ch_n = needle + j;

        if (j == n_l) {
            result = (char *) haystack + i;
            break;
        }

        if (tolower(*ch_h) != tolower(*ch_n)) {
            i++;
            j = 0;
        } else {
            j++;
        }
    }

    return result;
}

#ifdef DEBUG
int main()
{
    {
        char const *needle = "Ab", *src = "abracadaBra";
        char *pos1, *pos2, *pos3;
        pos1 = strstrci(needle, src);
        assert(pos1 != NULL);
        printf("%s\n", pos1);
        pos2 = strstrci(needle, pos1 + 2);
        assert(pos2 != NULL);
        printf("%s\n", pos2);
        pos3 = strstrci(needle, pos2 + 2);
        assert(pos3 == NULL);
    }
    {
        char const *needle = "Ab", *src = "ab";
        char *pos1 = strstrci(needle, src);
        assert(pos1 != NULL);
        printf("%s\n", pos1);
    }
    {
        char const *needle = "Ab", *src = "abABAb";
        char *pos1 = strstrci(needle, src);
        assert(pos1 != NULL);
        printf("%s\n", pos1);

        pos1 = strstrci(needle, pos1 + 2);
        assert(pos1 != NULL);
        printf("%s\n", pos1);

        pos1 = strstrci(needle, pos1 + 2);
        assert(pos1 != NULL);
        printf("%s\n", pos1);
    }
    {
        char const *needle = "Abb", *src = "ab";
        char *pos1 = strstrci(needle, src);
        assert(pos1 == NULL);
    }

    {
        char const *needle = "egubq", *src =
            "UvsegUBqeOVQpApROEhEVfEhFjWjVrvCkfsJHHSLmAEIanSGnYQYMSlCKBNSVgrGptAHMWRVQSDSajqdjdTEaNBkKvUmnDNmalQD";
        char *pos1 = strstrci(needle, src);
        assert(pos1 != NULL);
    }
    {
        char const *needle = "qwewe", *src = "";
        char *pos1 = strstrci(needle, src);
        assert(pos1 == NULL);
    }
}
#endif
