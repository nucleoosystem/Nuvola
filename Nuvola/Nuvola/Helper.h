#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <vector>
#include <string>
#include <Ws2tcpip.h>
#include <WinSock2.h>
#include <winsock.h>
#include <MSWSock.h>
#include <iphlpapi.h>
#include <Windns.h>


#include "ReceivedMessage.h"

#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


using namespace std;

class Helper
{
public:
	static int getMessageTypeCode(SOCKET sc);
	static char* getPartFromSocket(SOCKET sc, int bytesNum, int flags);
	static int getIntPartFromSocket(SOCKET sc, int bytesNum);
	static string getStringPartFromSocket(SOCKET sc, int bytesNum);
	static void sendData(SOCKET sc, std::string message);
	static string getPaddedNumber(int num, int digits);
	static std::vector<std::string> split(const std::string &text, char sep);

	static void getUsersOnNetwork();

private:
	static char* getPartFromSocket(SOCKET sc, int bytesNum);
	static std::vector<std::pair<std::string, std::string>> getLocalIpAddress();
	static std::vector<std::string> getFirstIpsOnSubnet(string srcIpString, string subnetMask);
	static void getIpAddrsFromFile(string fileName);
};


#ifdef _DEBUG // vs add this define in debug mode
#include <stdio.h>
// Q: why do we need traces ?
// A: traces are a nice and easy way to detect bugs without even debugging
// or to understand what happened in case we miss the bug in the first time
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
// for convenient reasons we did the traces in stdout
// at general we would do this in the error stream like that
// #define TRACE(msg, ...) fprintf(stderr, msg "\n", __VA_ARGS__);

#else // we want nothing to be printed in release version
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
#define TRACE(msg, ...) // do nothing
#endif