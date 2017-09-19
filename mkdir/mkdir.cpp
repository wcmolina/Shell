// mkdir.cpp : Defines the entry point for the console application.
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
	DWORD dw = GetLastError();
	cDirRet = GetCurrentDirectory(MAX_PATH, cDirBuffer);
	if (cDirRet == 0) {
		printf("GetCurrentDirectory failed (%d)\n", GetLastError());
		return 0;
	}
	StringCchCat(cDirBuffer, MAX_PATH, TEXT("\\*"));

	LARGE_INTEGER fileSz;
	SYSTEMTIME stUTC, stLocal;
	TCHAR writeTimeBuffer[MAX_PATH];
	BOOL isDir, isHidden;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(cDirBuffer, &data);

	if (hFind != INVALID_HANDLE_VALUE) {
		if (argc == 2) {
			if (argv[1][0] == 'c' || argv[1][0] == 'C') {
				TCHAR Directory[MAX_PATH];
				int i = 0;
				int iD = 0;
				while (argv[1][i] != NULL) {
					if (argv[1][i] == '\\') {
						Directory[iD] = '\\';
						iD++;
						Directory[iD] = '\\';
					} else {
						Directory[iD] = argv[1][i];
					}
					i++;
					iD++;
				}
				Directory[iD] = NULL;
				TCHAR* SecondDirectory = Directory;
				if (CreateDirectory(SecondDirectory, NULL)) {
					wcout << "Folder " << argv[1] << " Creado Exitosamente" << endl;
				}
				else if (ERROR_ALREADY_EXISTS == GetLastError()) {
					wcout << "Directorio ya Existente" << endl;
				}
				else {
					wcout << "No quiere funcionar" << endl;
				}
			}
			else {
				TCHAR Directory[MAX_PATH];
				int i = 0;
				int iD = 0;
				while (cDirBuffer[i] != '*') {
					if (cDirBuffer[i] == '\\')
					{
						Directory[iD] = '\\';
						iD++;
						Directory[iD] = '\\';
					}
					else {
						Directory[iD] = cDirBuffer[i];
					}
					i++;
					iD++;
				}
				int j = 0;
				while (argv[1][j] != NULL)
				{
					Directory[iD] = argv[1][j];
					iD++;
					j++;
				}
				Directory[iD] = NULL;
				TCHAR* SecondDirectory = Directory;
				if (CreateDirectory(SecondDirectory, NULL)) {
					wcout << "Folder " << argv[1] << " Creado Exitosamente" << endl;
				}
				else if (ERROR_ALREADY_EXISTS == GetLastError()) {
					wcout << "Directorio ya Existente" << endl;
				}
				else {
					wcout << "No quiere funcionar" << endl;
				}
			}
		}
		else if (argc == 1)
		{
			wcout << "Se necesita un nombre para la carpeta " << endl;
		}
		else
		{
			wcout << "Argumentos De Mas" << endl;
		}
		FindClose(hFind);
	}
	return 0;
}