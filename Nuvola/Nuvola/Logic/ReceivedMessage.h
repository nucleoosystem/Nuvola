#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../Networking/WSAInitializer.h"

using namespace std;

class ReceivedMessage
{
public:
	ReceivedMessage(SOCKET clientSocket, int num);
	ReceivedMessage(SOCKET clientSocket, int num, vector<string> vec);
	~ReceivedMessage();
	SOCKET getSock();
	int getMessageCode();
	vector<string>& getValues();

private:
	SOCKET _sock;
	int _messageCode;
	vector<string> _values;
};