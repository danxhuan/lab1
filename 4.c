#include <4.1.h>

int main() {
    check_convex_polygon(5, 0.0, 0.0, 2.0, 2.0, 4.0, 0.0, 3.0, -2.0, 1.0, -2.0);

    check_convex_polygon(5, 0.0, 0.0, 2.0, 2.0, 4.0, 0.0, 3.0, -2.0, 2.0, -1.0);

    check_convex_polygon(5, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0, 3.0, -2.0, 1.0, -2.0);

    check_convex_polygon(3, -.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    return 0;
}
