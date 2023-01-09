#include<stdio.h>
#include<tchar.h>
#include<windows.h>
#include<wchar.h>

/*
Terminate process:
ExitProcess(avoid this method) (will not be destruct) 
TerminateProcess
*/
int main(int argc, char* argv[], char* env[])
{
	printf(argv[1]);
	MessageBox(NULL, _T("Hello, This is ChildProcess!"), NULL, 0);
	return 0;
}