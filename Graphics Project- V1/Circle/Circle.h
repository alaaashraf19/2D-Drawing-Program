#pragma once

#include "windows.h" 
#include "../utils.h"

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c);
void DrawCircle(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCirclePolar(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCirclePolarIter(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCircleMidPoint1(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);
void DrawCircleMidPoint2(HDC hdc, int xc, int yc, int x1, int y1, COLORREF c);

void fillQrtr_line(HDC hdc, int xc, int yc, int x1, int y1, int x2, int y2, COLORREF c);
void fillQrtr_circle(HDC hdc, int xc, int yc, int x1, int y1, int x2, int y2, COLORREF c);
