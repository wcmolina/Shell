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

BOOL DirectoryExists(LPCTSTR);

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
		if (strcmp(cmd, "cd") == 0) {
			wcout << cDirBuffer << endl;
			continue;
		}
		if (cmd[0] == 'c' && cmd[1] == 'd') {
			if (cmd[3] == '.' && cmd[4] == '.') {
				TCHAR n_cDirBuffer[MAX_PATH];
				int i = 0;
				int Pleca = 0;
				while (cDirBuffer[i] != NULL) {
					if (cDirBuffer[i] == '\\') {
						Pleca = i;
					}
					i++;
				}
				for (int j = 0; j < Pleca; j++) {
					n_cDirBuffer[j] = cDirBuffer[j];
				}
				for (int j = 0; j < Pleca; j++) {
					cDirBuffer[j] = n_cDirBuffer[j];
					cDirBuffer[j + 1] = NULL;
				}
				if (cDirBuffer[2] == NULL) {
					cDirBuffer[2] = '\\';
					cDirBuffer[3] = NULL;
				}
				SetCurrentDirectory(cDirBuffer);
				wcout << "Directorio Anterior" << endl;
			} else if (cmd[3] == 'C') {
				TCHAR n_cDirBuffer[MAX_PATH];
				int i = 3;
				int j = 0;
				while (cmd[i] != NULL) {
					n_cDirBuffer[j] = cmd[i];
					n_cDirBuffer[j + 1] = NULL;
					i++;
					j++;
				}
				if (PathIsDirectory(n_cDirBuffer)) {
					for (int k = 0; k < j; k++) {
						cDirBuffer[k] = n_cDirBuffer[k];
						cDirBuffer[k + 1] = NULL;
					}
					SetCurrentDirectory(cDirBuffer);
					wcout << "Directorio Existe" << endl;
				} else {
					wcout << "Directorio Inexistente" << endl;
				}
			} else {
				TCHAR n_cDirBuffer[MAX_PATH];
				int i = 0;
				while (cDirBuffer[i] != NULL) {
					n_cDirBuffer[i] = cDirBuffer[i];
					i++;
				}
				n_cDirBuffer[i] = '\\';
				i++;
				int j = 3;
				while (cmd[j] != NULL) {
					n_cDirBuffer[i] = cmd[j];
					n_cDirBuffer[i + 1] = NULL;
					i++;
					j++;
				}
				wcout << n_cDirBuffer << endl;
				if (PathIsDirectory(n_cDirBuffer)) {
					for (int k = 0; k < i; k++) {
						cDirBuffer[k] = n_cDirBuffer[k];
						cDirBuffer[k + 1] = NULL;
					}
					SetCurrentDirectory(cDirBuffer);
					wcout << "Directorio Existe" << endl;
				} else {
					wcout << "Directorio Inexistente" << endl;
				}
			}
			continue;
		}
		if (cmd[0] == NULL) {
			continue;
		}
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

BOOL DirectoryExists(LPCTSTR szPath) {
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}