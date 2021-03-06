// kill.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <psapi.h>
#include <iostream>
#include <stdlib.h>
//#include <typeinfo>

using namespace std;

void killProcessById(DWORD processId)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (pEntry.th32ProcessID == processId)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

int _tmain(int argc, TCHAR *argv[])
{
	// Find out variable types with this
	// TCHAR *test = argv[1];
	// cout << typeid(test).name() << '\n';
	if (argc != 2)
	{
		_tprintf(TEXT("Wrong kill usage. Correct usage is: kill [process id]\n"));
		return 1;
	}

	//_wtol: converts wchar_t* to DWORD
	DWORD p_id = _wtol(argv[1]);
	killProcessById(p_id);
	return 0;
}
