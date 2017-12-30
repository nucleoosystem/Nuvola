#include <iostream>

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <vector>
#include <string>
#include <Ws2tcpip.h>
#include <Winsock2.h>
#include <winsock.h>
#include <MSWSock.h>
#include <iphlpapi.h>
#include <Windns.h>
#include <bitset>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

using namespace std;

class LocalNetFunctions
{
public:
	static void getUsersOnNetwork();

private:
	static std::vector<std::pair<std::string, std::string>> getLocalIpAddress();
	static void getIpAddrsFromFile(string fileName);
	static string getBitsInSubnet(string subnet);
	static std::vector<std::string> split(const std::string &text, char sep);
};