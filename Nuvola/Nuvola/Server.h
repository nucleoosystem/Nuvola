#pragma once

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <fstream>

#include "CreateVirtualDriver.h"
#include "WSAInitializer.h"
#include "ReceivedMessage.h"
#include "Helper.h"
#include "User.h"
#include "Database.h"
#include "sqlite3.h"
#include "Validator.h"

#pragma comment(lib, "Ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

	void serve();

private:
	SOCKET _serverSocket;
	SOCKET _connectingSocket;

	void bindAndListen();
	void accept();
	void clientHandler(SOCKET clientSocket);

	ReceivedMessage* buildRecieveMessage(SOCKET clientSocket, int typeCode);
	void addReceivedMessage(ReceivedMessage* msg);
	void handleReceivedMessages();

	int transmitFileToClient(ReceivedMessage* msg);
	int receiveFile(ReceivedMessage* msg);

	bool handleSignup(ReceivedMessage* msg);
	User* handleSignin(ReceivedMessage* msg);
	void handleSignout(ReceivedMessage* msg);

	mutex _mtxReceivedMessages;
	Database* db;
};
