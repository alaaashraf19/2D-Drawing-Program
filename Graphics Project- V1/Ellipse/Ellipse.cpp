
#include "Ellipse.h"
using namespace std;

void Draw4points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
}
vector<int> Get_Values(vector<Point>& v) {
    int xc = (v[0].x + v[1].x) / 2;
    int yc = (v[0].y + v[1].y) / 2;
    int rx = abs(v[1].x - v[0].x) / 2;
    int ry = abs(v[1].y - v[0].y) / 2;
    vector<int> vec;
    v.clear();
    vec.push_back(xc);
    vec.push_back(yc);
    vec.push_back(rx);
    vec.push_back(ry);
    return vec;
}
void Ellipse_Midpoint(HDC hdc, vector<Point>& v, COLORREF c) {
    vector<int> vec = Get_Values(v);
    int xc = vec[0];
    int yc = vec[1];
    int rx = vec[2];
    int ry = vec[3];
    int a = rx;
    int b = ry;
    int x = 0;
    int y = ry;
    double dx, dy;
    double d = (b * b) + (0.25 * a * a) - (a * a) * b; //Initial decision parameter for region 1 at x=0 y=ry=b
    dx = 2 * (b * b) * x;
    dy = 2 * (a * a) * y;
    while (dx < dy) {//region 1
        Draw4points(hdc, xc, yc, x, y, c);
        if (d < 0) {//inside
            x++;
            dx += 2 * b * b;
            d += dx + (b * b);

        }
        else {
            x++;
            y--;
            dy -= 2 * a * a;
            dx += 2 * b * b;
            d += (dx - dy) + (b * b);
        }
    }
    double d2 = (b * b) * ((x + 0.5) * (x + 0.5)) + (a * a) * ((y - 1) * (y - 1)) - ((a * a) * (b * b));
    while (y >= 0) {//region2
        Draw4points(hdc, xc, yc, x, y, c);
        if (d2 > 0) {
            y--;
            dy -= 2 * a * a;

            d2 += (a * a) - dy;
        }
        else {
            y--;
            x++;
            dy -= 2 * a * a;
            dx += 2 * b * b;

            d2 += (dx - dy) + (a * a);
        }
    }

}
void Ellipse_polar(HDC hdc, vector<Point>& v, COLORREF c) {
    vector<int> vec = Get_Values(v);
    int xc = vec[0];
    int yc = vec[1];
    int rx = vec[2];
    int ry = vec[3];
    double dtheta = 1.0 / max(rx, ry);
    int x = 0;
    int y = ry;
    double theta = 0;
    Draw4points(hdc, xc, yc, x, y, c);
    while (y >= 0) {
        theta += dtheta;
        x = rx * cos(theta);
        y = ry * sin(theta);
        Draw4points(hdc, xc, yc, x, y, c);
    }
}


void Ellipse_Direct(HDC hdc, vector<Point>& v, COLORREF c) {
    vector<int> vec = Get_Values(v);
    int xc = vec[0];
    int yc = vec[1];
    int rx = vec[2];
    int ry = vec[3];
    int x = 0;
    int y = ry;
    Draw4points(hdc, xc, yc, x, y, c);
    while (x <= rx) {
        x++;
        double new_y = ry * sqrt(1.0 - (double)(x * x) / (rx * rx));
        Draw4points(hdc, xc, yc, x, Round(new_y), c);
    }
    while (y >= 0) {
        y--;
        double new_x = rx * sqrt(1.0 - (double)(y * y) / (ry * ry));
        Draw4points(hdc, xc, yc, Round(new_x), y, c);
    }
}