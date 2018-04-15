#include "winsock2.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <fstream>

#include "Protocols.h"
#include "../Logic/Helper.h"

class WComm
{

public:

	WComm();

	void connectServer(char*, int);
	int sendData(char*);
	int recvData(char*, int);
	void fileSend(char*, SOCKET blockingSocket);
	void fileReceive(char*);
	void startServer(int);
	void waitForClient();
	void closeConnection();

	void sendFileUploadPer(string fileName, int percentage, SOCKET blockingSocket);
};