#ifndef INC_4_1_H
#define INC_4_1_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct {
    double x;
    double y;
} Point;

double cross_product(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void check_convex_polygon(int num_points, ...) {
    if (num_points < 3) {
        printf("Многоугольник должен иметь хотя бы 3 вершины.\n");
        return;
    }

    va_list args;
    va_start(args, num_points);

    Point *points = malloc(num_points * sizeof(Point));
    if (points == NULL) {
        printf("Ошибка выделения памяти.\n");
        va_end(args);
        return;
    }

    for (int i = 0; i < num_points; i++) {
        points[i].x = va_arg(args, double);
        points[i].y = va_arg(args, double);
    }

    va_end(args);

    for (int i = 0; i < num_points; i++) {
        for (int j = i + 1; j < num_points; j++) {
            if (points[i].x == points[j].x && points[i].y == points[j].y) {
                printf("Ошибка: есть совпадающие точки.\n");
                free(points);
                return;
            }
        }
    }

    int sign = 0;
    for (int i = 0; i < num_points; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % num_points];
        Point p3 = points[(i + 2) % num_points];
        double cross_prod = cross_product(p1, p2, p3);
        if (cross_prod != 0) {
            if (sign == 0) {
                sign = (cross_prod > 0) ? 1 : -1;
            } else if ((cross_prod > 0 && sign < 0) || (cross_prod < 0 && sign > 0)) {
                printf("Многоугольник не является выпуклым.\n");
                free(points);
                return;
            }
        }
    }
    printf("Многоугольник является выпуклым.\n");
    free(points);
}

#endif // INC_4_1_H
