// Singleton.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include<strsafe.h>
#include "Singleton.h"
#include<sddl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE g_hNamespace;
HANDLE g_hSingleton;

LPCTSTR g_szBoundary = _T("Singleton-Boundary");
LPCTSTR g_szNamespace = _T("Singleton-Namespace");
HANDLE g_hBoundary;
HWND hWnd;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void AddTextToWindow(PCTSTR pszFormat, ...);
void CheckInstances();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    CheckInstances();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SINGLETON, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SINGLETON));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SINGLETON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SINGLETON);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    //add

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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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

void AddTextToWindow(PCTSTR pszFormat, ...)
{
    va_list argList;
    va_start(argList, pszFormat);

    TCHAR sz[100];
    _vstprintf_s(sz, pszFormat, argList);


    va_end(argList);
}

void CheckInstances()
{
    //create the boundary descriptor
    g_hBoundary = CreateBoundaryDescriptor(g_szBoundary, 0);
    
    //create a sid corresponding to the local administrator group
    BYTE localAdminSID[SECURITY_MAX_SID_SIZE];
    PSID pLocalAdminSID = &localAdminSID;
    DWORD cbSID = sizeof(localAdminSID);
    if (!CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, pLocalAdminSID, &cbSID)) {
        //MessageBox(NULL, _T("AddSIDToBoundaryDescriptor failed!\r\n"), NULL, 0);
        return;
    }

    //Associate the local admin sid to the boundary descriptor
    if (!AddSIDToBoundaryDescriptor(&g_hBoundary, pLocalAdminSID)) {
        return;
    }

    //Create the namespace for local administrators only
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = FALSE;
    if (!ConvertStringSecurityDescriptorToSecurityDescriptor(_T("D:(A;;GA;;;BA)"), SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL)) {
        return;
    }

    g_hNamespace = CreatePrivateNamespace(&sa, g_hBoundary, g_szNamespace);
    LocalFree(sa.lpSecurityDescriptor);

    DWORD dErrorCode = GetLastError();
    if (g_hNamespace == NULL) {
        if (dErrorCode == ERROR_ACCESS_DENIED) {
            MessageBox(NULL, _T("Acess denied and should run as administrator!\r\n"), NULL, 0);
            return;
        }
        else {
            if (dErrorCode == ERROR_ALREADY_EXISTS) {
                g_hNamespace = OpenPrivateNamespace(g_hBoundary, g_szBoundary);
                if (g_hNamespace == NULL) {
                    return;
                }
            }
            else {
                return;
            }
        }
    }

    //try to create the nutex object with a name
    TCHAR szMutexName[64];
    StringCchPrintf(szMutexName, _countof(szMutexName), _T("%s%s\r\n"), g_szNamespace, _T("Singleton"));
    g_hSingleton = CreateMutex(NULL, FALSE, szMutexName);
    dErrorCode = GetLastError();
    if (dErrorCode == ERROR_ALREADY_EXISTS) {
        MessageBox(NULL, _T("Another instance of Singleton is running!\r\n"), NULL, 0);
    }
    else if(dErrorCode == ERROR_SUCCESS) {
        MessageBox(NULL, _T("First instance of Singleton is running!\r\n"), NULL, 0);
    }
    else {
        MessageBox(NULL, _T("Another wrong!\r\n"), NULL, 0);
    }
}

