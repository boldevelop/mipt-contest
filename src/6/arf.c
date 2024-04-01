#include <math.h>

long foo(unsigned x)
{
    float xf = x;
    float res = log(log(xf));
    res += log(xf);
    res *= xf;
    res = round(res);
    return res;
}

int main()
{
}
