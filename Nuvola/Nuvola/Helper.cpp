#include "Helper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// recieves the type code of the message from socket (3 bytes)
// and returns the code. if no message found in the socket returns 0 (which means the client disconnected)
int Helper::getMessageTypeCode(SOCKET sc)
{
	char* s = "";

	try
	{
		s = getPartFromSocket(sc, 3);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}

	std::string msg(s);

	if (msg == "")
		return 0;

	int res = std::atoi(s);
	delete s;
	return  res;
}

// recieve data from socket according byteSize
// returns the data as int
int Helper::getIntPartFromSocket(SOCKET sc, int bytesNum)
{
	char* s = "";
	try
	{
		s = getPartFromSocket(sc, bytesNum, 0);
	}
	catch (std::exception e)
	{
		cout << e.what() << endl;
	}
	return atoi(s);
}

// recieve data from socket according byteSize
// returns the data as string
string Helper::getStringPartFromSocket(SOCKET sc, int bytesNum)
{
	char* s = "";
	try
	{
		s = getPartFromSocket(sc, bytesNum, 0);
	}
	catch (std::exception e)
	{
		cout << e.what() << endl;
	}
	string res(s);
	return res;
}

// return string after padding zeros if necessary
string Helper::getPaddedNumber(int num, int digits)
{
	std::ostringstream ostr;
	ostr << std::setw(digits) << std::setfill('0') << num;
	return ostr.str();

}

// recieve data from socket according byteSize
// this is private function
char* Helper::getPartFromSocket(SOCKET sc, int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

char* Helper::getPartFromSocket(SOCKET sc, int bytesNum, int flags)
{
	if (bytesNum == 0)
	{
		return "";
	}

	char* data = new char[bytesNum + 1];
	int res = recv(sc, data, bytesNum, flags);

	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}

	data[bytesNum] = 0;
	return data;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, std::string message)
{
	const char* data = message.c_str();

	if (send(sc, data, message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}

void Helper::getUsersOnNetwork()
{
	ofstream batFile;
	batFile.open("GetLocalIps.bat", ios::trunc);
	getIpAddrsFromFile("foundIps.txt");
	//batFile << "@echo off\n";
	//batFile << "netsh interface ip delete arpcache\n";

	std::vector<pair<string, string>> ipAndSubnet; /* Gets the ip and the subnet of the local computer */
	
	/*ipAndSubnet = getLocalIpAddress();
	auto currentPair = ipAndSubnet.begin();
	for (currentPair; currentPair != ipAndSubnet.end(); currentPair++)
	{
		char* srcIpString = new char[strlen(currentPair->first.c_str()) + 1];
		char* subnetMask = new char[strlen(currentPair->second.c_str()) + 1];

		strncpy(srcIpString, currentPair->first.c_str(), strlen(currentPair->first.c_str()));
		srcIpString[strlen(currentPair->first.c_str()) + 1] = '\0';
		strncpy(subnetMask, currentPair->second.c_str(), strlen(currentPair->second.c_str()));
		subnetMask[strlen(currentPair->second.c_str()) + 1] = '\0';

		vector<string> ips = getFirstIpsOnSubnet(srcIpString, subnetMask);
		for (int i = 0; i < ips.size(); i++)
		{
			batFile << "for /L %%a in (1,1,254) do @start /b ping " + ips[i] + "%%a -w 100 -n 2 >nul\n";
		}
	}
	batFile << "arp -a > foundIps.txt";
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

	LPCTSTR lpApplicationName = L"GetLocalIps.bat";
	DWORD creationFlags = CREATE_NO_WINDOW;

	CreateProcess(lpApplicationName, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);*/
	
	//getIpAddrsFromFile("foundIps.txt");
}


// Returns the local ip address and the ip mask of the computer
std::vector<std::pair<std::string, std::string>> Helper::getLocalIpAddress()
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
				std::pair<std::string, std::string> addrMask(ipAddr, ipMask);
				returnArr.push_back(addrMask);
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

std::vector<std::string> Helper::split(const std::string &text, char sep)
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

std::vector<std::string> Helper::getFirstIpsOnSubnet(string srcIpString, string subnetMask)
{
	std::vector<std::string> ips;
	string firstIp = "";
	string oldIp = "";

	std::vector<std::string> ipTokens = split(srcIpString, '.');
	std::vector<std::string> subnetTokens = split(subnetMask, '.');

	if (subnetTokens[0] == "255" && subnetTokens[1] == "255" && subnetTokens[2] == "255")
	{
		firstIp += ipTokens[0] + "." + ipTokens[1] + "." + ipTokens[2] + ".";
		ips.push_back(firstIp);
	}
	else if (subnetTokens[0] == "255" && subnetTokens[1] == "255" && subnetTokens[2] != "0")
	{
		firstIp = ipTokens[0] + "." + ipTokens[1] + ".";
		oldIp = firstIp;
		for (int currentPart = 0; currentPart < (255 - atoi(subnetTokens[2].c_str())); currentPart++)
		{
			firstIp += to_string(currentPart) + ".";
			ips.push_back(firstIp);
			firstIp = oldIp;
		}
	}

	return ips;
}

void Helper::getIpAddrsFromFile(string fileName)
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
	guiIpsFile.open("../../NuvolaWindowsForms/NuvolaWindowsForms/bin/Debug/IP.txt", fstream::out | fstream::trunc);

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