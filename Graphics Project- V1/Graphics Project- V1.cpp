// Graphics Project- V1.cpp : Defines the entry point for the application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Graphics Project- V1.h"
#include "Line/lines.h"
#include "Circle/Circle.h"
#include "filling/filling.h"
#include "Curves/curves.h"
#include "Ellipse/Ellipse.h"
#include "Clipping/clipping.h"
#include "utils.h"
#include "vector"
#include <stdio.h>
#include <commdlg.h>  
#include <windows.h>
#include <iostream>
#include <fstream>

#define MAX_LOADSTRING 100

#define Draw_Line_DDA 211
#define Draw_Line_Bres 212
#define Draw_Line_Param 213

#define Non_Rec_Flood_Fill 214
#define Rec_Flood_Fill 215

#define Convex_Fill 216
#define Non_Convex_Fill 221


#define Draw_Cardinal_Spline 217
#define ellipse_direct 218
#define ellipse_polar 219
#define ellipse_midpoint 220

#define Choose_Color 222
#define Choose_fill_Color 300

#define clip_point_rec 223
#define clip_point_square 224
#define clip_line_rec 225
#define clip_line_square 226
#define clip_polygon 227

#define circle_direct 228
#define circle_polar 229
#define circle_polar_iter 230
#define circle_midpoint 231
#define circle_modified_mid 232

#define fill_circle_line 233
#define fill_circle_circles 234

#define fill_square_hermite 235
#define fill_rectangle_bezier 236

#define SAVE_DC 11
#define RESTORE_DC 12

#define Set_Arrow_Cursor 41
#define Set_Cross_Cursor 42
#define Set_Ibeam_Cursor 43

#define Set_Bkg_Light 31
#define Set_Bkg_Dark 32

#define LOAD 33
#define CLEAR_SCREEN 1001
using namespace std;

HCURSOR ARROW = LoadCursor(nullptr, IDC_ARROW);
HCURSOR CROSS = LoadCursor(nullptr, IDC_CROSS);
HCURSOR IBEAM = LoadCursor(nullptr, IDC_IBEAM);
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void AttachConsoleWindow() {
    AllocConsole(); // Allocate a new console
    freopen("CONOUT$", "w", stdout); // Redirect stdout
    freopen("CONOUT$", "w", stderr); // Redirect stderr
    freopen("CONIN$", "r", stdin);   // Redirect stdin
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
   
    /*vector<Point>& pv;
  
    while (true) {
            cout << "Welcome To the 2D Drawing Program\n Choose an option\n1- Draw\n2- Fill\n3-Clip\n 0-quit";
        int n;
        cin >> n;
        if (n == 0)break;
        else {

        }
    }*/



    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHICSPROJECTV1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICSPROJECTV1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICSPROJECTV1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GRAPHICSPROJECTV1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
bool SaveBitmapToFile(HBITMAP hBitmap, HDC hDC, LPCWSTR filename)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;
    FILE* fp = nullptr;

    if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp))
        return false;

    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    pbmi->bmiHeader.biCompression = BI_RGB;
    pbmi->bmiHeader.biSizeImage = 0;
    pbmi->bmiHeader.biXPelsPerMeter = 0;
    pbmi->bmiHeader.biYPelsPerMeter = 0;
    pbmi->bmiHeader.biClrUsed = 0;
    pbmi->bmiHeader.biClrImportant = 0;

    // Create file for writing
    errno_t err = _wfopen_s(&fp, filename, L"wb");
    if (err != 0)
        return false;

    BITMAPFILEHEADER hdr;
    DWORD dwTotalBitsSize = pbmi->bmiHeader.biSizeImage;

    if (dwTotalBitsSize == 0)
        dwTotalBitsSize = ((bmp.bmWidth * cClrBits + 31) / 32) * 4 * bmp.bmHeight;

    hdr.bfType = 0x4D42;  // 'BM'
    hdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwTotalBitsSize;
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Write headers
    fwrite(&hdr, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&pbmi->bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    // Allocate memory for bitmap bits
    BYTE* lpBits = new BYTE[dwTotalBitsSize];

    // Get bitmap bits
    if (!GetDIBits(hDC, hBitmap, 0, (UINT)bmp.bmHeight, lpBits, pbmi, DIB_RGB_COLORS)) {
        delete[] lpBits;
        fclose(fp);
        return false;
    }

    // Write bitmap bits to file
    fwrite(lpBits, 1, dwTotalBitsSize, fp);

    delete[] lpBits;
    fclose(fp);
    LocalFree(pbmi);

    return true;
}
///////////////////////////////////////////////////////////


COLORREF PickColor(HWND hwndParent) {
    CHOOSECOLOR cc;                 // common dialog box structure
    static COLORREF customColors[16]; 

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwndParent;
    cc.lpCustColors = customColors;
    cc.rgbResult = RGB(255, 0, 0);  // default color (red)
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        return cc.rgbResult;  
    }
    else {
        return RGB(0, 0, 0);  //user cancelled,returning color (black)
    }
}

/////////////////////////////////////////////////////////

int takeInput() {
    int n;
    cout << "Enter number of points: ";
    cin >> n;
    return n;
}

void Add_Theme_Menu(HWND);
HMENU MainMenu;
HCURSOR currentCursor = LoadCursor(NULL, IDC_ARROW);

int Last_Saved_DC;
int Required_clicks = 0;


enum Algorithm {
    NONE,
    LINE_DDA,
    LINE_BRES,
    LINE_PARAM,
    CIRCLE_DIRECT,
    CIRCLE_POLAR,
    CIRCLE_POLAR_ITER,
    CIRCLE_MIDPOINT,
    CIRCLE_MODIFIED_MID,
    FILL_CIRCLE_LINE,
    FILL_SQUARE_HERMITE,
    FILL_RECTANGLE_BEZIER,
    FILL_CIRCLE_CIRCLES,
    CARDINAL_SPLINE,
    CONVEX_FILL,
    NON_CONVEX_FILL,
    ELLIPSE_DIRECT,
    ELLIPSE_POLAR,
    ELLIPSE_MIDPOINT,
    CLIP_POINT_REC,
    CLIP_POINT_SQUARE,
    CLIP_LINE_REC,
    CLIP_LINE_SQUARE,
    CLIP_POLYGON,
    DRAW_REC_WINDOW,
    DRAW_SQUARE_WINDOW,
    NON_REC_FLOOD_FILL,
    REC_FLOOD_FILL,
};

class input_requirements_base {
public:
    int req_pts;
    vector<Point> pv;
    Algorithm alg;
    COLORREF c;
    input_requirements_base(Algorithm algorithm, int pts, COLORREF color) : alg(algorithm), req_pts(pts), c(color) {
        pv.reserve(pts);  
    }
    virtual input_requirements_base* clone() const = 0;
    virtual void run(HDC hdc) = 0;
    virtual ~input_requirements_base() {}
};

template<typename T>
class input_requirements : public input_requirements_base {
public:
    T instance;

    input_requirements(Algorithm algorithm, int pts, COLORREF c) : input_requirements_base(algorithm, pts,c) {}

    input_requirements_base* clone() const override {
        return new input_requirements<T>(*this);  // copy construct
    }
    void run(HDC hdc) {
        instance.run(hdc, pv, c);
    }
};

//class shape {
//public:
//    virtual void run(HDC hdc)=0;
//    virtual ~shape() {}
//};
//
//template<typename T>
//class shapewrapper : public shape {
//public:
//    input_requirements<T> req;
//    shapewrapper(const input_requirements<T> & inp) : req(inp) {}
//    void run (HDC hdc) override {
//        req.run(hdc);
//    }
//};


//struct LastShape {
//    int pts;
//    vector<Point> points;
//    COLORREF c;
//    Algorithm alg;
//    LastShape(const vector<Point>& p, COLORREF C, Algorithm a, int pt): points(p), c(C), alg(a), pts(pt)  {}
//};

class DDA_LINE {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawLineDDA(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class BRES_LINE {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawLineBresenham(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class Param_LINE {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawLineParametric(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class Circle_Direct {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawCircle(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class Circle_Polar {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawCirclePolar(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class Circle_Polar_Iter {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawCirclePolarIter(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class Circle_Midpoint {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawCircleMidPoint1(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class Circle_Modified_Mid {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        DrawCircleMidPoint1(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, c);
    }
};

class Fill_Circle_Lines {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        fillQrtr_line(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, pv[2].x, pv[2].y, c);
    }
};

class Fill_Circle_Circles {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        fillQrtr_circle(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, pv[2].x, pv[2].y, c);
    }
};

class Cardinal_Spline {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        CardinalSpline(hdc, pv, 0.0, 1000, c);
    }
};

class Convex_fill {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        ConvexFill(hdc, pv, pv.size(), c);
    }
};

class Non_Convex_fill {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        NonConvexFill(hdc, pv, pv.size(), c);
    }
};

class ellipse_Direct {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        Ellipse_Direct(hdc, pv , c);
    }
};

class ellipse_Polar {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        Ellipse_polar(hdc, pv, c);
    }
};

class ellipse_Midpoint {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        Ellipse_Midpoint(hdc, pv , c);
    }
}; 

class Clip_Point_rec {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        point_clip(hdc, Window(0), pv[0].x, pv[0].y, c);
    }
}; 

class Clip_Point_square {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        point_clip(hdc, Window(1), pv[0].x, pv[0].y, c);
    }
}; 

class Clip_Line_rec {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        line_clip(hdc, Window(0), Point_out(pv[0].x, pv[0].y), Point_out(pv[1].x, pv[1].y), c);
    }
}; 

class Clip_Line_square {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        line_clip(hdc, Window(1), Point_out(pv[0].x, pv[0].y), Point_out(pv[1].x, pv[1].y), c);
    }
}; 

class Clip_Polygon {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        vector <Point_out> pv2;
        for (int i = 0; i < pv.size(); i++) {
            pv2.push_back(Point_out(pv[i].x, pv[i].y));
        }
        polygon_clip(hdc, Window(0), pv2, c);
    }
}; 

class Draw_Rectangle_Window {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        Draw_window(hdc, Window(0), c);
    }
}; 

class Draw_Square_Window {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        Draw_window(hdc, Window(1), c);
    }
}; 

class Fill_Square_Hermite {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        //Point p[] = { pv[0],pv[1]};
        filleHermiteSq(hdc,pv, 4, c);

    }
};

class Fill_Rectangle_Bezier {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        //Point p[] = { pv[0],pv[1],pv[2],pv[3] };
        fillRecBezier(hdc, pv, 4, c);

    };
};

class NonRec_Flood_fill {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {   

        NonRecFloodFill(hdc, pv[0].x, pv[0].y, c);

    };
};
class Rec_Flood_fill {
public:
    void run(HDC hdc, vector<Point>& pv, COLORREF c) {
        COLORREF bg = GetPixel(hdc, pv[0].x, pv[0].y);
        if(bg!= c) RecFloodFill(hdc, pv[0].x, pv[0].y, bg ,c);

    };
};

input_requirements_base* current_input_req = nullptr;

Algorithm chosen_algo = NONE;
HBITMAP g_hLoadedBitmap = nullptr;
COLORREF chosen_color;
int xg, yg;
vector<input_requirements_base*> shapes;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    AttachConsoleWindow();
    HDC hdc;
    hdc = GetDC(hWnd);
    switch (message)
    {

    case WM_CREATE:
    {
        //AttachConsoleWindow();
        Add_Theme_Menu(hWnd);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case SAVE_DC:
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            HDC hdcWindow = GetDC(hWnd);
            HDC hdcMem = CreateCompatibleDC(hdcWindow);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, rc.right - rc.left, rc.bottom - rc.top);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

            // Copy screen contents to memory DC
            BitBlt(hdcMem, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcWindow, 0, 0, SRCCOPY);

            // Save bitmap to file
            if (SaveBitmapToFile(hBitmap, hdcMem, L"saved_screen.bmp")) {
                MessageBox(hWnd, L"Screen saved successfully.", L"Save", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Failed to save screen.", L"Save Error", MB_OK | MB_ICONERROR);
            }

            // Cleanup
            SelectObject(hdcMem, hOldBitmap);
            DeleteObject(hBitmap);
            DeleteDC(hdcMem);
            ReleaseDC(hWnd, hdcWindow);

        }
        break;

        case RESTORE_DC:
        {
            hdc = GetDC(hWnd);
            for (auto shape : shapes) shape->run(hdc);
            ReleaseDC(hWnd, hdc);
        }
        break;
        case LOAD:
        {
            //hdc = GetDC(hWnd);
            //for (auto shape : all_drawn_shapes) shape->run(hdc);
            if (g_hLoadedBitmap) {
                DeleteObject(g_hLoadedBitmap);
                g_hLoadedBitmap = nullptr;
            }

            g_hLoadedBitmap = (HBITMAP)LoadImageW(
                nullptr,
                L"saved_screen.bmp",
                IMAGE_BITMAP,
                0, 0,
                LR_LOADFROMFILE
            );

            if (!g_hLoadedBitmap) {
                MessageBox(hWnd, L"Failed to load bitmap.", L"Load Error", MB_OK | MB_ICONERROR);
            }
            else {
                InvalidateRect(hWnd, nullptr, TRUE);  // Trigger WM_PAINT to redraw window with new bitmap
            }
        }
        break;

        case CLEAR_SCREEN:
        {
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case Set_Bkg_Light:
        {
            HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
            SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case Set_Bkg_Dark:
        {
            HBRUSH brush = CreateSolidBrush(RGB(50, 50, 50));
            SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        case Set_Arrow_Cursor:
        {
            currentCursor = LoadCursor(NULL, IDC_ARROW);
            SetCursor(currentCursor);
        }          
        break;

        case Set_Cross_Cursor:
        {
            currentCursor = LoadCursor(NULL, IDC_CROSS);
            SetCursor(currentCursor);
        }         
        break;

        case Set_Ibeam_Cursor:
        {
            currentCursor = LoadCursor(NULL, IDC_IBEAM);
            SetCursor(currentCursor);
        }
        break;

        case Draw_Line_DDA:
        {
            
            chosen_algo = LINE_DDA;
            cout << "Drawing a line with DDA.\nDraw 2 Points.\n";
            current_input_req = new input_requirements<DDA_LINE>(chosen_algo, 2, chosen_color);
        }
        break;
        case Draw_Line_Bres:
        {
            chosen_algo = LINE_BRES;
            cout << "Drawing a line with bresenham.\nDraw 2 Points.\n";
            current_input_req = new input_requirements<BRES_LINE>(chosen_algo, 2, chosen_color);
        }
        break;
        case Draw_Line_Param:
        {
            chosen_algo = LINE_PARAM;
            cout << "Drawing a line with parametric.\nDraw 2 Points.\n";
            current_input_req = new input_requirements<Param_LINE>(chosen_algo, 2, chosen_color);

        }
        break;
        case circle_direct:
        {
            chosen_algo = CIRCLE_DIRECT;
            cout << "Drawing a Circle with the Direct Method.\nDraw 1 Point for circle, and 1 point for R.\n";
            current_input_req = new input_requirements<Circle_Direct>(chosen_algo, 2, chosen_color);
        }
        break;
        case circle_polar:
        {
            chosen_algo = CIRCLE_POLAR;
            cout << "Drawing a Circle with the Polar Method.\nDraw 1 Point for circle, and 1 point for R.\n";
            current_input_req = new input_requirements<Circle_Polar>(chosen_algo, 2, chosen_color);
        }
        break;
        case circle_polar_iter:
        {
            chosen_algo = CIRCLE_POLAR_ITER;
            cout << "Drawing a Circle with the Polar Iterative Method.\nDraw 1 Point for circle, and 1 point for R.\n";
            current_input_req = new input_requirements<Circle_Polar_Iter>(chosen_algo, 2, chosen_color);
        }
        break;
        case circle_midpoint:
        {
            chosen_algo = CIRCLE_MIDPOINT;
            cout << "Drawing a Circle with the MidPoint Method.\nnDraw 1 Point for circle, and 1 point for R.\n";
            current_input_req = new input_requirements<Circle_Midpoint>(chosen_algo, 2, chosen_color);
        }
        break;
        case circle_modified_mid:
        {
            chosen_algo = CIRCLE_MODIFIED_MID;
            cout << "Drawing a Circle with the Modified MidPoint Method.\nnDraw 1 Point for circle, and 1 point for R.\n";
            current_input_req = new input_requirements<Circle_Modified_Mid>(chosen_algo, 2, chosen_color);
        }
        break;
        case fill_circle_line:
        {
            chosen_algo = FILL_CIRCLE_LINE;
            cout << "Filling a Circle with Lines.\nDraw 1 Point for center, 1 Point for R, and 1 point to represnt the quarter.\n";
            current_input_req = new input_requirements<Fill_Circle_Lines>(chosen_algo, 3, chosen_color);
        }
            break;
        case fill_circle_circles:
        {
            chosen_algo = FILL_CIRCLE_CIRCLES;
            cout << "Filling a Circle with Circles\nDraw 1 Point for center, 1 Point for R, and 1 point to represnt the quarter.\n";
            current_input_req = new input_requirements<Fill_Circle_Circles>(chosen_algo, 3, chosen_color);
        }
            break;
        case fill_square_hermite:
        {
            chosen_algo = FILL_SQUARE_HERMITE;
            cout << "Filling a Sqaure with Hermite Curves.\nDraw Top_Left and Bottom_Right Points for diagonal of the square.\n";
            current_input_req = new input_requirements<Fill_Square_Hermite>(chosen_algo, 2, chosen_color);
        }
        break;
        case fill_rectangle_bezier:
        {
            chosen_algo = FILL_RECTANGLE_BEZIER;
            cout << "Filling a rectangle with Hermite Curves.\nDraw Top_Left and Bottom_Right Points for diagonal of the rectangle.\n";
            current_input_req = new input_requirements<Fill_Rectangle_Bezier>(chosen_algo, 2, chosen_color);
        }
        break;
        case Non_Rec_Flood_Fill:
        {
            chosen_algo = NON_REC_FLOOD_FILL;
            cout << "Filling using Non Recursive FloodFill.\nDraw 1 Point inside 1 shape to fill.\n";
            current_input_req = new input_requirements<NonRec_Flood_fill>(chosen_algo, 1, chosen_color);
            cout << "Choose filling color:";
            chosen_color = PickColor(hWnd);
        }
        break;
        case Rec_Flood_Fill:
        {
            chosen_algo = REC_FLOOD_FILL;
            cout << "Filling using Recursive FloodFill.\nDraw 1 Point inside 1 shape to fill.\n";
            current_input_req = new input_requirements<Rec_Flood_fill>(chosen_algo, 1, chosen_color);
            cout << "Choose filling color:";
            chosen_color = PickColor(hWnd);
        }
        break;
        case Draw_Cardinal_Spline:
        {
            cout << "Drawing Cardinal Splines.\n";
            int n = takeInput();
            chosen_algo = CARDINAL_SPLINE;
            cout << "Draw " << n << " points for the spline.\n";
            current_input_req = new input_requirements<Cardinal_Spline>(chosen_algo, n, chosen_color);
        }
        break;
        case ellipse_direct:
        {
            chosen_algo = ELLIPSE_DIRECT;
            cout << "Drawing Ellipse with the direct method.\nDraw 2 Points for diagonal of the ellipse.\n";
            current_input_req = new input_requirements<ellipse_Direct>(chosen_algo, 2, chosen_color);

        }
        break;

        case ellipse_polar:
        {
            chosen_algo = ELLIPSE_POLAR;
            cout << "Drawing Ellipse with the polar method.\nDraw 2 Points for diagonal of the ellipse.\n";
            current_input_req = new input_requirements<ellipse_Polar>(chosen_algo, 2, chosen_color);
        }
        break;

        case ellipse_midpoint:
        {
            chosen_algo = ELLIPSE_MIDPOINT;
            cout << "Drawing Ellipse with the MidPoint method.\nDraw 2 Points for diagonal of the ellipse.\n";
            current_input_req = new input_requirements<ellipse_Midpoint>(chosen_algo, 2, chosen_color);
        }
        break;

        case Convex_Fill:
        {
            cout << "Filling using Convex Fill.\n";
            int n = takeInput();
            chosen_algo = CONVEX_FILL;
            cout << "Draw " << n << " Points for the polygon.\n";
            current_input_req = new input_requirements<Convex_fill>(chosen_algo, n, chosen_color);
        }
        break;

        case Non_Convex_Fill:
        {
            cout << "Filling using Non-Convex Fill.\n";
            int n = takeInput();
            chosen_algo = NON_CONVEX_FILL;
            cout << "Draw " << n << " Points for the polygon.";
            current_input_req = new input_requirements<Non_Convex_fill>(chosen_algo, n, chosen_color);
        }
        break;
        case clip_point_rec:
        {
            cout << "Point Clipping in rectangular window.\nChoose a color for the window:\n";
            COLORREF color = PickColor(hWnd);
            cout << "Color Chosen: " << (int)GetRValue(color) << " " << (int)GetGValue(color) << " " << (int)GetBValue(color) << "\n";

            input_requirements_base* temp = new 
                input_requirements<Draw_Rectangle_Window>(DRAW_REC_WINDOW, 0, color);
            temp->run(hdc);

            chosen_algo = CLIP_POINT_REC;
            cout << "Draw a point.\n";
            current_input_req = new input_requirements<Clip_Point_rec>(chosen_algo, 1, chosen_color);
        }
        break;
        case clip_point_square:
        {
            cout << "Point Clipping in sqaure window.\nChoose a color for the window:\n";
            COLORREF color = PickColor(hWnd);
            cout << "Color Chosen: " << (int)GetRValue(color) << " " << (int)GetGValue(color) << " " << (int)GetBValue(color) << "\n";

            input_requirements_base* temp = new 
                input_requirements<Draw_Square_Window>(DRAW_SQUARE_WINDOW, 0, color);
            temp->run(hdc);

            chosen_algo = CLIP_POINT_SQUARE;
            cout << "Draw a point.\n";
            current_input_req = new input_requirements<Clip_Point_square>(chosen_algo, 1, chosen_color);
        }
        break;
        case clip_line_rec:
        {
            cout << "Line Clipping in rectangular window.\nChoose a color for the window:\n";
            COLORREF color = PickColor(hWnd);
            cout << "Color Chosen: " << (int)GetRValue(color) << " " << (int)GetGValue(color) << " " << (int)GetBValue(color) << "\n";

            input_requirements_base* temp = new 
                input_requirements<Draw_Rectangle_Window>(DRAW_REC_WINDOW, 0, color);
            temp->run(hdc);

            chosen_algo = CLIP_LINE_REC;
            cout << "Draw 2 points to represent the line.\n";
            current_input_req = new input_requirements<Clip_Line_rec>(chosen_algo, 2, chosen_color);
        }
        break;
        case clip_line_square:
        {
            cout << "Line Clipping in square window.\nChoose a color for the window:\n";
            COLORREF color = PickColor(hWnd);
            cout << "Color Chosen: " << (int)GetRValue(color) << " " << (int)GetGValue(color) << " " << (int)GetBValue(color) << "\n";

            input_requirements_base* temp = new 
                input_requirements<Draw_Square_Window>(DRAW_SQUARE_WINDOW, 0, color);
            temp->run(hdc);

            chosen_algo = CLIP_LINE_SQUARE;
            cout << "Draw 2 points to represent the line.\n";
            current_input_req = new input_requirements<Clip_Line_square>(chosen_algo, 2, chosen_color);
        }
        break;
        case clip_polygon:
        {
            cout << "Polygon Clipping.\nChoose a color for the window:\n";
            COLORREF color = PickColor(hWnd);
            cout << "Color Chosen: " << (int)GetRValue(color) << " " << (int)GetGValue(color) << " " << (int)GetBValue(color) << "\n";

            input_requirements_base* temp = new 
                input_requirements<Draw_Rectangle_Window>(DRAW_REC_WINDOW, 0, color);
            temp->run(hdc);

            int n = takeInput();
            chosen_algo = CLIP_POLYGON;
            cout << "Draw " << n << " Points to represent the polygon.\n";
            current_input_req = new input_requirements<Clip_Polygon>(chosen_algo, n, chosen_color);
        }
        break;
        case Choose_Color:
        {
            cout << "pick a color:\n";
            chosen_color =PickColor(hWnd);
            cout << "Color Chosen: " << (int)GetRValue(chosen_color) << " " << (int)GetGValue(chosen_color) << " "<< (int)GetBValue(chosen_color) << "\n";
        }
        break;

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        xg = LOWORD(lParam);
        yg = HIWORD(lParam);
        if (chosen_algo != NONE && current_input_req) {
            if(chosen_algo != CLIP_POINT_REC && chosen_algo != CLIP_POINT_SQUARE && chosen_algo != NON_REC_FLOOD_FILL && chosen_algo != REC_FLOOD_FILL)
                SetPixel(hdc, xg, yg, chosen_color);
            current_input_req->pv.push_back(Point(xg, yg));
            if (current_input_req->pv.size() == current_input_req->req_pts) {
                current_input_req->c = chosen_color;
                shapes.push_back(current_input_req->clone());
                current_input_req->run(hdc);
                current_input_req->pv.clear();
                //delete current_input_req;
                //current_input_req = nullptr;
                //chosen_algo = NONE;
            }

        }
        else {
            SetPixel(hdc, xg, yg, chosen_color);
        }

        ReleaseDC(hWnd, hdc);

    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        
        if (g_hLoadedBitmap) {
            // Create a memory DC compatible with the window DC
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, g_hLoadedBitmap);

            // Get bitmap size
            BITMAP bmp;
            GetObject(g_hLoadedBitmap, sizeof(BITMAP), &bmp);

            // Copy bitmap to window DC
            BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);

            // Cleanup
            SelectObject(hdcMem, hOldBmp);
            DeleteDC(hdcMem);
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_SETCURSOR:
        SetCursor(currentCursor);
        return TRUE;
    case WM_DESTROY:
    {
        if (g_hLoadedBitmap) {
            DeleteObject(g_hLoadedBitmap);
            g_hLoadedBitmap = nullptr;
        }
        PostQuitMessage(0);
    }
        
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Add_Theme_Menu(HWND hWnd) {
    MainMenu = CreateMenu();

    HMENU File = CreateMenu();
    HMENU Draw = CreateMenu();
    HMENU Line = CreateMenu();
    HMENU Circle = CreateMenu();
    HMENU Ellipse = CreateMenu();
    HMENU Fill = CreateMenu();
    HMENU FloodFill = CreateMenu();
    HMENU ScanLineFilling = CreateMenu();
    HMENU Clipping = CreateMenu();
    HMENU Clip_Point = CreateMenu();
    HMENU Clip_Line = CreateMenu();
    //HMENU Color = CreateMenu();
    HMENU Theme = CreateMenu();
    HMENU Cursor = CreateMenu();

    AppendMenuW(File, MF_STRING, SAVE_DC, L"Save");
    AppendMenuW(File, MF_STRING, RESTORE_DC, L"Restore");
    AppendMenuW(File, MF_STRING, LOAD, L"Load");
    AppendMenuW(File, MF_STRING, CLEAR_SCREEN, L"Clear Screen");


    AppendMenuW(Draw, MF_POPUP, (UINT_PTR)Line, L"Line");
    AppendMenuW(Draw, MF_POPUP, (UINT_PTR)Circle, L"Circle");
    AppendMenuW(Draw, MF_POPUP, (UINT_PTR)Ellipse, L"Ellipse");
    AppendMenuW(Draw, MF_POPUP, Draw_Cardinal_Spline, L"Cardinal Spline Curve");

    AppendMenuW(Line, MF_STRING, Draw_Line_DDA, L"DDA");
    AppendMenuW(Line, MF_STRING, Draw_Line_Bres, L"Midpoint");
    AppendMenuW(Line, MF_STRING, Draw_Line_Param, L"Parametric");

    AppendMenuW(Circle, MF_STRING, circle_direct, L"Direct");
    AppendMenuW(Circle, MF_STRING, circle_polar, L"Polar");
    AppendMenuW(Circle, MF_STRING, circle_polar_iter, L"Iterative polar");
    AppendMenuW(Circle, MF_STRING, circle_midpoint, L"Midpoint");
    AppendMenuW(Circle, MF_STRING, circle_modified_mid, L"Modified Midpoint");


    AppendMenuW(FloodFill, MF_STRING, Rec_Flood_Fill, L"Recursive");
    AppendMenuW(FloodFill, MF_STRING, Non_Rec_Flood_Fill, L"Non Recursive");

    AppendMenuW(ScanLineFilling, MF_STRING, Convex_Fill, L"Convex Filling");
    AppendMenuW(ScanLineFilling, MF_STRING, Non_Convex_Fill, L"Non Convex Filling");

    AppendMenuW(Fill, MF_POPUP, (UINT_PTR)FloodFill, L"Flood Fill");
    AppendMenuW(Fill, MF_POPUP, (UINT_PTR)ScanLineFilling, L"Scan Line Fill");
    AppendMenuW(Fill, MF_STRING, fill_circle_line, L"Fill Circle Quarter (Lines)");
    AppendMenuW(Fill, MF_STRING, fill_circle_circles, L"Fill Circle Quarter (Circles)");
    AppendMenuW(Fill, MF_STRING, fill_square_hermite, L"Fill Square Hermite");
    AppendMenuW(Fill, MF_STRING, fill_rectangle_bezier, L"Fill Rectangle Bezier");


    AppendMenuW(Ellipse, MF_STRING, ellipse_direct, L"Direct");
    AppendMenuW(Ellipse, MF_STRING, ellipse_polar, L"Polar");
    AppendMenuW(Ellipse, MF_STRING, ellipse_midpoint, L"Midpoint");

    AppendMenuW(Clipping, MF_POPUP, (UINT_PTR)Clip_Point, L"Point");
    AppendMenuW(Clipping, MF_POPUP, (UINT_PTR)Clip_Line, L"Line");
    AppendMenuW(Clipping, MF_POPUP, clip_polygon, L"Polygon");

    AppendMenuW(Clip_Point, MF_POPUP, clip_point_rec, L"Rectangle Window");
    AppendMenuW(Clip_Point, MF_POPUP, clip_point_square, L"Square Window");

    AppendMenuW(Clip_Line, MF_POPUP, clip_line_rec, L"Rectangle Window");
    AppendMenuW(Clip_Line, MF_POPUP, clip_line_square, L"Square Window");

    AppendMenuW(Theme, MF_STRING, Set_Bkg_Light, L"Light");
    AppendMenuW(Theme, MF_STRING, Set_Bkg_Dark, L"Dark");

    AppendMenuW(Cursor, MF_STRING, Set_Arrow_Cursor, L"Arrow");
    AppendMenuW(Cursor, MF_STRING, Set_Cross_Cursor, L"Plus");
    AppendMenuW(Cursor, MF_STRING, Set_Ibeam_Cursor, L"I Beam");

    
    //AppendMenuW(Color, MF_STRING, Choose_fill_Color, L"Choose Fill Color");
    //AppendMenuW(Color, MF_STRING, Choose_Color, L"Choose Drawing Color");

    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)File, L"File");
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Draw, L"Draw");
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Fill, L"Fill");
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Clipping, L"Clip");
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Theme, L"Theme"); 
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Cursor, L"Cursor");
    AppendMenuW(MainMenu, MF_STRING, Choose_Color, L"Choose Color");
    SetMenu(hWnd, MainMenu);
}
