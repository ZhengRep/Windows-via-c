#include<windows.h>
#include<shellapi.h>
#include<tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = TEXT("RunAs");
	sei.lpFile = TEXT("cmd.exe");
	sei.nShow = SW_SHOWNORMAL;

	if (!ShellExecuteEx(&sei)) {
		DWORD status = GetLastError();
		if (status == ERROR_CANCELLED) {
			MessageBox(NULL, TEXT("Cancel!"), NULL, 0);
		}
		else if (status == ERROR_FILE_NOT_FOUND) {
			MessageBox(NULL, TEXT("Error file not found!"), NULL, 0);
		}
	}

	return 0;
}