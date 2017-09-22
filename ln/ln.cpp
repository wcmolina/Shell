// ln.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int _tmain(int argc, TCHAR *argv[])
{
	// ln -s [path to file] [path to link]
	// argv[0] = ls
	// argv[1] = -s
	// argv[2] = path to file
	// argv[3] = path to link
	if (argc != 4) {
		_tprintf(TEXT("Wrong ln usage. Correct usage is: ln -s [path to file] [path to link]\n"));
		return 1;
	}

	if (_tcscmp(argv[1], _T("-s"))) {
		_tprintf(TEXT("Unknown -s option. Correct usage is: ln -s [path to file] [path to link]\n"));
		return 1;
	}

	// Check if file or dir exists
	DWORD dwAttrib = GetFileAttributes(argv[2]);
	if (dwAttrib != INVALID_FILE_ATTRIBUTES) {
		BOOL isDir = (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
		if (isDir) {
			if (!CreateSymbolicLink(argv[3], argv[2], SYMBOLIC_LINK_FLAG_DIRECTORY)) {
				_tprintf(_TEXT("An error ocurred trying to create a symbolic link of the directory. Error code: %d\n"), GetLastError());
				return 1;
			}
		}
		else {
			// Is a file
			if (!CreateSymbolicLink(argv[3], argv[2], 0)) {
				_tprintf(_TEXT("An error ocurred trying to create a symbolic link of the file. Error code: %d\n"), GetLastError());
				return 1;
			}
		}
	}
	else {
		_tprintf(_TEXT("File or directory doesn't exist\n"));
		return 1;
	}
	return 0;
}