// ls.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h> 
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>

using namespace std;

int _tmain(int argc, TCHAR *argv[])
{	
	// Current directory
	DWORD cDirRet;
	TCHAR cDirBuffer[MAX_PATH];
	cDirRet = GetCurrentDirectory(MAX_PATH, cDirBuffer);
	if (cDirRet == 0) {
		printf("GetCurrentDirectory failed (%d)\n", GetLastError());
		return 0;
	}
	// Concat cDirBuffer with *
	StringCchCat(cDirBuffer, MAX_PATH, TEXT("\\*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(cDirBuffer, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// Directory
				if (!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
					wcout << data.cFileName << "/" << endl;
				}
			}
			else {
				// File
				if (!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
					wcout << data.cFileName << endl;
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	return 0;
}