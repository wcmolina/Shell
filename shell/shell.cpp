// shell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <winbase.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <atlbase.h>

using namespace std;

int _tmain(int argc, TCHAR *argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	// Command
	const int CMD_SZ = 64;
	char cmd[CMD_SZ];

	// Current directory
	DWORD cDirRet;
	TCHAR cDirBuffer[MAX_PATH];
	cDirRet = GetCurrentDirectory(MAX_PATH, cDirBuffer);
	if (cDirRet == 0) {
		printf("GetCurrentDirectory failed (%d)\n", GetLastError());
		return 0;
	}

	// Loop until user enters "exit" or kills this shell
	for (;;) {
		wcout << "\n" << cDirBuffer << "> ";
		cin.getline(cmd, CMD_SZ);
		if (strcmp(cmd, "exit") == 0) break;
		// Macro needed to use A2T below
		USES_CONVERSION;
		// Start the child process.
		if (!CreateProcess(
			NULL,		// No module name (use command line)
			A2T(cmd),	// Command line
			NULL,       // Process handle not inheritable
			NULL,       // Thread handle not inheritable
			FALSE,      // Set handle inheritance to FALSE
			0,          // No creation flags
			NULL,       // Use parent's environment block
			NULL,       // Use parent's starting directory 
			&si,        // Pointer to STARTUPINFO structure
			&pi)        // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess error (%d).\n", GetLastError());
			return 0;
		}

		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return 0;
}
