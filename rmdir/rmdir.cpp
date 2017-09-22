// rmdir.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <strsafe.h>
#include <iostream>
#include <atlstr.h>
//#include <string>

using namespace std;
void rm_R(TCHAR * Directory);
TCHAR * ReducePath(TCHAR * Directory);
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
		//wcout << argc << " tamaño del argc" << endl;
		if (argc == 3){//&& _tcscmp(argv[1], _T("-R"))) {
			if (argv[2][0] == 'c' || argv[2][0] == 'C') {
				TCHAR Directory[MAX_PATH];
				int i = 0;
				int iD = 0;
				while (argv[2][i] != NULL) {
					if (argv[2][i] == '\\') {
						Directory[iD] = '\\';
						iD++;
						Directory[iD] = '\\';
					}
					else {
						Directory[iD] = argv[2][i];
					}
					i++;
					iD++;
				}

				Directory[iD] = NULL;
				TCHAR* SecondDirectory = Directory;
				rm_R(SecondDirectory);//llamado recursivo de borrar y reducir el PATH

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
				while (argv[2][j] != NULL)
				{
					Directory[iD] = argv[2][j];
					iD++;
					j++;
				}
				Directory[iD] = NULL;
				TCHAR* SecondDirectory = Directory;
				rm_R(SecondDirectory);//llamado recursivo de borrar y reducir el PATH

			}
		}
		else if (argc == 2) {
			if (argv[1][0] == 'c' || argv[1][0] == 'C') {
				TCHAR Directory[MAX_PATH];
				int i = 0;
				int iD = 0;
				while (argv[1][i] != NULL) {
					if (argv[1][i] == '\\') {
						Directory[iD] = '\\';
						iD++;
						Directory[iD] = '\\';
					}
					else {
						Directory[iD] = argv[1][i];
					}
					i++;
					iD++;
				}
				Directory[iD] = NULL;
				TCHAR* SecondDirectory = Directory;
				if (RemoveDirectory(SecondDirectory)) {
					wcout << "Folder " << argv[1] << " borrado Exitosamente" << endl;
				}
				else if (ERROR_FILE_NOT_FOUND == GetLastError()) {
					wcout << "Directorio no Existente" << endl;
				}
				else if (ERROR_DIR_NOT_EMPTY == GetLastError()) {
					wcout << "Directorio no esta vacio" << endl;
				}
				else {
					wcout << "Directorio no Existe" << endl;
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
				if (RemoveDirectory(SecondDirectory)) {
					wcout << "Folder " << argv[1] << " borrado Exitosamente" << endl;
				}
				else if (ERROR_FILE_NOT_FOUND == GetLastError()) {
					wcout << "Directorio no Existente" << endl;
				}
				else if (ERROR_DIR_NOT_EMPTY == GetLastError()) {
					wcout << "Directorio no esta vacio" << endl;
				}
				else {
					wcout << "Directorio no Existe" << endl;
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


//reduce el PATH hasta el directorio ( \ ) anterior
TCHAR * ReducePath(TCHAR * Directory) {
	int k = _tcslen(Directory);
	while (Directory[k] != '\\') {
		//wcout << Directory[k] << " ";
		k--;
	}
	k--;
	Directory[k] = NULL;
	TCHAR * D2 = Directory;
	return D2;
}

//borra de forma recursiva el directorio hasta encontrar uno que no esté vacío
void rm_R(TCHAR * Directory) {
	//int i = 1;
	//TCHAR * temp = Directory;
	if (RemoveDirectory(Directory)) {
		wcout << " Folder " << " borrado Exitosamente" << endl;
		//rm_R(ReducePath(temp));
		rm_R(ReducePath(Directory));
	}
	else if (ERROR_FILE_NOT_FOUND == GetLastError()) {
		wcout << "Directorio no Existente" << endl;
	}
	else if (ERROR_DIR_NOT_EMPTY == GetLastError()) {
		wcout << "este directorio no  esta vacio" << endl;
	}
	else {
		wcout << "Directorio no Existe" << endl;
	}
}