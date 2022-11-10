#include<windows.h>
#include<tchar.h>
#include<strsafe.h>

void DumpEnvStrings()
{
	PTSTR pEnvBlock = GetEnvironmentStrings();

	/*
	=::=::\.
	=C:=C:\.
	=D:=D:\.
	ALLUSERSPROFILE=C:\ProgramData.
	APPDATA=C:\Users\ASUS\AppData\Roaming.
	*/
	//some other strings might begin with '=' (some application is started from a network share)
	TCHAR szName[MAX_PATH];
	TCHAR szValue[MAX_PATH];
	HRESULT hResult = S_OK;
	
	PTSTR pszCurrent = pEnvBlock;
	int num = 0;
	while (pszCurrent != NULL) {
		//skip the meaningless strings
		if (*pszCurrent != _T('=')) {
			PTSTR pszPos = _tcschr(pszCurrent, _T('='));
			pszPos++;

			//copy the variable name
			size_t cbNameLength = pszPos - pszCurrent - sizeof(TCHAR);
			hResult = StringCbCopyN(szName, MAX_PATH, pszCurrent, cbNameLength);
			if (FAILED(hResult)) break;

			//copy the variable value
			hResult = StringCbCopyN(szValue, MAX_PATH, pszPos, _tcslen(pszPos) + 1);
			if (SUCCEEDED(hResult)) {
				_tprintf(_T("[%d] %s = %s \r\n"), num, szName, szValue);
			}
			else if(hResult == STRSAFE_E_INSUFFICIENT_BUFFER){
				_tprintf(_T("[%d] %s = %s... \r\n"), num, szName, szValue);
			}
			else{
				_tprintf(_T("[%d] %s = ??? \r\n"), num, szName);
				break;
			}
		}
		else {
			_tprintf(_T("[%d] %s \r\n"), num, pszCurrent);
		}
		num++;

		//move to the and of the string
		while (*pszCurrent != _T('\0')) {
			pszCurrent++;
		}
		pszCurrent++;

		if (*pszCurrent == _T('\0')) break;
	}
}

int _tmain(int argc, TCHAR* argv, TCHAR* evn[])
{
	while (*evn != NULL) {
		_tprintf(_T("%s\r\n"), *evn);
		*evn++;
	}
	_tprintf(_T("************************\r\n"));
	DumpEnvStrings();
	return 0;
}