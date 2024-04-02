int foo(const int *x, int s)
{
    int cur, j, w7;
    cur = *x;

    if (s <= 1) {
        return cur;
    }

    j = 1;
    w7 = 1;

    for (int i = 1; i < s; ++i) {
        int i_val = *(x + i);
        int w5 = j - 1;

        if (i_val != cur) {
            j = w5;
        } else {
            j++;
        }

        if (j == 0) {
            cur = i_val;
            j = w7;
        }
    }

    return cur;
}

int main()
{
}
