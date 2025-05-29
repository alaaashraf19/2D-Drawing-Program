#pragma once

#include <cmath>

int Round(double x);
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

Point operator*(double a, const Point& p) {
    return Point(a * p.x, a * p.y);
}


Point operator+(const Point& p, const Point& p2) {
    return Point(p2.x + p.x, p2.y + p.y);
}

