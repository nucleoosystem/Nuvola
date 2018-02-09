#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../Networking/WSAInitializer.h"

using namespace std;

class ReceivedMessage
{
public:
	ReceivedMessage(SOCKET clientSocket, int num, string IP);
	ReceivedMessage(SOCKET clientSocket, int num, vector<string> vec, string IP);
	~ReceivedMessage();
	SOCKET getSock();
	int getMessageCode();
	vector<string>& getValues();
	string getIP();

private:
	SOCKET _sock;
	int _messageCode;
	vector<string> _values;
	string ip;
};