#pragma once

#include "windows.h" 
#include "vector"
#include "../utils.h"
using namespace std;

struct Point_out {
	int x, y, out;
	Point_out(int x1, int y1) :x(x1), y(y1) { out = 0; }
	Point_out(int x1, int y1, int o) : x(x1), y(y1), out(o) {}
};

struct Window {
	//8 = left / 4 = right / 2 = top / 1 = bottom
	int x_left, x_right, y_top, y_bottom;
	Window(int w) {
		if (w == 0) {		//rectangle	
			x_left = 200;
			x_right = 1100;
			y_top = 100;
			y_bottom = 550;
		}
		else {				//square
			x_left = 425;
			x_right = 875;
			y_top = 100;
			y_bottom = 550;
		}
	}
	Window(int xl, int xr, int yt, int yb) :x_left(xl), x_right(xr), y_top(yt), y_bottom(yb) {}
};

void Draw_window(HDC hdc, Window w, COLORREF c);
Point_out intersect(Point_out p1, Point_out p2, int x, int y);
void findPos(Point_out& p, Window w);
void point_clip(HDC hdc, Window w, int x, int y, COLORREF c);
void line_clip(HDC hdc, Window w, Point_out p1, Point_out p2, COLORREF c);
vector<Point_out> edge_clip(vector<Point_out> pv, int edge, int edgeValue);
void polygon_clip(HDC hdc, Window w, vector<Point_out> pv, COLORREF c);