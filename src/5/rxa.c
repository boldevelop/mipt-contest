#include <stdlib.h>
#include <stdio.h>

char astate[6] = { 0, 1, 1, 5, 5, 5 };
char bstate[6] = { 0, 2, 2, 4, 4, 5 };
char cstate[6] = { 0, 5, 3, 3, 3, 5 };

int main()
{
    char c;
    int st = 1;
    while ((c = getchar()) != EOF) {
        if (c != 'a' && c != 'b' && c != 'c') {
            break;
        }
        switch (c) {
        case 'a':
            st = astate[st];
            break;
        case 'b':
            st = bstate[st];
            break;
        case 'c':
            st = cstate[st];
            break;
        default:
            break;
        }
    }
    printf("%d\n", st == 3 || st == 4 || st == 2);
}
