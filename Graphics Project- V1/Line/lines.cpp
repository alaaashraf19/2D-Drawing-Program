#include "lines.h"

using namespace std;
int Round(double x) {
    return (int)(x + 0.5);
}
void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = x2 - x1, dy = y2 - y1;
    SetPixel(hdc, x1, y1, c);
    if (abs(dx) >= abs(dy)) {
        double m = (double)dy / dx;
        int x = x1;
        double y = y1;
        while (x < x2) {
            x++;
            y += m;
            SetPixel(hdc, x, Round(y), c);
        }

    }
    else {
        double mi = (double)dx / dy;
        int y = y1;
        double x = x1;
        while (y < y2) {
            y++;
            x += mi;
            SetPixel(hdc, Round(x), y, c);
        }
    }
}

void DrawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {

    bool steep = abs(y2 - y1) > abs(x2 - x1);//if dy>dx we swap roles of x and y like right and left instead of vertical up and down 
    if (steep) {
        swap(x1, y1);
        swap(x2, y2);
    }

    if (x2 < x1) {
        swap(x1, x2);
        swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = abs(y2 - y1);
    int ystep = (y1 < y2) ? 1 : -1; // lw y2<y1 n3ml y-- y3ni ystep=-1

    int d = dx - 2 * dy;//initial value of d
    int d1 = -2 * dy;
    int d2 = 2 * (dx - dy);

    int x = x1, y = y1;

    while (x < x2) {
        if (steep)
            SetPixel(hdc, y, x, c);  // x and y roles are swapped
        else
            SetPixel(hdc, x, y, c);
        if (d > 0) {
            d += d1;

        }
        else {
            d += d2;
            y += ystep;
        }
        x++;

    }

}

void DrawLineParametric(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    double dt = 1.0 / max(abs(dx), abs(dy));
    for (double t = 0; t <= 1; t += dt) {
        int x = Round(x1 + t * dx);
        int y = Round(y1 + t * dy);
        SetPixel(hdc, x, y, c);
    }
}