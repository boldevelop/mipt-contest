/* суммирование в строке */

#include <stdlib.h>
#include <stdio.h>

int main()
{
    char c;
    int sum = 0;
    while ((scanf("%c", &c)) != EOF) {
        sum += c;
    }
    printf("%d\n", sum);
}
