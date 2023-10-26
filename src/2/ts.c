/* Площадь треугольника */
#include <stdio.h>

struct point_t {
    int x, y;
};

struct triangle_t {
    struct point_t pts[3];
};

int double_area(struct triangle_t tr)
{
    int area = 0;
    area += tr.pts[1].x * tr.pts[2].y - tr.pts[2].x * tr.pts[1].y;
    area += tr.pts[2].x * tr.pts[0].y - tr.pts[0].x * tr.pts[2].y;
    area += tr.pts[0].x * tr.pts[1].y - tr.pts[1].x * tr.pts[0].y;

    if (area < 0) {
        return -area;
    }
    return area;
}


int main()
{
    struct point_t pts[3] = { { -3, -10 }, { 8, 8 }, { 6, -2 } };
    struct triangle_t tr = { 0 };
    tr.pts[0] = pts[0];
    tr.pts[1] = pts[1];
    tr.pts[2] = pts[2];
    printf("%d\n", double_area(tr));
}
