#pragma once

#include "windows.h" 
#include "../utils.h"

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c);
void DrawCircle(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCirclePolar(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCirclePolarIter(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCircleMidPoint1(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCircleMidPoint2(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);

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
