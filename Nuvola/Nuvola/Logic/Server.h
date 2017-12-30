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
#include "../Networking/WSAInitializer.h"
#include "ReceivedMessage.h"
#include "Helper.h"
#include "User.h"
#include "../Networking/LocalNetFunctions.h"
#include "../DB\Database.h"
#include "../DB\sqlite3.h"
#include "Validator.h"
#include "../Networking/WComm.h"

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

	int handleUploadFile(ReceivedMessage* msg);
	int handleSendFile(char* ip, char* fpath);
	int handleReceiveFile();

	bool handleSignup(ReceivedMessage* msg);
	User* handleSignin(ReceivedMessage* msg);
	void handleSignout(ReceivedMessage* msg);

	mutex _mtxReceivedMessages;
	Database* db;
};
