#include <Windows.h>
#include <algorithm>
#include<iostream>
using namespace std;

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x{ x }, y{ y } {};
};


Point operator*(double a, const Point& p) {
    return Point(a * p.x, a * p.y);
}


Point operator+(const Point& p, const Point& p2) {
    return Point(p2.x + p.x, p2.y + p.y);
}



int Round(double a) {
    return (0.5 + a);
}
template<typename T>
void mySwap(T &a,T &b) {
    T temp = a;
    a = b;
    b = temp;
}

double distance(Point p1, Point p2) {
    return sqrt((pow(p2.x - p1.x, 2)) + pow(p2.y - p1.y, 2));
}
void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1, dy = y2 - y1;
    SetPixel(hdc, x1, y1, c);
    if (abs(dx) >= abs(dy))
    {
        double m = (double)dy / dx;
        int x = x1;
        double y = y1;
        while (x < x2)
        {
            x++;
            y += m;
            SetPixel(hdc, x, Round(y), c);
        }
    }
    else {
        double mi = (double)dx / dy;
        int y = y1;
        double x = x1;
        while (y < y2)
        {
            y++;
            x += mi;
            SetPixel(hdc, Round(x), y, c);
        }

    }
}


void drawBazRec(HDC hdc, Point p1, Point p2, Point p3, Point p4, COLORREF c) {
    if (distance(p1, p4) <= 1)return;
    Point q1 = 0.5 * (p1 + p2);
    Point q2 = 0.5 * (p2 + p3);
    Point q3 = 0.5 * (p3 + p4);
    Point r1 = 0.5 * (q1 + q2);
    Point r2 = 0.5 * (q2 + q3);
    Point midPoint = 0.5 * (r1+  r2);
    SetPixel(hdc,Round(midPoint.x),Round(midPoint.y),c);
    drawBazRec(hdc, p1, q1, r1, midPoint,c);
    drawBazRec(hdc, p4, q3, r2, midPoint,c);
}

void fillRecBaz(HDC hdc, Point p[], int n, COLORREF c) {
    // Draw rectangle border
    DrawLineDDA(hdc, p[0].x, p[0].y, p[1].x, p[1].y, c);
    DrawLineDDA(hdc, p[0].x, p[0].y, p[3].x, p[3].y, c);
    DrawLineDDA(hdc, p[1].x, p[1].y, p[2].x, p[2].y, c);
    DrawLineDDA(hdc, p[3].x, p[3].y, p[2].x, p[2].y, c);


    int yTop = p[0].y;
    int yBottom = p[3].y;
    if (yTop > yBottom) std::swap(yTop, yBottom);

    int height = yBottom - yTop;
    int waveH = height / 4; // wave height



    for (double y = yTop; y <= yBottom; y+=0.5) {
        Point left(p[0].x, y);
        Point right(p[1].x, y);

        int offset = waveH * sin((y - yTop) * 3.14 / height);  

        Point control1 = Point((2 * left.x + right.x) / 3, y + offset / 2);
        Point control2 = Point((left.x + 2 * right.x) / 3, y - offset / 2);
      
        drawBazRec(hdc, left, control1, control2, right, c);

    }
}



/*void fillRecBaz(HDC hdc, Point p[], int n, COLORREF c) {
    // to draw the rectangle
    DrawLineDDA(hdc, p[0].x, p[0].y, p[1].x, p[1].y, c);
    DrawLineDDA(hdc, p[0].x, p[0].y, p[3].x, p[3].y, c);
    DrawLineDDA(hdc, p[1].x, p[1].y, p[2].x, p[2].y, c);
    DrawLineDDA(hdc, p[3].x, p[3].y, p[2].x, p[2].y, c);
    Point p1 = p[0], p2 = p[1], p3 = p[2], p4 = p[3];
    double xleft = p1.x, xright = p3.x;
    double ytop = p1.y,ybottom = p4.y;


    for (int x = xleft; x <= xright; x++) {
        drawBazRec(hdc, p1, Point(x,ytop), Point(x,ybottom), p4,c);
        x++;
        ytop++;
        ybottom--;
        drawBazRec(hdc, p1, Point(x, ytop), Point(x, ybottom), p4, c);
        p1.x++;
        p4.x++;

    }


}

*/

Point p1 = { 100, 100 };  // Top-left
Point p2 = { 200, 100 };  // Top-right
Point p3 = { 200, 200 };  // Bottom-right
Point p4 = { 100, 200 };  // Bottom-left

Point p[4] = { p1,p2,p3,p4 };

LRESULT WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc;

    switch (m)
    {
    case WM_LBUTTONDOWN: // to get the center of the circle
        hdc = GetDC(hwnd);
 
        ReleaseDC(hwnd, hdc);
        break;

    case WM_LBUTTONUP:
        hdc = GetDC(hwnd);
        fillRecBaz(hdc,p, 4, RGB(0, 0, 0));
        ReleaseDC(hwnd, hdc);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd); break;
    case WM_DESTROY:
        PostQuitMessage(0); break;
    default:return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.lpszClassName = L"MyClass";
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hi;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(L"MyClass", L"Hello World!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hi, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

