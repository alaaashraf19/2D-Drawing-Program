#include "filling.h"
#include "../Curves/curves.h"
#include <algorithm>

using namespace std;

#define PI 3.14

template<typename T>
void mySwap(T &a,T &b) {
    T temp = a;
    a = b;
    b = temp;
}
void RecFloodFill(HDC hdc, int x, int y, COLORREF bg, COLORREF fc) {
    COLORREF c = GetPixel(hdc, x, y);
    if (c != bg || c == fc) return;
    SetPixel(hdc, x, y, fc);
    RecFloodFill(hdc, x + 1, y, bg, fc);
    RecFloodFill(hdc, x - 1, y, bg, fc);
    RecFloodFill(hdc, x, y + 1, bg, fc);
    RecFloodFill(hdc, x, y - 1, bg, fc);

}



void NonRecFloodFill(HDC hdc, int x, int y, COLORREF fc) {
    stack <Point> st;
    COLORREF bg = GetPixel(hdc, x, y);
    if (bg == fc) return;
    st.push(Point(x, y));
    while (!st.empty()) {
        Point p = st.top();
        st.pop();
        COLORREF c = GetPixel(hdc, p.x, p.y);
        //if (c == bc || c == fc) continue;
        if (c != bg) continue;
        SetPixel(hdc, p.x, p.y, fc);
        st.push(Point(p.x + 1, p.y));
        st.push(Point(p.x - 1, p.y));
        st.push(Point(p.x, p.y + 1));
        st.push(Point(p.x, p.y - 1));

    }
}

///Convex Filling

typedef struct {
    int xleft, xright;
}Edgetable;

void init(Edgetable tbl[]) {
    for (int i = 0; i < 800; i++) {
        tbl[i].xleft = INT_MAX;
        tbl[i].xright = INT_MIN;
    }
}


void Edge2Table(Point p1, Point p2, Edgetable tbl[]) {
    if (p1.y == p2.y) return;
    if (p1.y > p2.y) std::swap(p1, p2);
    double mi = (double)(p2.x - p1.x) / (p2.y - p1.y);
    int y = p1.y;
    double x = p1.x;
    while (y < p2.y) {
        if (x < tbl[y].xleft) tbl[y].xleft = ceil(x);
        if (x > tbl[y].xright) tbl[y].xright = floor(x);
        y++;
        x += mi;
    }
}
void polygon2Table(std::vector<Point> v, int n, Edgetable tbl[]) {
    Point v1 = v[n - 1];
    for (int i = 0; i < n; i++) {
        Point v2 = v[i];
        Edge2Table(v1, v2, tbl);
        v1 = v[i];
    }
}

void Table2screen(HDC hdc, Edgetable tbl[], COLORREF c) {
    for (int i = 0; i < 800; i++) {
        if (tbl[i].xleft < tbl[i].xright) {
            DrawLineBresenham(hdc, tbl[i].xleft, i, tbl[i].xright, i, c);
        }
    }
}
void ConvexFill(HDC hdc, std::vector<Point> v, int n, COLORREF c) {
    Edgetable tbl[800];
    init(tbl);
    polygon2Table(v, n, tbl);
    Table2screen(hdc, tbl, c);

}


//Non convex fill
struct node {
    double x;
    double slopeinv;
    int ymax;

    bool operator<(node n) {
        return x < n.x;
    }
};

node Init_nodes(Point &v1, Point &v2) {
    if (v1.y > v2.y)
        swap(v1, v2);
    node n;
    n.x = v1.x;
    n.ymax = v2.y;
    n.slopeinv = double(v2.x - v1.x) / (v2.y - v1.y);
    return n;
}

void Init_edges(vector<Point> v, int n, vector<list<node>>  &Ys) {
    Point v1 = v[n - 1];
    for (int i = 0; i < n; ++i) {
        Point v2 = v[i];
        if (v1.y == v2.y) {
            v1 = v2;
            continue;
        }
        node n = Init_nodes(v1, v2);
        Ys[v1.y].push_back(n);
        v1 = v[i];
    }
}

void NonConvexFill(HDC hdc, std::vector<Point> v, int n, COLORREF c) {
    vector<list<node>> Ys(800);
    Init_edges(v, n, Ys);
    int y = 0;
    while (y < 800 && Ys[y].size() == 0)
        y++;
    if (y == 800)
        return;

    list <node> ActiveList = Ys[y];
    while (ActiveList.size() > 0)
    {
        ActiveList.sort();
        for (auto it = ActiveList.begin(); it != ActiveList.end(); ++it) {
            int x1 = (int)ceil(it->x);
            it++;
            int x2 = (int)floor(it->x);
            DrawLineBresenham(hdc, x1, y, x2, y, c);
        }
        y++;

        auto it = ActiveList.begin();
        while (it != ActiveList.end()) {
            if (y == it->ymax)
                it = ActiveList.erase(it);
            else
                it++;
        }
        for (auto it = ActiveList.begin(); it != ActiveList.end(); it++)
            it->x += it->slopeinv;
        ActiveList.insert(ActiveList.end(), Ys[y].begin(), Ys[y].end());
    }
}


double distance(Point p1, Point p2) {
    return sqrt((pow(p2.x - p1.x, 2)) + pow(p2.y - p1.y, 2));
}

void fillRecBezier(HDC hdc, vector<Point> p, int n, COLORREF c) {
    /*
    (X0,Y0)<--p1        (X1,Y0)<--p2

    (X0,Y1)<--p4        (X1,Y1)<--p3
    */
    Point p1 = p[0];
    Point p2 = Point(p[1].x, p[0].y);
    Point p3 = p[1];
    Point p4 = Point(p[0].x, p[1].y);
    p.push_back(p2);
    p.push_back(p4);
    // Sort by X, then Y
    sort(p.begin(), p.end(), [](Point a, Point b) {
        if (a.x == b.x) return a.y < b.y;
        return a.x < b.x;
        });

    int left = p[0].x;
    int right = p[3].x;

    int minY = min(p[0].y, p[1].y, p[2].y, p[3].y);
    int maxY = max(p[0].y, p[1].y, p[2].y, p[3].y);

    for (int x = left; x <= right; x++) {
        Point start = { x, minY };
        Point end = { x, maxY };

        // Example control points to create vertical wave
        Point ctrl1 = { x - 10, minY + (maxY - minY) / 3 };
        Point ctrl2 = { x + 10, minY + 2 * (maxY - minY) / 3 };

        DrawBezierCurve(hdc, start, ctrl1, ctrl2, end, c);
    }
}



void filleHermiteSq(HDC hdc, vector<Point> p, int n, COLORREF c) {

    Point p1 = p[0], p3 = p[1];

    if (p1.y > p3.y) swap(p1, p3);

    // Get the diagonal length and square side
    double diag = distance(p1, p3);
    int length = round(diag / sqrt(2));  

    // Determine direction
    int dx = (p3.x - p1.x >= 0) ? 1 : -1;
    int dy = (p3.y - p1.y >= 0) ? 1 : -1;

    Point p2 = Point(p1.x + dx * length, p1.y);
    Point p4 = Point(p1.x, p1.y + dy * length);
    p3 = Point(p1.x + dx * length, p1.y + dy * length); 

    p.clear();
    p.push_back(p2);
    p.push_back(p1);
    p.push_back(p3);
    p.push_back(p4);

    // Sort points: by X then Y
    sort(p.begin(), p.end(), [](Point a, Point b) {
        if (a.x == b.x) return a.y < b.y;
        return a.x < b.x;
        });


    int startY = min(p[0].y, p[1].y);
    int endY = max(p[2].y, p[3].y);
    int startX = p[0].x;
    int endX = p[2].x;

    if (startY > endY) swap(startY, endY);
    if (startX > endX) swap(startX, endX);

    for (int y = startY; y <= endY; ++y) {
        DrawHermiteCurve(hdc, startX, y, 0, 0, endX, y, 0, 0, 500, c);
    }
}
