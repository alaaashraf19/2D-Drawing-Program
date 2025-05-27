#pragma once
#include <windows.h>  
#include "../utils.h"
#include "vector"
using namespace std;


void Ellipse_Direct(HDC hdc, vector<Point>& v, COLORREF c);
void Ellipse_polar(HDC hdc, vector<Point>& v, COLORREF c);
void Ellipse_Midpoint(HDC hdc, vector<Point>& v, COLORREF c);