#include<windows.h>
#include<tchar.h>
#include "resource.h"


BOOL OnCommand(HWND hwnd, UINT controlId, UINT notificationId)
{
	switch (controlId)
	{
	case IDCANCEL:
		break;
	case IDOK:
	{
		DWORD dwErrorCode = GetDlgItemInt(hwnd, IDC_ERRORCODE, NULL, FALSE);

		HLOCAL hlocal = NULL;
		DWORD dwSystemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

		BOOL fOk = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, dwSystemLocale, (PTSTR)&hlocal, 0, NULL);
		if (!fOk) {
			HMODULE hDll = LoadLibraryEx(_T("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
			if (hDll != FALSE) {
				fOk = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, hDll, dwErrorCode, dwSystemLocale, (PTSTR)&hlocal, 0, NULL);
				FreeLibrary(hDll);
			}
		}
		
		if (fOk && (hlocal != NULL)) {
			SetDlgItemText(hwnd, IDC_ERRORTEXT, (PTSTR)LocalLock(hlocal));
			LocalFree(hlocal);
		}
		else {
			SetDlgItemText(hwnd, IDC_ERRORTEXT, _T("No text found for this error number!"));
		}
	}
		break;
	case IDC_ALWAYSONTOP:
		SetWindowPos(hwnd, IsDlgButtonChecked(hwnd, IDC_ALWAYSONTOP) ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;
	}
	return FALSE;
}

INT_PTR WINAPI DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_COMMAND:
		OnCommand(hwnd,wParam, lParam);
		break;
	}
	return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ERRORSHOWDIALOG), NULL, DlgProc, NULL);
	return 0;
}