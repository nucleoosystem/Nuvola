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
#include <future>

#include "CreateVirtualDriver.h"
#include "Helper.h"
#include "User.h"
#include "../Networking/LocalNetFunctions.h"
#include "../DB\Database.h"
#include "../DB\sqlite3.h"
#include "Validator.h"
#include "HardDiskManager.h"
#include "../Security/FileEncrypt.h"

#pragma comment(lib, "Ws2_32.lib")

#define KEY "cipher"

class ReceivedMessage;
class Database;

extern SOCKET blockingSocket;

class Server
{
public:
	Server();
	~Server();

	void serve();

	static string getDriveFreeSpace();
	static string getCurrentUsername();
	static SOCKET getCurrentSocket();

private:
	SOCKET _serverSocket;
	SOCKET _connectingSocket;

	void bindAndListen();
	void accept();
	void clientHandler(SOCKET clientSocket, string IP);

	ReceivedMessage* buildRecieveMessage(SOCKET clientSocket, int typeCode, string IP);
	void addReceivedMessage(ReceivedMessage* msg);
	void handleReceivedMessages();
	void connectBlockingSocket();

	int handleUploadFile(ReceivedMessage* msg);
	void handleGetAllFilesInfo(ReceivedMessage* msg);
	void handleGetFileBack(ReceivedMessage* msg);
	void handleRequestToReturnFile(ReceivedMessage* msg);

	bool handleSignup(ReceivedMessage* msg);
	User* handleSignin(ReceivedMessage* msg);
	void handleSignout(ReceivedMessage* msg);
	void handleGetUserInfo(ReceivedMessage* msg);
	void handleGetAllUsersInfo(ReceivedMessage* msg);

	void handleCreateNewGroup(ReceivedMessage* msg);
	void handleAddUserToGroup(ReceivedMessage* msg);
	void handleGetInfoAboutGroups(ReceivedMessage* msg);
	void handleUploadFileToGroup(ReceivedMessage* msg);
	void handleDeleteUserFromGroup(ReceivedMessage* msg);
	void handleExitGroup(ReceivedMessage* msg);
	void handleInsertNewGroup(ReceivedMessage* msg);

	void handleFinishWork(ReceivedMessage* msg);
	void handleDeleteVHD(ReceivedMessage* msg);

	void handleAskPermission(ReceivedMessage* msg);

	mutex _mtxReceivedMessages;
	Database* db;
	static User* currentUser;
	static string username;
};
