#include "floodfill.h"
using namespace std;

void RecFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc) {
    COLORREF c = GetPixel(hdc, x, y);
    if (c == bc || c == fc) return;
    SetPixel(hdc, x, y, fc);
    RecFloodFill(hdc, x + 1, y, bc, fc);
    RecFloodFill(hdc, x - 1, y, bc, fc);
    RecFloodFill(hdc, x, y + 1, bc, fc);
    RecFloodFill(hdc, x, y - 1, bc, fc);

}


struct Point {
    int x, y;
    Point(int x1 = 0, int y1 = 0) : x(x1), y(y1) {}
};
void NonRecFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc) {
    stack <Point> st;

    st.push(Point(x, y));
    while (!st.empty()) {
        Point p = st.top();
        st.pop();
        COLORREF c = GetPixel(hdc, p.x, p.y);
        if (c == bc || c == fc) continue;
        SetPixel(hdc, p.x, p.y, fc);
        st.push(Point(p.x + 1, p.y));
        st.push(Point(p.x - 1, p.y));
        st.push(Point(p.x, p.y + 1));
        st.push(Point(p.x, p.y - 1));

    }
}