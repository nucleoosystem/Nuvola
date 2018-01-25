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
	/*thread findIps(LocalNetFunctions::getUsersOnNetwork);
	findIps.detach();*/

	thread handleFiles(&LocalNetFunctions::receiveFile);
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
		case Protocol::SIGN_IN_REQUEST:
			handleSignin(msg);
			break;

		case Protocol::SIGN_OUT_REQUEST:
			handleSignout(msg);
			break;

		case Protocol::SIGN_UP_REQUEST:
			handleSignup(msg);
			break;

		case Protocol::UPLOAD_FILE_REQUEST:
			handleUploadFile(msg);
			break;

		case Protocol::GET_USER_INFO_REQUEST:
			handleGetUserInfo(msg);
			break;

		case Protocol::CREATE_NEW_GROUP:
			handleCreateNewGroup(msg);
			break;

		case Protocol::ADD_USER_TO_GROUP:
			handleAddUserToGroup(msg);
			break;

		case Protocol::GET_INFO_ABOUT_GROUPS:
			handleGetInfoAboutGroups(msg);
			break;
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

	if (typeCode == Protocol::SIGN_IN_REQUEST)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string username = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string password = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(username);
		vec.push_back(password);
	}

	else if (typeCode == Protocol::SIGN_UP_REQUEST)
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

	else if (typeCode == Protocol::UPLOAD_FILE_REQUEST)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string filePath = Helper::getStringPartFromSocket(clientSocket, length);
		int encrypt = Helper::getIntPartFromSocket(clientSocket, 1);
		vec.push_back(filePath);
		vec.push_back(to_string(encrypt));

		length = Helper::getIntPartFromSocket(clientSocket, 2);
		int numberOfUsers = Helper::getIntPartFromSocket(clientSocket, length);
		for (int i = 0; i < numberOfUsers; i++)
		{
			length = Helper::getIntPartFromSocket(clientSocket, 2);
			string ip = Helper::getStringPartFromSocket(clientSocket, length);
			vec.push_back(ip);
		}
	}

	else if (typeCode == Protocol::CREATE_NEW_GROUP)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string groupName = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string password = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(groupName);
		vec.push_back(password);
	}

	else if (typeCode == Protocol::ADD_USER_TO_GROUP)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string username = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string ip = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(username);
		vec.push_back(ip);
	}

	return new ReceivedMessage(clientSocket, typeCode, vec);
}

bool Server::handleSignup(ReceivedMessage* msg)
{
	bool res = 0;
	if (!Validator::isUsernameValid(msg->getValues()[0]))
	{
		Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_UP_USERNAME_ILLEGAL));
	}
	else if (!Validator::isPasswordValid(msg->getValues()[1]))
	{
		Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_UP_PASS_ILLEGAL));
	}
	else if (db->doesUserExists(msg->getValues()[0]))
	{
		Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_UP_USERNAME_EXISTS));
	}
	else
	{
		bool res = db->addNewUser(msg->getValues()[0], msg->getValues()[1], msg->getValues()[2],
			msg->getValues()[3]);

		if (res == 0)
			Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_UP_OTHER_ERROR));
		else
			Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_UP_SUCCESS));
	}

	return res;
}

User* Server::handleSignin(ReceivedMessage* msg)
{
	if (Validator::isUsernameValid(msg->getValues()[0]) && Validator::isPasswordValid(msg->getValues()[1]))
	{
		if (!db->isUserAndPassMatch(msg->getValues()[0], msg->getValues()[1])) // Checks if the user has the given password
		{
			Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_IN_WRONG_DETAILS));
			return nullptr;
		}
		else
		{
			Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_IN_SUCCESS));
		}

		return new User(msg->getValues()[0], msg->getSock());
	}
	else
	{
		Helper::sendData(msg->getSock(), to_string(Protocol::SIGN_IN_DETAILS_NOT_VALID));
		return nullptr;
	}
}

void Server::handleSignout(ReceivedMessage* msg)
{
	// TODO : handle user signout
}

int Server::handleUploadFile(ReceivedMessage* msg)
{
	string filePath = msg->getValues()[0];
	int encrypt = atoi(msg->getValues()[1].c_str());
	vector<string> ips;

	for (int i = 2; i < msg->getValues().size(); i++)
		ips.push_back(msg->getValues()[i]);

	if (ips.size() == 1)
	{
		LocalNetFunctions::sendFileToIp(strdup(ips.at(0).c_str()), strdup(filePath.c_str()));
	}
	else
	{
		LocalNetFunctions::uploadFileToGroup(filePath, encrypt, ips);
	}

	return 0;
}

void Server::handleGetUserInfo(ReceivedMessage* msg)
{
	vector<string> values = db->getCurrentUserInfo();
	string sendString = "102" + Helper::getPaddedNumber(values[0].length(), 2) + values[0];
	sendString += Helper::getPaddedNumber(values[2].length(), 2) + values[2];
	sendString += Helper::getPaddedNumber(values[1].length(), 2) + values[1];

	Helper::sendData(msg->getSock(), sendString);
}

void Server::handleCreateNewGroup(ReceivedMessage* msg)
{
	db->insertNewGroup(msg->getValues()[0], msg->getValues()[1]);
}

void Server::handleAddUserToGroup(ReceivedMessage* msg)
{

}

void Server::handleGetInfoAboutGroups(ReceivedMessage* msg)
{
	vector<pair<string,string>> values = db->getInfoAboutGroups();
	string sendString = "103" + Helper::getPaddedNumber(to_string(values.size()).length(), 2) + to_string(values.size());

	for (int i = 0; i < values.size(); i++)
	{
		sendString += Helper::getPaddedNumber(values[i].first.length(), 2);
		sendString += values[i].first;
		sendString += Helper::getPaddedNumber(values[i].second.length(), 2);
		sendString += values[i].second;
	}

	Helper::sendData(msg->getSock(), sendString);
}