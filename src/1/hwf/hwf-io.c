#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

void trancuate(char *input, char *trancuated)
{
    while (*input) {
        if (isspace(*input)) {
            input++;
            continue;
        }
        *trancuated = *input;
        trancuated++;
        input++;
    }
}

int is_equal(char *l, char *r)
{
    while (*l && *r) {
        if (*l != *r) {
            return 0;
        }
        l++;
        r++;
    }
    return 1;
}

void flush()
{
    while (getchar() != '\n');
}

/* 2147483648 borders [min, max) */
int read_int(int min, int max)
{
    int val, size;
    char input[15];
    char trancuated[15] = { 0 };
    char *input_end;

    for (;;) {
        for (;;) {
            errno = 0;
            if (fgets(input, sizeof(input), stdin) == NULL) {
                exit(0);
            }

            val = strtol(input, &input_end, 10);

            if (errno == ERANGE) {
                printf("Wrong input. Too big number\n");
                continue;
            }

            if (*input_end != '\n') {
                flush();
                printf("Wrong input\n");
                continue;
            }

            if (input_end <= input) {
                continue;
            }

            if (*input_end == '\0') {
                printf("Wrong input. Too big number\n");
                continue;
            }

            trancuate(input, trancuated);
            sprintf(input, "%d", val);
            if (is_equal(input, trancuated) == 0) {
                printf("Wrong input. Overflow number\n");
                continue;
            }

            break;
        }

        if (val < min) {
            size = input_end - input;
            printf("Size %d\n", size);

            if (size >= 10) {
                printf("Wrong input. Too big number\n");
            } else {
                printf("Wrong input. Should more or equal than %d\n", min);
            }
            continue;
        }

        if (val > max) {
            printf("Wrong input. Should less or equal than %d\n", max);
            continue;
        }

        break;
    }

    return val;
}

int prompt(const char *question)
{
    int c;
    printf("%s [y/n] ", question);
    do {
        c = getchar();
        if (c == EOF) {
            printf("\n");
            exit(0);
        }
    } while (isspace(c) || (c != 'y' && c != 'n' && c != 'Y' && c != 'N'));
    flush();
    return c == 'y' || c == 'Y';
}
