// Graphics Project- V1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Graphics Project- V1.h"
#include "Line/lines.h"
#include "filling/floodfill.h"
#include "Curves/curves.h"
#include "utils.h"
#include "vector"

#define MAX_LOADSTRING 100

#define Draw_Line_DDA 211
#define Draw_Line_Bres 212
#define Draw_Line_Param 213

#define Rec_Flood_Fill 214
#define Draw_Cardinal_Spline 215

#define SAVE_DC 11
#define RESTORE_DC 12

#define Set_Arrow_Cursor 41
#define Set_Cross_Cursor 42
#define Set_Ibeam_Cursor 43

#define Set_Bkg_Light 31
#define Set_Bkg_Dark 32

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

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



void Add_Theme_Menu(HWND);
HMENU MainMenu;


int Last_Saved_DC;
int Required_clicks = 0;

enum Algorithm {
    NONE,
    LINE_DDA,
    LINE_BRES,
    LINE_PARAM,
    CARDINAL_SPLINE,
};

template<typename T>
class input_requirements {
public:
    int req_pts;
    vector<Point> pv;
    Algorithm alg;
    T instance;

    input_requirements(Algorithm algorithm, int pts) : alg(algorithm), req_pts(pts){
        pv.reserve(pts);
    }
    void run(HDC hdc) {
        instance.run(hdc, pv);
    }
};
class DDA_LINE {
public:
    void run(HDC hdc, vector<Point>& pv) {
        DrawLineDDA(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, RGB(255, 0, 0));
    }
};
class BRES_LINE {
public:
    void run(HDC hdc, vector<Point>& pv) {
        DrawLineBresenham(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, RGB(255, 0, 0));
    }
};
class Param_LINE {
public:
    void run(HDC hdc, vector<Point>& pv) {
        DrawLineParametric(hdc, pv[0].x, pv[0].y, pv[1].x, pv[1].y, RGB(255, 0, 0));
    }
};
class Cardinal_Spline {
public:
    void run(HDC hdc, vector<Point>& pv) {
        CardinalSpline(hdc, pv, 0.0, 1000, RGB(255, 0, 0));
    }
};
input_requirements<DDA_LINE>* dda_class = nullptr;
input_requirements<BRES_LINE>* bres_class = nullptr;
input_requirements<Param_LINE>* param_class = nullptr;
input_requirements<Cardinal_Spline>* cardinal_spline_class = nullptr;

Algorithm chosen_algo = NONE;
//req_input input;
//vector<Point> pv;
int xg, yg;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    hdc = GetDC(hWnd);
    switch (message)
    {

    case WM_CREATE:
    {

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
            HDC current_hdc = GetDC(hWnd);
            Last_Saved_DC = SaveDC(current_hdc);

        }
        break;
        case RESTORE_DC:
        {
            hdc = GetDC(hWnd);
            RestoreDC(hdc, Last_Saved_DC);
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
            SetCursor(ARROW);
            break;
        case Set_Cross_Cursor:
            SetCursor(CROSS);
            break;
        case Set_Ibeam_Cursor:
            SetCursor(IBEAM);
            break;
        case Draw_Line_DDA:
        {
            chosen_algo = LINE_DDA;
            dda_class = new input_requirements<DDA_LINE>(chosen_algo, 2);
            //DrawLineDDA(hdc, 10, 10, 300, 100, RGB(255, 0, 0));
        }
        break;
        case Draw_Line_Bres:
        {
            chosen_algo = LINE_BRES;
            bres_class = new input_requirements<BRES_LINE>(chosen_algo, 2);

        }
        break;
        case Draw_Line_Param:
        {
            chosen_algo = LINE_PARAM;
            param_class = new input_requirements<Param_LINE>(chosen_algo, 2);

        }
        break;
        //case Rec_Flood_Fill:
        case Draw_Cardinal_Spline:
        {
            chosen_algo = CARDINAL_SPLINE;
            cardinal_spline_class = new input_requirements<Cardinal_Spline>(chosen_algo, 6);
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
        if (chosen_algo != NONE) {
            //int x = LOWORD(lParam);
            //int y = HIWORD(lParam);
            //pv.push_back(Point(x, y));
            //if (pv.size() == input.req) break;

            switch (chosen_algo) {
            case LINE_DDA:
            {
                xg = LOWORD(lParam);
                yg = HIWORD(lParam);
                dda_class->pv.push_back(Point(xg, yg));
                if (dda_class->pv.size() == dda_class->req_pts) {
                    dda_class->run(hdc);
                    delete dda_class;
                    dda_class = nullptr;
                    chosen_algo = NONE;
                }
            }
            break;
            case LINE_BRES:
            {
                xg = LOWORD(lParam);
                yg = HIWORD(lParam);
                bres_class->pv.push_back(Point(xg, yg));
                if (bres_class->pv.size() == bres_class->req_pts) {
                    bres_class->run(hdc);
                    delete bres_class;
                    bres_class = nullptr;
                    chosen_algo = NONE;
                }
            }
            break;
            case LINE_PARAM:
            {
                xg = LOWORD(lParam);
                yg = HIWORD(lParam);
                param_class->pv.push_back(Point(xg, yg));
                if (param_class->pv.size() == param_class->req_pts) {
                    param_class->run(hdc);
                    delete param_class;
                    param_class = nullptr;
                    chosen_algo = NONE;
                }
            }
            break;
            case CARDINAL_SPLINE:
            {
                xg = LOWORD(lParam);
                yg = HIWORD(lParam);
                cardinal_spline_class->pv.push_back(Point(xg, yg));
                if (cardinal_spline_class->pv.size() == cardinal_spline_class->req_pts) {
                    cardinal_spline_class->run(hdc);
                    delete cardinal_spline_class;
                    cardinal_spline_class = nullptr;
                    chosen_algo = NONE;
                }
            }
            break;
            }//bta3t el switch

            ReleaseDC(hWnd, hdc);
        }//bta3t el if
        

    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
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
    HMENU Theme = CreateMenu();
    HMENU Cursor = CreateMenu();

    AppendMenuW(File, MF_STRING, SAVE_DC, L"Save");
    AppendMenuW(File, MF_STRING, RESTORE_DC, L"Load");

    AppendMenuW(Draw, MF_POPUP, (UINT_PTR)Line, L"Line");
    AppendMenuW(Draw, MF_POPUP, (UINT_PTR)Circle, L"Circle");
    AppendMenuW(Draw, MF_POPUP, (UINT_PTR)Ellipse, L"Ellipse");
    AppendMenuW(Draw, MF_POPUP, Draw_Cardinal_Spline, L"Cardinal Spline Curve");

    AppendMenuW(Line, MF_STRING, Draw_Line_DDA, L"DDA");
    AppendMenuW(Line, MF_STRING, Draw_Line_Bres, L"Midpoint");
    AppendMenuW(Line, MF_STRING, Draw_Line_Param, L"Parametric");

    AppendMenuW(Circle, MF_STRING, NULL, L"Direct");
    AppendMenuW(Circle, MF_STRING, NULL, L"Polar");
    AppendMenuW(Circle, MF_STRING, NULL, L"Iterative polar");
    AppendMenuW(Circle, MF_STRING, NULL, L"Midpoint");
    AppendMenuW(Circle, MF_STRING, NULL, L"Modified Midpoint");


    AppendMenuW(FloodFill, MF_STRING, NULL, L"Recursive");
    AppendMenuW(FloodFill, MF_STRING, NULL, L"Non Recursive");

    AppendMenuW(Fill, MF_POPUP, (UINT_PTR)FloodFill, L"Flood Fill");


    AppendMenuW(Ellipse, MF_STRING, NULL, L"Direct");
    AppendMenuW(Ellipse, MF_STRING, NULL, L"Polar");
    AppendMenuW(Ellipse, MF_STRING, NULL, L"Midpoint");


    AppendMenuW(Theme, MF_STRING, Set_Bkg_Light, L"Light");
    AppendMenuW(Theme, MF_STRING, Set_Bkg_Dark, L"Dark");

    AppendMenuW(Cursor, MF_STRING, Set_Arrow_Cursor, L"Arrow");
    AppendMenuW(Cursor, MF_STRING, Set_Cross_Cursor, L"Plus");
    AppendMenuW(Cursor, MF_STRING, Set_Ibeam_Cursor, L"I Beam");

    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)File, L"File");
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Draw, L"Draw");
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Fill, L"Fill");
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Theme, L"Theme"); //id is sent to wndproc in wparam
    AppendMenuW(MainMenu, MF_POPUP, (UINT_PTR)Cursor, L"Cursor");

    SetMenu(hWnd, MainMenu);
}
