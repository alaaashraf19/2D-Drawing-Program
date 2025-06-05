#include "Circle.h"
#include "../Line/lines.h"


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

void fillQrtr_line(HDC hdc, int xc, int yc, int x1, int y1, int x2, int y2, COLORREF c) {
	double dxsq = pow(xc - x1, 2);
	double dysq = pow(yc - y1, 2);
	double r = sqrt(dxsq + dysq);

	int x = 0, y = r, d = 1 - r;
	int d1 = 3, d2 = 5 - 2 * r;
	DrawCircleMidPoint2(hdc, xc, yc, x1, y1, c);

	int pos;
	if (x2 >= xc && y2 >= yc) pos = 0;
	else if (x2 < xc && y2 >= yc) pos = 1;
	else if (x2 < xc && y2 < yc) pos = 2;
	else pos = 3;

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

		switch (pos) {
		case 0:
			DrawLineDDA(hdc, xc, yc, xc + x, yc + y, c);
			DrawLineDDA(hdc, xc, yc, xc + y, yc + x, c);
			break;
		case 1:
			DrawLineDDA(hdc, xc, yc, xc - x, yc + y, c);
			DrawLineDDA(hdc, xc, yc, xc - y, yc + x, c);
			break;
		case 2:
			DrawLineDDA(hdc, xc, yc, xc - x, yc - y, c);
			DrawLineDDA(hdc, xc, yc, xc - y, yc - x, c);
			break;
		case 3:
			DrawLineDDA(hdc, xc, yc, xc + x, yc - y, c);
			DrawLineDDA(hdc, xc, yc, xc + y, yc - x, c);
			break;
		}
	}
}

void fillQrtr_circle(HDC hdc, int xc, int yc, int x1, int y1, int x2, int y2, COLORREF c) {
	double dxsq = pow(xc - x1, 2);
	double dysq = pow(yc - y1, 2);
	double r = sqrt(dxsq + dysq);

	int x, y, d;
	int d1, d2;
	DrawCircleMidPoint2(hdc, xc, yc, x1, y1, c);

	int pos;
	if (x2 >= xc && y2 >= yc) pos = 0;
	else if (x2 < xc && y2 >= yc) pos = 1;
	else if (x2 < xc && y2 < yc) pos = 2;
	else pos = 3;

	while (r > 0) {
		x = 0;
		y = r;
		d = 1 - r;
		d1 = 3;
		d2 = 5 - 2 * r;
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
			
			switch (pos) {
			case 0:
				SetPixel(hdc, xc + x, yc + y, c);
				SetPixel(hdc, xc + y, yc + x, c);
				break;
			case 1:
				SetPixel(hdc, xc - x, yc + y, c);
				SetPixel(hdc, xc - y, yc + x, c);
				break;
			case 2:
				SetPixel(hdc, xc - x, yc - y, c);
				SetPixel(hdc, xc - y, yc - x, c);
				break;
			case 3:
				SetPixel(hdc, xc + x, yc - y, c);
				SetPixel(hdc, xc + y, yc - x, c);
				break;
			}
		}
		r-=3;	//decrease R each rotation
	}
}
