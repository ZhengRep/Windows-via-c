#include<windows.h>
#include<tchar.h>

int main()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	TCHAR commandLine[] = _T("Test.txt");
	TCHAR path[] = _T("ChildProcess.exe");
	/*
	command line, CreateProcess also searches for the excutable in the following order;
	1. The directory containing the .exe file of the calling process
	2. The current derictory of the calling process
	3. The Windows system directory(GetSystemDirectory)
	4. The Windows directory
	5. The directories listed in the PATH environment variable
	*/

	CreateProcess(path, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	return 0;
}