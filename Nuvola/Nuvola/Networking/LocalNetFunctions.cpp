#include "LocalNetFunctions.h"

const char* arpExe64Bit = "Logic\\arp-scan-64.exe";
const char* arpExe32Bit = "Logic\\arp-scan-86.exe";
const char* targetIpFileName = "..//..//NuvolaWindowsForms//NuvolaWindowsForms//bin//Debug//IP.txt";
const char* batFileName = "Networking\\GetLocalIps.bat";
const char* foundIpsFileName = "Networking\\foundIps.txt";

// Returns the local ip address and the ip mask of the computer
std::vector<std::pair<std::string, std::string>> LocalNetFunctions::getLocalIpAddress()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	std::vector<std::pair<std::string, std::string>> returnArr;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return returnArr;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return returnArr;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			string ipAddr(pAdapter->IpAddressList.IpAddress.String);
			string ipMask(pAdapter->IpAddressList.IpMask.String);

			if ((pAdapter->Type == IF_TYPE_IEEE80211 || pAdapter->Type == IF_TYPE_ETHERNET_CSMACD) && ipAddr != "0.0.0.0" && ipMask != "0.0.0.0")
			{
				if (split(ipAddr, '.')[0] == "192" || split(ipAddr, '.')[0] == "10")
				{
					std::pair<std::string, std::string> addrMask(ipAddr, ipMask);
					returnArr.push_back(addrMask);
				}
			}
			pAdapter = pAdapter->Next;
		}
	}
	else {
		printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);

	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return returnArr;
}

std::vector<std::string> LocalNetFunctions::split(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

void LocalNetFunctions::getIpAddrsFromFile(string fileName)
{
	vector<string> foundIps;
	ifstream file;
	string line;

	file.open(fileName);
	if (file.is_open())
	{
		while (file.eof() == false)
		{
			wchar_t wtext[20];
			file >> line;
			mbstowcs(wtext, line.c_str(), strlen(line.c_str()) + 1); /* Converting a string to a wchar_t */

			struct sockaddr_in sa;
			if (InetPton(AF_INET, wtext, &(sa.sin_addr)) != 0)
			{
				foundIps.push_back(line);
			}

		}
	}
	file.close();

	fstream guiIpsFile;
	guiIpsFile.open(targetIpFileName, fstream::out | fstream::trunc);

	if (guiIpsFile.is_open())
	{
		for (int i = 0; i < foundIps.size(); i++)
		{
			guiIpsFile << foundIps[i];
			guiIpsFile << '\n';
		}
	}
	guiIpsFile.close();

}

void LocalNetFunctions::getUsersOnNetwork()
{
	string exeName = arpExe64Bit;

	ofstream foundIpsFile;
	foundIpsFile.open(foundIpsFileName, ios::trunc);
	foundIpsFile.close();

	ofstream batFile;
	batFile.open(batFileName, ios::trunc);
	batFile << "@echo off	\n";
	batFile << "netsh interface ip delete arpcache\n";

	std::vector<pair<string, string>> ipAndSubnet; /* Gets the ip and the subnet of the local computer */

	ipAndSubnet = getLocalIpAddress();
	auto currentPair = ipAndSubnet.begin();
	for (currentPair; currentPair != ipAndSubnet.end(); currentPair++)
	{
		char* srcIpString = new char[strlen(currentPair->first.c_str()) + 1];
		char* subnetMask = new char[strlen(currentPair->second.c_str()) + 1];

		strncpy(srcIpString, currentPair->first.c_str(), strlen(currentPair->first.c_str()));
		srcIpString[strlen(currentPair->first.c_str())] = '\0';
		strncpy(subnetMask, currentPair->second.c_str(), strlen(currentPair->second.c_str()));
		subnetMask[strlen(currentPair->second.c_str()) + 1] = '\0';

		string dataToWrite = exeName + " -t " + srcIpString + "/" + getBitsInSubnet(subnetMask) + "\n";
		batFile << dataToWrite;
	}
	batFile << "arp -a > ";
	batFile << foundIpsFileName;
	batFile.close();

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

	LPCTSTR lpApplicationName = L"Networking\\GetLocalIps.bat";
	DWORD creationFlags = CREATE_NO_WINDOW;

	CreateProcess(lpApplicationName, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	cout << "Finished" << endl;
	getIpAddrsFromFile(foundIpsFileName);
}

string LocalNetFunctions::getBitsInSubnet(string subnet)
{
	vector<string> tokens = split(subnet, '.');
	int totalSum = 0;
	for (int i = 0; i < tokens.size(); i++)
	{
		string binary = std::bitset<8>(atoi(tokens[i].c_str())).to_string();
		size_t numberOfOnes = std::count(binary.begin(), binary.end(), '1');
		totalSum += numberOfOnes;
	}

	return to_string(totalSum);
}