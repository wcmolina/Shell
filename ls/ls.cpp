// ls.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
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
	LARGE_INTEGER fileSz;
	SYSTEMTIME stUTC, stLocal;
	TCHAR writeTimeBuffer[MAX_PATH];
	BOOL isDir, isHidden;

	if (hFind != INVALID_HANDLE_VALUE) {
		if (argc >= 2) {
			do {
				isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				isHidden = (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
				// _tcscmp returns 0 when they are equal
				// ls -m
				if (!_tcscmp(argv[1], _T("-m")) && !isHidden) {
					if (isDir) wcout << data.cFileName << "/, ";
					else wcout << data.cFileName << ", ";
				}
				// ls -l
				else if (!_tcscmp(argv[1], _T("-l")) && !isHidden) {
					// Last write time
					FileTimeToSystemTime(&data.ftLastWriteTime, &stUTC);
					SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
					StringCchPrintf(writeTimeBuffer, MAX_PATH,
						TEXT("%02d/%02d/%d  %02d:%02d"),
						stLocal.wMonth, stLocal.wDay, stLocal.wYear,
						stLocal.wHour, stLocal.wMinute);
					wcout.width(16); wcout << left << writeTimeBuffer << " ";
					if (isDir) {
						wcout.width(10); wcout << right << "" << " ";
						wcout << left << data.cFileName << "/" << endl;
					}
					else {
						fileSz.LowPart = data.nFileSizeLow;
						fileSz.HighPart = data.nFileSizeHigh;
						wcout.width(10); wcout << right << fileSz.QuadPart << " ";
						wcout << left << data.cFileName << endl;
					}
				}
				// ls -a
				else if (!_tcscmp(argv[1], _T("-a"))) {
					if (isDir) wcout << data.cFileName << "/" << endl;
					else wcout << data.cFileName << endl;
				}
			} while (FindNextFile(hFind, &data));
			// Just print an extra space after ls -m
			if (!_tcscmp(argv[1], _T("-m"))) {
				cout << endl;
			}
		}
		// No args, simple ls
		else {
			do {
				isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				isHidden = (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
				if (!isHidden) {
					if (isDir) wcout << data.cFileName << "/" << endl;
					else wcout << data.cFileName << endl;
				}
			} while (FindNextFile(hFind, &data));
		}
		FindClose(hFind);
	}
	return 0;
}