#include "Circle.h"

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
}

void DrawCircle(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c) {
	double dxsq = pow(xc - x1, 2);
	double dysq = pow(yc - y1, 2);
	double r = sqrt(dxsq + dysq);
	int x = 0, y = r;
	Draw8Points(hdc, xc, yc, x, y, c);
	while (x < y) {
		x++;
		y = Round(sqrt(pow(r, 2) - pow(x, 2)));
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}

void DrawCirclePolar(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c) {
	double dxsq = pow(xc - x1, 2);
	double dysq = pow(yc - y1, 2);
	double r = sqrt(dxsq + dysq);
	int x = 0, y = 0;
	for (double theta = 0; theta < 3.14 / 2; theta += 0.001) {
		x = Round(r * cos(theta));
		y = Round(r * sin(theta));
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}

void DrawCirclePolarIter(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c) {
	double dxsq = pow(xc - x1, 2);
	double dysq = pow(yc - y1, 2);
	double r = sqrt(dxsq + dysq);
	double x = r, y = 0, dTheta = 1.0 / r;
	Draw8Points(hdc, xc, yc, Round(x), Round(y), c);

	double cos_dTheta = cos(dTheta), sin_dTheta = sin(dTheta);
	while (x > y) {
		double new_x = x * cos_dTheta - y * sin_dTheta;
		double new_y = x * sin_dTheta + y * cos_dTheta;

		x = new_x;
		y = new_y;
		Draw8Points(hdc, xc, yc, Round(x), Round(y), c);
	}
}

void DrawCircleMidPoint1(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c) {
	double dxsq = pow(xc - x1, 2);
	double dysq = pow(yc - y1, 2);
	double r = sqrt(dxsq + dysq);
	int x = 0, y = r, d = 1 - r;
	Draw8Points(hdc, xc, yc, x, y, c);
	while (x < y) {
		if (d < 0) {
			d += 2 * x + 3;
		}
		else {
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}

void DrawCircleMidPoint2(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c) {
	double dxsq = pow(xc - x1, 2);
	double dysq = pow(yc - y1, 2);
	double r = sqrt(dxsq + dysq);

	int x = 0, y = r, d = 1 - r;
	int d1 = 3, d2 = 5 - 2 * r;
	Draw8Points(hdc, xc, yc, x, y, c);
	while (x < y) {
		if (d < 0) {
			d += d1;
		}
		else {
			d += d2;
			d2 += 2;
			y--;
		}

		d1 += 2;
		d2 += 2;
		x++;
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}

//void fillQrtr(HDC hdc, int xc, int yc, int r, COLORREF c) {
//	int x = 0, y = r, d = 1 - r;
//	int d1 = 3, d2 = 5 - 2 * r;
//	while (x < y) {
//		if (d < 0) {
//			d += d1;
//		}
//		else {
//			d += d2;
//			d2 += 2;
//			y--;
//		}
//
//		d1 += 2;
//		d2 += 2;
//		x++;
//		DrawLineDDA(hdc, xc, yc, xc + x, yc - y, c);
//		DrawLineDDA(hdc, xc, yc, xc - x, yc + y, c);
//	}
//}
