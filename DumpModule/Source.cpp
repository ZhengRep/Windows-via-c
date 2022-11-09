#include<windows.h>
#include<tchar.h>

extern "C" const IMAGE_DOS_HEADER __ImageBase;

void DumpModule()
{
	//get base address of the running module
	HMODULE hModule = GetModuleHandle(L"DumpModule.exe");
	_tprintf(_T("with GetModuleHandle(NULL) = 0x%08p \r\n"), (UINT)hModule);

	//use the pseudo-variable _ImageBase to get
	_tprintf(_T("with _ImageBase = 0x%08p \r\n"), &__ImageBase);

	hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (PCTSTR)DumpModule, &hModule);
	_tprintf(_T("with GetModuleHandleEx = 0x%08p \r\n"), (UINT)hModule);
}

int _tmain(int argc, TCHAR* argv[])
{
	DumpModule();
	return 0;
}