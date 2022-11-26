#include<windows.h>
#include<tchar.h>
#include<stdio.h>
#include<VersionHelpers.h>

#pragma warning(disable: 4996)

int _tmain(int argc, TCHAR* argv)
{
	/*OSVERSIONINFOEX osInfo;
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);
	bool res = GetVersionEx((LPOSVERSIONINFOW) & osInfo);
	if (!res) {
		_tprintf(_T("GetVersionEx wrong\r\n"));
	}*/


   /* if (IsWindows10OrGreater())
    {
        printf("Windows10OrGreater\n");
    }

    if (IsWindowsServer())
    {
        printf("Server\n");
    }
    else
    {
        printf("Client\n");
    }*/

	return 0;
}