#include "Server.h"


std::condition_variable cv; // Lockes the handle message function until a message has arrived
queue<ReceivedMessage*> _queRcvMessages;

Server::Server()
{
	srand(static_cast<int>(time(NULL)));

	try
	{
		_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Creating the main socket of the sever
	}
	catch (exception e)
	{
		std::cout << e.what() << std::endl;
	}

	if (_serverSocket == INVALID_SOCKET) // If the create socket function was not successful
		throw std::exception(__FUNCTION__ " - socket");
}

Server::~Server()
{
	closesocket(_serverSocket);
}

void Server::serve()
{
	thread findIps(LocalNetFunctions::getUsersOnNetwork);
	findIps.detach();

	thread handleFiles(&Server::handleReceiveFile, this);
	handleFiles.detach();

	thread t1(&Server::handleReceivedMessages, this);
	t1.detach();

	bindAndListen(); // Calling the function that will wait for a client response
}

void Server::bindAndListen()
{
	const int port = 22223;
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); 			// port that server will listen for
	sa.sin_family = AF_INET;   			// must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. 

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (::listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		try
		{
			cv.notify_all();
			accept();
		}
		catch (exception& e)
		{
			std::cout << "Exception at: " << e.what() << std::endl;
			exit(1);
		}
	}
}

void Server::accept()
{
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = ::accept(_serverSocket, NULL, NULL); // Accepting the client that wants to connect

	if (clientSocket == INVALID_SOCKET) // If the accept function was not successful
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;

	thread t1(&Server::clientHandler, this, clientSocket); // Creating a thread of the client handler function
	t1.detach();
}

void Server::handleReceivedMessages()
{
	while (1)
	{
		std::unique_lock<std::mutex> lck(_mtxReceivedMessages);
		cv.wait(lck, []{return _queRcvMessages.size() != 0; });

		ReceivedMessage* msg = _queRcvMessages.front(); // Gets the message from the queue
		_queRcvMessages.pop();

		switch (msg->getMessageCode()) // Switch based on the type code
		{
		case 200:
			handleSignin(msg);
			break;

		case 201:
			handleSignout(msg);
			break;

		case 202:
			handleSignup(msg);
			break;

		case 203:
			handleUploadFile(msg);
		case 204: 
			handleReceiveFile();
		}	
	}
}

void Server::clientHandler(SOCKET clientSocket)
{
	ReceivedMessage* msg = NULL;
	int typeCode = 0;

	try
	{
		do
		{
			typeCode = Helper::getMessageTypeCode(clientSocket);
			msg = buildRecieveMessage(clientSocket, typeCode); // Builds the message
			addReceivedMessage(msg); // Adds the message to the messages queue 
		} while (typeCode != 0);
	}
	catch (exception e)
	{
		std::cout << e.what() << std::endl;
		closesocket(clientSocket);
	}
	closesocket(clientSocket);
}

void Server::addReceivedMessage(ReceivedMessage* msg)
{
	_mtxReceivedMessages.lock();
	_queRcvMessages.push(msg); // Adds the message to the queue
	_mtxReceivedMessages.unlock();
	cv.notify_all(); // Unlockes all locked functions
}

ReceivedMessage* Server::buildRecieveMessage(SOCKET clientSocket, int typeCode)
{
	vector<string> vec;
	int length = 0;

	if (typeCode == 200)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string username = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string password = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(username);
		vec.push_back(password);
	}

	else if (typeCode == 202)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string username = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string password = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string email = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string cloudSize = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(username);
		vec.push_back(password);
		vec.push_back(email);
		vec.push_back(cloudSize);
	}

	if (typeCode == 203)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string filePath = Helper::getStringPartFromSocket(clientSocket, length);
		int encrypt = Helper::getIntPartFromSocket(clientSocket, 1);

		vec.push_back(filePath);
		vec.push_back(to_string(encrypt));
	}

	return new ReceivedMessage(clientSocket, typeCode, vec);
}

bool Server::handleSignup(ReceivedMessage* msg)
{
	bool res = 0;
	if (!Validator::isUsernameValid(msg->getValues()[0]))
	{
		Helper::sendData(msg->getSock(), "1013");
	}
	else if (!Validator::isPasswordValid(msg->getValues()[1]))
	{
		Helper::sendData(msg->getSock(), "1011");
	}
	else if (db->doesUserExists(msg->getValues()[0]))
	{
		Helper::sendData(msg->getSock(), "1012");
	}
	else
	{
		bool res = db->addNewUser(msg->getValues()[0], msg->getValues()[1], msg->getValues()[2],
			msg->getValues()[3]);

		if (res == 0)
			Helper::sendData(msg->getSock(), "1014");
		else
			Helper::sendData(msg->getSock(), "1010");
	}

	return res;
}

User* Server::handleSignin(ReceivedMessage* msg)
{
	if (Validator::isUsernameValid(msg->getValues()[0]) && Validator::isPasswordValid(msg->getValues()[1]))
	{
		if (!db->isUserAndPassMatch(msg->getValues()[0], msg->getValues()[1])) // Checks if the user has the given password
		{
			Helper::sendData(msg->getSock(), "1001");
			return nullptr;
		}
		else
		{
			Helper::sendData(msg->getSock(), "1000");
		}

		return new User(msg->getValues()[0], msg->getSock());
	}
	else
	{
		Helper::sendData(msg->getSock(), "1003");
		return nullptr;
	}
}

void Server::handleSignout(ReceivedMessage* msg)
{
	// TODO : handle user signout
}

int Server::handleUploadFile(ReceivedMessage* msg)
{
	string fpath = msg->getValues()[0].c_str();
	string ip = "192.168.1.27";

	handleSendFile(strdup(ip.c_str()), strdup(fpath.c_str()));
	return 0;
}

int Server::handleSendFile(char* ip, char* fpath)
{
	WComm w;
	string rec = " ";
	const int port = 8888;

	// Connect To Server
	w.connectServer(ip, port);

	// Sending File
	w.sendData("FileSend");	w.recvData(strdup(rec.c_str()), 32);
	w.fileSend(fpath);

	// Send Close Connection Signal
	w.sendData("EndConnection"); w.recvData(strdup(rec.c_str()), 32);

	//delete rec;
	return 0;
}

int Server::handleReceiveFile()
{
	WComm w;
	const int port = 8888;
	// Start Server Daemon
	w.startServer(port);
	while (TRUE) {
		// Wait until a client connects
		w.waitForClient();

		// Work with client
		while (TRUE)
		{
			char rec[50] = "";
			w.recvData(rec, 32); w.sendData("OK");

			if (strcmp(rec, "FileSend") == 0)
			{
				char fname[32] = "";
				w.fileReceive(fname);
				break;
			}
			if (strcmp(rec, "EndConnection") == 0)break;
		}
		// Disconnect client
		w.closeConnection();
	}

	return 0;
}