#pragma once
#include "windows.h" 
#include "vector"
#include "../utils.h"
using namespace std;

vector<int> vectorMatrixMultiplication(int hmatrix[4][4], vector<int> g);
void DrawHermiteCurve(HDC hdc, int x1, int y1, int u1, int v1, int x2, int y2, int u2, int v2, int numpts, COLORREF c);
void CardinalSpline(HDC hdc,const vector<Point>& points, double controlParameter, int numpts, COLORREF c);
void DrawBezierCurve(HDC hdc, Point p1, Point p2, Point p3, Point p4, COLORREF c);