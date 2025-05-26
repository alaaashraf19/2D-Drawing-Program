#pragma once
#include <windows.h>  
#include <cmath>
#include <algorithm>
#include <stack>
#include "../utils.h"


void RecFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);
void NonRecFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);