#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef long long ull;

ull llabs(ull x) { return (x < 0) ? -x : x; }
ull gcd(ull x, ull y) {
  ull q;
  x = llabs(x);
  y = llabs(y);
  if (y > x) {
    ull t = y;
    y = x;
    x = t;
  }

  do {
    assert (y > 0);
    q = x % y;
    x = y;
    y = q;
  } while (q != 0);

  return x;
}

int main() {
  ull x = 0, y = 0, g;
  int res;

  res = scanf("%llu %llu", &x, &y);
  assert(res == 2);
  g = gcd(x, y);
  printf("%llu\n", g);
  return 0;
}
