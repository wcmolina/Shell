// chmod.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <strsafe.h>
#include <iostream>
#include <atlbase.h>

using namespace std;

void printHelp();
BOOL FileExists(LPCTSTR);

int _tmain(int argc, TCHAR *argv[]) {
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
		if (argc == 3) {
			BOOL paramValid = false, rw = false;
			if (argv[1][0] == 'r') {
				rw = true;
				paramValid = true;
			} else if (argv[1][0] == 'w') {
				rw = false;
				paramValid = true;
			} else {
				paramValid = false;
				wcout << "Parametro \'" << argv[1] << "\' no reconocido" << endl;
				printHelp();
			}
			if (paramValid) {
				BOOL ruta = false;
				TCHAR Directory[MAX_PATH];
				if (argv[2][0] == 'C') {
					int i = 0, iD = 0;
					while (argv[2][i] != NULL) {
						if (argv[2][i] == '\\') {
							Directory[iD] = '\\';
							iD++;
							Directory[iD] = '\\';
						} else {
							Directory[iD] = argv[2][i];
						}
						i++;
						iD++;
					}
					Directory[iD] = NULL;
					wcout << Directory << endl;
					if (FileExists(Directory)) {
						ruta = true;
					} else {
						ruta = false;
					}
					if (!ruta) {
						wcout << "Ruta no valida" << endl;
					}
				} else {
					int i = 0, iD = 0;
					while (cDirBuffer[i] != '*') {
						if (cDirBuffer[i] == '\\') {
							Directory[iD] = '\\';
							iD++;
							Directory[iD] = '\\';
						} else {
							Directory[iD] = cDirBuffer[i];
						}
						i++;
						iD++;
					}
					i = 0;
					while (argv[2][i] != NULL) {
						Directory[iD] = argv[2][i];
						i++;
						iD++;
					}
					Directory[iD] = NULL;
					wcout << Directory << endl;
					if (FileExists(Directory)) {
						ruta = true;
					}
					else {
						ruta = false;
					}
					if (!ruta) {
						wcout << "Archivo no valida" << endl;
					}
				}
				if (rw && ruta) {
					// Permiso para leer
					_wchmod(Directory, _S_IREAD);
					wcout << "Cambiaron Permisos a Solo Leer" << endl;
				} else {
					// Permiso para leer/escribir
					_wchmod(Directory, _S_IWRITE);
					wcout << "Cambiaron Permisos a Leer y Escribir" << endl;
				}
			}
		} else if (argc < 3) {
			wcout << "Parametros Insuficientes" << endl;
			printHelp();
		} else if (argc > 3) {
			wcout << "Demasiados Parametros" << endl;
			printHelp();
		}
		FindClose(hFind);
	}
	return 0;
}

void printHelp() {
	wcout << "chmod usage: " << endl << "chmod r|w rutaAbsoluta|nombreArchivo" << endl << "      r: Da permisos de lectura al archivo" << endl << "      w: Da permisos de lectura/escritura al archivo" << endl;
}

BOOL FileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}