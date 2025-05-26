#pragma once
#include <windows.h>  
#include <cmath>
#include <algorithm>
#include "../utils.h"

int Round(double x);
void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
void DrawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
void DrawLineParametric(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
