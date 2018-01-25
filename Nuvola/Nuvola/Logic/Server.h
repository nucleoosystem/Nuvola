#pragma once

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <fstream>
#include <WinSock2.h>

#include "CreateVirtualDriver.h"
#include "Helper.h"
#include "User.h"
#include "../Networking/LocalNetFunctions.h"
#include "../DB\Database.h"
#include "../DB\sqlite3.h"
#include "Validator.h"

#pragma comment(lib, "Ws2_32.lib")

class ReceivedMessage;

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

	int handleUploadFile(ReceivedMessage* msg);

	bool handleSignup(ReceivedMessage* msg);
	User* handleSignin(ReceivedMessage* msg);
	void handleSignout(ReceivedMessage* msg);
	void handleGetUserInfo(ReceivedMessage* msg);

	void handleCreateNewGroup(ReceivedMessage* msg);
	void handleAddUserToGroup(ReceivedMessage* msg);
	void handleGetInfoAboutGroups(ReceivedMessage* msg);

	mutex _mtxReceivedMessages;
	Database* db;
};
