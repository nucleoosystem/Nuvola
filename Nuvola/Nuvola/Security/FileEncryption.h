#include <iostream>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <conio.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

#define KEYLENGTH  0x00800000
#define ENCRYPT_ALGORITHM CALG_RC4 
#define ENCRYPT_BLOCK_SIZE 8 

void EncryptFileMain(std::string srcFile, std::string destFile, std::string password);

bool MyEncryptFile(LPTSTR szSource,	LPTSTR szDestination, LPTSTR szPassword);

void MyHandleError(LPTSTR psz, int nErrorNumber);

LPTSTR stringToLptstr(std::string str);