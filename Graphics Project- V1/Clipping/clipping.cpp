#include "clipping.h"
#include "../Line/lines.h"

void Draw_window(HDC hdc, Window w, COLORREF c) {
	DrawLineDDA(hdc, w.x_left, w.y_top, w.x_right, w.y_top, c);
	DrawLineDDA(hdc, w.x_left, w.y_top, w.x_left, w.y_bottom, c);
	DrawLineDDA(hdc, w.x_right, w.y_bottom, w.x_right, w.y_top, c);
	DrawLineDDA(hdc, w.x_right, w.y_bottom, w.x_left, w.y_bottom, c);
}

Point_out intersect(Point_out p1, Point_out p2, int x, int y) {
	if (x != -1) {		//intersection with vertical line x = ?
		if (p1.y == p2.y) {
			Point_out p(x, p1.y);
			return p;
		}

		double y = (double)p1.y + (double)(x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
		Point_out p(x, Round(y));
		return p;
	}
	else {				//intersection with horizontal line y = ?
		if (p1.x == p2.x) {
			Point_out p(p1.x, y);
			return p;
		}

		double x = (double)p1.x + (double)(y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
		Point_out p(Round(x), y);
		return p;
	}
}

void findPos(Point_out& p, Window w) {
	p.out = 0;

	if (p.x < w.x_left) {
		p.out |= 8;
	}
	if (p.x > w.x_right) {
		p.out |= 4;
	}
	if (p.y < w.y_top) {
		p.out |= 2;
	}
	if (p.y > w.y_bottom) {
		p.out |= 1;
	}
}

void point_clip(HDC hdc, Window w, int x, int y, COLORREF c) {
	if ((x > w.x_left) && (x < w.x_right) && (y > w.y_top) && (y < w.y_bottom)) {
		SetPixel(hdc, x, y, c);
	}
}

void line_clip(HDC hdc, Window w, Point_out p1, Point_out p2, COLORREF c) {
	findPos(p1, w);
	findPos(p2, w);

	while (true) {
		if (p1.out == 0 && p2.out == 0) {
			DrawLineDDA(hdc, p1.x, p1.y, p2.x, p2.y, c);
			return;
		}

		if ((p1.out & p2.out) != 0) {
			return;
		}

		if (p1.out) {
			if (p1.out & 8) {		//in left
				p1 = intersect(p1, p2, w.x_left, -1);
			}
			else if (p1.out & 4) {  //in right
				p1 = intersect(p1, p2, w.x_right, -1);
			}
			else if (p1.out & 2) {  //in top
				p1 = intersect(p1, p2, -1, w.y_top);
			}
			else {					//in bottom
				p1 = intersect(p1, p2, -1, w.y_bottom);
			}
			findPos(p1, w);
		}
		else {
			if (p2.out & 8) {		//in left
				p2 = intersect(p1, p2, w.x_left, -1);
			}
			else if (p2.out & 4) {  //in right
				p2 = intersect(p1, p2, w.x_right, -1);
			}
			else if (p2.out & 2) {  //in top
				p2 = intersect(p1, p2, -1, w.y_top);
			}
			else {					//in bottom
				p2 = intersect(p1, p2, -1, w.y_bottom);
			}
			findPos(p2, w);
		}
	}
}

vector<Point_out> edge_clip(vector<Point_out> pv, int edge, int edgeValue) {
	Point_out v1 = pv[pv.size() - 1];
	bool in1 = (v1.out & edge) == 0;
	vector <Point_out> output;

	for (int i = 0; i < pv.size(); i++) {
		Point_out v2 = pv[i];
		bool in2 = (v2.out & edge) == 0;

		if (in1 && in2) {
			output.push_back(v2);
		}
		else if (in2) {
			if (edge == 8 || edge == 4) {		//left or right
				output.push_back(intersect(v1, v2, edgeValue, -1));
			}
			else {								//top or bottom
				output.push_back(intersect(v1, v2, -1, edgeValue));
			}

			output.push_back(v2);
		}
		else if (in1) {
			if (edge == 8 || edge == 4) {		//left or right
				output.push_back(intersect(v1, v2, edgeValue, -1));
			}
			else {								//top or bottom
				output.push_back(intersect(v1, v2, -1, edgeValue));
			}
		}
		v1 = v2;
		in1 = in2;
	}
	return output;
}

void polygon_clip(HDC hdc, Window w, vector<Point_out> pv, COLORREF c) {
	int x1, y1, x2, y2;
	for (int i = 0; i < pv.size(); i++) {
		findPos(pv[i], w);
	}

	vector<Point_out> left_clipped = edge_clip(pv, 8, w.x_left);
	if (left_clipped.empty()) return;
	for (int i = 0; i < left_clipped.size(); i++) {
		findPos(left_clipped[i], w);
	}

	vector<Point_out> top_clipped = edge_clip(left_clipped, 2, w.y_top);
	if (top_clipped.empty()) return;
	for (int i = 0; i < top_clipped.size(); i++) {
		findPos(top_clipped[i], w);
	}

	vector<Point_out> right_clipped = edge_clip(top_clipped, 4, w.x_right);
	if (right_clipped.empty()) return;
	for (int i = 0; i < right_clipped.size(); i++) {
		findPos(right_clipped[i], w);
	}

	vector<Point_out> final = edge_clip(right_clipped, 1, w.y_bottom);
	if (final.empty()) return;
	for (int i = 0; i < final.size(); i++) {
		findPos(final[i], w);
	}

	for (int i = 0; i < final.size(); i++) {
		x1 = final[i].x;
		y1 = final[i].y;
		if (i == final.size() - 1) {
			x2 = final[0].x;
			y2 = final[0].y;
		}
		else {
			x2 = final[i + 1].x;
			y2 = final[i + 1].y;
		}

		DrawLineDDA(hdc, x1, y1, x2, y2, c);
	}
}
