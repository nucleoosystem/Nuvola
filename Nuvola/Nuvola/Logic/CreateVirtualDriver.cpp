#include "CreateVirtualDriver.h"

static const GUID VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT = { 0xEC984AEC, 0xA0F9, 0x47e9, 0x90, 0x1F, 0x71, 0x41, 0x5A, 0x66, 0x34, 0x5B };

DWORD CreateVirtualDriver(PCWSTR path, int size)
{
	HANDLE handle;
	DWORD result = CreateDisk(path, &handle, size);

	if (result == ERROR_FILE_EXISTS)
	{
		result = OpenDisk(path, &handle);
		if (result != ERROR_SUCCESS)
		{
			std::wcout << "Unable to open virtual disk" << std::endl;
			return 1;
		}
	}
	else if (result != ERROR_SUCCESS)
	{
		std::wcout << "Unable to create virtual disk" << std::endl;
		return 1;
	}

	CloseHandle(handle);
	
	editDiskpartCommands(path);
	diskpartCall();

	return 0;
}


DWORD CreateDisk(PCWSTR virtualDiskFilePath, HANDLE *handle, int size)
{
	VIRTUAL_STORAGE_TYPE storageType =
	{
		VIRTUAL_STORAGE_TYPE_DEVICE_VHD,
		VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT
	};

	CREATE_VIRTUAL_DISK_PARAMETERS parameters = {};
	parameters.Version = CREATE_VIRTUAL_DISK_VERSION_1;
	parameters.Version1.MaximumSize = size; 
	parameters.Version1.BlockSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
	parameters.Version1.SectorSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE;
	parameters.Version1.SourcePath = NULL;

	int result = ::CreateVirtualDisk(
		&storageType,
		virtualDiskFilePath,
		VIRTUAL_DISK_ACCESS_ALL,
		NULL,
		CREATE_VIRTUAL_DISK_FLAG_NONE,
		0,
		&parameters,
		NULL,
		handle);

	return result;
}


DWORD OpenDisk(PCWSTR virtualDiskFilePath, HANDLE *handle)
{
	OPEN_VIRTUAL_DISK_PARAMETERS openParameters;
	openParameters.Version = OPEN_VIRTUAL_DISK_VERSION_1;
	openParameters.Version1.RWDepth = OPEN_VIRTUAL_DISK_RW_DEPTH_DEFAULT;

	VIRTUAL_STORAGE_TYPE storageType;
	storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
	storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;

	ATTACH_VIRTUAL_DISK_PARAMETERS attachParameters;
	attachParameters.Version = ATTACH_VIRTUAL_DISK_VERSION_1;

	return (OpenVirtualDisk(&storageType, virtualDiskFilePath,
		VIRTUAL_DISK_ACCESS_ALL, OPEN_VIRTUAL_DISK_FLAG_NONE,
		&openParameters, handle));
}

void diskpartCall()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.wShowWindow = SW_HIDE;

	LPCTSTR lpApplicationName = L"c:\\WINDOWS\\system32\\diskpart.exe";
	LPTSTR lpCommandLine = L"diskpart.exe /s diskPartScript.txt";
	DWORD creationFlags = CREATE_NO_WINDOW;

	CreateProcess(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, creationFlags, NULL, NULL, &si, &pi);

	std::cout << "Success Creating" << std::endl;
}


void editDiskpartCommands(PCWSTR path)
{
	std::vector<std::string> commands;	
	
	commands.push_back("select vdisk file = ");

	std::wstring wstr(path);

	commands.push_back(std::string(wstr.begin(), wstr.end()));
	commands.push_back("attach vdisk");
	commands.push_back("create partition primary");
	commands.push_back("active");
	commands.push_back("assign letter = N");
	commands.push_back("format quick fs = ntfs label = Nuvola");
	
	std::ofstream file;
	char* currentPath = NULL;
	std::string fullPath = " ";

	currentPath = _getcwd(NULL, 0);
	
	fullPath = currentPath;
	fullPath += "\\";
	fullPath += "diskPartScript.txt";

	file.open(fullPath, std::ios::trunc);
	if (file.is_open())
	{
		file.write(commands[0].c_str(), commands[0].size());
		file.write(commands[1].c_str(), commands[1].size());

		for (int i = 2; i < commands.size(); i++)
		{
			file.write("\n", 1);
			file.write(commands[i].c_str(), commands[i].size());
		}
	}
}

void deleteVirtualHardDriver(string path)
{
	std::vector<std::string> commands;
	commands.push_back("select volume N");
	commands.push_back("delete volume");

	std::ofstream file;
	char* currentPath = NULL;
	std::string fullPath = " ";

	currentPath = _getcwd(NULL, 0);

	fullPath = currentPath;
	fullPath += "\\";
	fullPath += "diskPartDelete.txt";

	file.open(fullPath, std::ios::trunc);
	if (file.is_open())
	{
		file.write(commands[0].c_str(), commands[0].size());
		file.write(commands[1].c_str(), commands[1].size());
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.wShowWindow = SW_HIDE;

	LPCTSTR lpApplicationName = L"c:\\WINDOWS\\system32\\diskpart.exe";
	LPTSTR lpCommandLine = L"diskpart.exe /s diskPartDelete.txt";
	DWORD creationFlags = CREATE_NO_WINDOW;

	CreateProcess(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, creationFlags, NULL, NULL, &si, &pi);

	std::cout << "Success Deleting" << std::endl;
}