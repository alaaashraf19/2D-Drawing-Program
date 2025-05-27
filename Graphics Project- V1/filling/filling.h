#pragma once
#include <windows.h>  
#include <cmath>
#include <algorithm>
#include <stack>
#include "vector"
#include "../utils.h"
#include "../Line/lines.h"
using namespace std;
void RecFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);
void NonRecFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);

///Convex Filling
void ConvexFill(HDC hdc, std::vector<Point> v, int n, COLORREF c);