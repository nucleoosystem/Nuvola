#define WINVER _WIN32_WINNT_WIN7

#include <iostream>
#include <windows.h>
#include <winioctl.h>
#include <direct.h>
#include <virtdisk.h>	
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#pragma comment(lib, "virtdisk.lib")

/* The function is the main function of the cpp file.
   It calls all the other functions and creates a new virtual drive. */
DWORD CreateVirtualDriver(PCWSTR path, int size);

/* The function creates a new virtual disk.*/
DWORD CreateDisk(PCWSTR virtualDiskFilePath, HANDLE *handle, int size);

/* The function opens an existing disk if one was already created. */
DWORD OpenDisk(PCWSTR virtualDiskFilePath, HANDLE *handle);

/* The fucntion calls the diskpart program to mount the newly created virtual drive. */
void diskpartCall();

/* The function changes the diskpart commands file based on the given path. */
void editDiskpartCommands(PCWSTR path, int mbSize);

/* The function deletes the local virtual hard drive. */
void deleteVirtualHardDriver(string path);

/* The function creates the base of the virtual hard drive. */
void createVHDFile(int mbSize);
