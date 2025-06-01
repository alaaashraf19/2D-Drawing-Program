#include "curves.h"
#include "vector"
using namespace std;

vector<int> vectorMatrixMultiplication(int hmatrix[4][4], vector<int> g) {
    vector<int> c(4);
    int tmpSum = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {//matrix loop
            tmpSum += hmatrix[i][j] * g[j];
        }
        c[i] = tmpSum;
        tmpSum = 0;
    }
    return c;
}



void DrawHermiteCurve(HDC hdc, int x1, int y1, int u1, int v1, int x2, int y2, int u2, int v2, int numpts, COLORREF c) {

    int Hmatrix[4][4] = { {2,1,-2,1},{-3,-2,3,-1},{0,1,0,0},{1,0,0,0} };
    vector<int> gX = { x1,u1,x2,u2 };
    vector<int> gY = { y1,v1,y2,v2 };
    vector<int> cX = vectorMatrixMultiplication(Hmatrix, gX);
    vector<int> cy = vectorMatrixMultiplication(Hmatrix, gY);

    double step = 1.0 / numpts;
    double x, y;
    for (double t = 0.0; t <= 1; t += step) {
        x = cX[0] * pow(t, 3) + cX[1] * pow(t, 2) + cX[2] * t + cX[3];
        y = cy[0] * pow(t, 3) + cy[1] * pow(t, 2) + cy[2] * t + cy[3];
        SetPixel(hdc, Round(x), Round(y), c);
    }

}

void DrawBezierCurve(HDC hdc, Point p1, Point p2, Point p3, Point p4, COLORREF c) {

    int bezier_matrix[4][4] = { {-1,3,-3,1},{3,-6,3,0},{-3,3,0,0},{1,0,0,0} };
    vector <int> x_vec = { p1.x, p2.x, p3.x, p4.x };
    vector <int> y_vec = { p1.y, p2.y, p3.y, p4.y };

    vector <int> x_param = vectorMatrixMultiplication(bezier_matrix, x_vec);
    vector <int> y_param = vectorMatrixMultiplication(bezier_matrix, y_vec);

    double step = 1.0 / max(abs(p4.x - p1.x), abs(p4.y - p1.y));
    double x, y;
    for (double t = 0.0; t <= 1; t += step) {
        x = x_param[0] * pow(t, 3) + x_param[1] * pow(t, 2) + x_param[2] * t + x_param[3];
        y = y_param[0] * pow(t, 3) + y_param[1] * pow(t, 2) + y_param[2] * t + y_param[3];
        SetPixel(hdc, Round(x), Round(y), c);
    }

    //another version of bezier
    //for (float t = 0; t <= 1; t += 0.001) {
    //    float u = 1 - t;

    //    float x = u * u * u * p1.x + 3 * u * u * t * p2.x + 3 * u * t * t * p3.x + t * t * t * p4.x;
    //    float y = u * u * u * p1.y + 3 * u * u * t * p2.y + 3 * u * t * t * p3.y + t * t * t * p4.y;


    //    SetPixel(hdc, round(x), round(y), c);
    //}

}

void CardinalSpline(HDC hdc, const vector<Point>& points, double controlParameter, int numpts, COLORREF c) {
    int q_x1, q_y1, q_x2, q_y2;
    double t = (1 - controlParameter) / 2.0;
    for (int i = 1; i < points.size() - 2; i++) {

        q_x1 = Round((points[i + 1].x - points[i - 1].x) * t);
        q_y1 = Round((points[i + 1].y - points[i - 1].y) * t);

        q_x2 = Round((points[i + 2].x - points[i].x) * t);
        q_y2 = Round((points[i + 2].y - points[i].y) * t);

        DrawHermiteCurve(hdc, points[i].x, points[i].y, q_x1, q_y1, points[i + 1].x, points[i + 1].y, q_x2, q_y2, numpts, c);

    }
}
