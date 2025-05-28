#include "filling.h"
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
