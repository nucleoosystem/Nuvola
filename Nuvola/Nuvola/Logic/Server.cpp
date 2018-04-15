#include "Server.h"

std::condition_variable cv; // Lockes the handle message function until a message has arrived
queue<ReceivedMessage*> _queRcvMessages; // Hold the messages recieved from the user
User* Server::currentUser = NULL; // Holds the current user
string Server::username = " ";
SOCKET blockingSocket = INVALID_SOCKET;

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
	db->deleteAllNetworkUsers(); // On every run delete every known user on the network

	thread handleFiles(&LocalNetFunctions::receiveFile); // Wait until you get a file
	handleFiles.detach();

	thread t1(&Server::handleReceivedMessages, this); // Handle received messages from the user
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
	SOCKET newConnection;
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);

	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = ::accept(_serverSocket, (SOCKADDR*)&addr, &addrlen); // Accepting the client that wants to connect

	char *ip = inet_ntoa(addr.sin_addr);

	if (clientSocket == INVALID_SOCKET) // If the accept function was not successful
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;

	thread t1(&Server::clientHandler, this, clientSocket, ip); // Creating a thread of the client handler function
	t1.detach();
}

void Server::handleReceivedMessages()
{
	while (1)
	{
		std::unique_lock<std::mutex> lck(_mtxReceivedMessages);
		cv.wait(lck, []{return _queRcvMessages.size() != 0; }); // Wait until there are messages

		ReceivedMessage* msg = _queRcvMessages.front(); // Gets the message from the queue
		_queRcvMessages.pop();

		switch (msg->getMessageCode()) // Switch based on the type code
		{
		case Protocol::SIGN_IN_REQUEST:
			currentUser = handleSignin(msg);
			break;

		case Protocol::SIGN_OUT_REQUEST:
			handleSignout(msg);
			break;

		case Protocol::SIGN_UP_REQUEST:
			handleSignup(msg);
			break;

		case Protocol::UPLOAD_FILE_REQUEST:
		{
			thread sendFileThread(&Server::handleUploadFile, this, msg);
			sendFileThread.detach();
			break;
		}

		case Protocol::GET_USER_INFO_REQUEST:
			handleGetUserInfo(msg);
			break;

		case Protocol::CREATE_NEW_GROUP:
			handleCreateNewGroup(msg);
			break;

		case Protocol::ADD_USER_TO_GROUP:
		{
			thread addUserThread(&Server::handleAddUserToGroup, this, msg);
			addUserThread.detach();
			break;
		}

		case Protocol::GET_INFO_ABOUT_GROUPS:
			handleGetInfoAboutGroups(msg);
			break;

		case Protocol::UPLOAD_FILE_TO_GROUP:
			handleUploadFileToGroup(msg);
			break;

		case Protocol::GET_ALL_USERS:
			handleGetAllUsersInfo(msg);
			break;

		case Protocol::GET_WORK_END:
			handleFinishWork(msg);
			break;

		case Protocol::DELETE_USER_FROM_GROUP:
			handleDeleteUserFromGroup(msg);
			break;

		case Protocol::GET_ALL_FILES_INFO:
			handleGetAllFilesInfo(msg);
			break;

		case Protocol::GET_MY_FILE_BACK:
			handleGetFileBack(msg);
			break;

		case Protocol::REQUEST_TO_RETURN_FILE:
			handleRequestToReturnFile(msg);
			break;

		case Protocol::EXIT_GROUP:
			handleExitGroup(msg);
			break;

		case Protocol::DELETE_VHD:
			handleDeleteVHD(msg);
			break;

		case Protocol::NEW_GROUP_INFO:
			handleInsertNewGroup(msg);
			break;

		case Protocol::ASK_PERMIT:
			handleAskPermission(msg);
			break;
		}	
	}
}

void Server::clientHandler(SOCKET clientSocket, string IP)
{
	ReceivedMessage* msg = NULL;
	int typeCode = 0;

	try
	{
		do
		{
			typeCode = Helper::getMessageTypeCode(clientSocket);
			msg = buildRecieveMessage(clientSocket, typeCode, IP); // Builds the message
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

ReceivedMessage* Server::buildRecieveMessage(SOCKET clientSocket, int typeCode, string IP)
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
		length = Helper::getIntPartFromSocket(clientSocket, 3);
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

	else if (typeCode == Protocol::GET_USER_INFO_REQUEST)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string username = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string email = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string cloudSize = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(username);
		vec.push_back(email);
		vec.push_back(cloudSize);
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
		string groupName = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(username);
		vec.push_back(groupName);
	}

	else if (typeCode == Protocol::UPLOAD_FILE_TO_GROUP)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string groupName = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 3);
		string filePath = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(groupName);
		vec.push_back(filePath);
	}

	else if (typeCode == Protocol::DELETE_USER_FROM_GROUP)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string username = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string groupname = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(username);
		vec.push_back(groupname);
	}

	else if (typeCode == Protocol::GET_MY_FILE_BACK)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string fileName = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string atUser = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(fileName);
		vec.push_back(atUser);
	}

	else if (typeCode == Protocol::REQUEST_TO_RETURN_FILE)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string fileName = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(fileName);
	}

	else if (typeCode == Protocol::EXIT_GROUP)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string groupName = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(groupName);
	}

	else if (typeCode == Protocol::ASK_PERMIT)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 3);
		string message = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(message);
	}


	else if (typeCode == Protocol::NEW_GROUP_INFO)
	{
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string groupName = Helper::getStringPartFromSocket(clientSocket, length);
		length = Helper::getIntPartFromSocket(clientSocket, 2);
		string users = Helper::getStringPartFromSocket(clientSocket, length);

		vec.push_back(groupName);
		vec.push_back(users);
	}
	return new ReceivedMessage(clientSocket, typeCode, vec, IP);
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

		//string vhdPath = Helper::getCurrentPath() + "\\cloudFile.vhd"; // Converting the string to LPCWSTR
		//std::wstring stemp = Helper::s2ws(vhdPath);
		//LPCWSTR result = stemp.c_str();

		//int cloudSize = atoi(db->getUserCloudSize(msg->getValues()[0]).c_str());
		//CreateVirtualDriver(result, cloudSize * 1000 * 1000); // Multiplying by 1000^2 to turn it into gb
		connectBlockingSocket();

		this->username = msg->getValues()[0];
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
	string filePath = msg->getValues()[0]; // Get the file path that the user wants to send
	int encrypt = atoi(msg->getValues()[1].c_str()); // Get if the user wants to encrypt the file or not
	vector<string> ips;

	for (int i = 2; i < msg->getValues().size(); i++)
		ips.push_back(msg->getValues()[i]);

	string ip = ips[0]; // The computer that we send the file to
	string fileName = Helper::split(filePath, '\\').back();
	string message = username + " Wants to send you the file: " + fileName;

	int answer = LocalNetFunctions::askUserForPermission(ip, message);
	if (answer == 1)
	{
		if (ips.size() == 1)
		{
			LocalNetFunctions::sendFileToIp(strdup(ips.at(0).c_str()), strdup(filePath.c_str()));
		}
		else
		{
			LocalNetFunctions::uploadFileToGroup(filePath, encrypt, ips);
		}
	}
		else
	{
		string result = MsgEncrypt::Encipher("User refused to accept the file", "cipher");
		string data = to_string(Protocol::GENERAL_MESSAGE);
		data += Helper::getPaddedNumber(result.length(), 3);
		data += result;

		Helper::sendBlockingData(blockingSocket, data);
	}

	return 0;
}

void Server::handleGetUserInfo(ReceivedMessage* msg)
{	
	LocalNetFunctions::writeToIpsFile(msg->getIP(), msg->getValues()[0], msg->getValues()[1], msg->getValues()[2]);
	db->insertNetworkUser(msg->getValues()[0], msg->getValues()[1], msg->getValues()[2]);
	LocalNetFunctions::addUserToList(msg->getValues()[0], msg->getIP());

	if (currentUser)
	{
		string sendString = "102";
		sendString += Helper::getPaddedNumber(currentUser->getUserName().length(), 2);
		sendString += MsgEncrypt::Encipher(currentUser->getUserName(), KEY);
		sendString += Helper::getPaddedNumber(db->getUserEmail(currentUser->getUserName()).length(), 2);
		sendString += MsgEncrypt::Encipher(db->getUserEmail(currentUser->getUserName()), KEY);
		sendString += Helper::getPaddedNumber(getDriveFreeSpace().length(), 2);
		sendString += MsgEncrypt::Encipher(getDriveFreeSpace(), KEY);

		Helper::sendData(msg->getSock(), sendString);
	}
	else
	{
		string sendString = "102";
		sendString += MsgEncrypt::Encipher("04NULL", KEY);
		Helper::sendData(msg->getSock(), sendString);
	}
}

void Server::handleCreateNewGroup(ReceivedMessage* msg)
{
	db->insertNewGroup(msg->getValues()[0], msg->getValues()[1]);
}

void Server::handleAddUserToGroup(ReceivedMessage* msg)
{
	string ip = LocalNetFunctions::usernameToIP(msg->getValues()[0]);
	string message = "You are invited to join the group " + msg->getValues()[1];
	
	int answer = LocalNetFunctions::askUserForPermission(ip, message);
	if (answer == 1)
	{
		string users;
		string groupName = msg->getValues()[1];
		for (pair<string, string> group : db->getInfoAboutGroups())
		{
			if (group.first == groupName)
				users = group.second;
		}
		LocalNetFunctions::sendUserInfoAboutGroup(ip, groupName, users); // Sending the user info about the group

		db->addUserToGroup(msg->getValues()[1], msg->getValues()[0]); // Adding the user to our database
	}
	else
	{
		string result = MsgEncrypt::Encipher("User refused to join the group", "cipher");
		string data = to_string(Protocol::GENERAL_MESSAGE);
		data += Helper::getPaddedNumber(result.length(), 3);
		data += result;

		Helper::sendBlockingData(blockingSocket, data);
	}
}

void Server::handleGetInfoAboutGroups(ReceivedMessage* msg)
{
	vector<pair<string, string>> values = db->getInfoAboutGroups();

	int amount = 0;
	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].second.find(this->username) != string::npos) // Checks if the user is in the group 
			amount++;
	}

	string sendString = "103" + Helper::getPaddedNumber(to_string(amount).length(), 2) + to_string(amount);

	for (int i = 0; i < values.size(); i++)
	{
		if (values[i].second.find(this->username) != string::npos) // Checks if the user is in the group 
		{
			sendString += Helper::getPaddedNumber(values[i].first.length(), 2);
			sendString += MsgEncrypt::Encipher(values[i].first, KEY);
			sendString += Helper::getPaddedNumber(values[i].second.length(), 2);
			sendString += MsgEncrypt::Encipher(values[i].second, KEY);
		}
	}

	Helper::sendData(msg->getSock(), sendString);
}

string Server::getDriveFreeSpace()
{
	CHardDiskManager manager;
	manager.CheckFreeSpace(L"c:"); // TODO : Change to N

	return to_string(manager.GetTotalNumberOfFreeGBytes());
}

string Server::getCurrentUsername()
{
	if (currentUser)
	{
		return currentUser->getUserName();
	}
	return "";
}

SOCKET Server::getCurrentSocket()
{
	if (currentUser)
	{
		return currentUser->getSocket();
	}
	return NULL;
}

void Server::handleUploadFileToGroup(ReceivedMessage* msg)
{
	vector<pair<string, string>> groups = db->getInfoAboutGroups();
	vector<string> users;
	vector<string> ips;
	
	auto it = groups.begin();
	for (it; it != groups.end(); it++)
	{
		if (it->first.compare(msg->getValues()[0]) == 0)
		{
			users = LocalNetFunctions::split(it->second, '/');
		}
	}
	
	for (auto user : users)
	{
		if (LocalNetFunctions::usernameToIP(user).compare("Not Found") != 0)
			ips.push_back(LocalNetFunctions::usernameToIP(user));
	}

	LocalNetFunctions::uploadFileToGroup(msg->getValues()[1], 0, ips);
}

void Server::handleGetAllUsersInfo(ReceivedMessage* msg)
{
	vector<string> users = db->getAllNetUsersInfo();
	string data = "104";
	data += Helper::getPaddedNumber(to_string(users.size()).length(), 2);
	data += to_string(users.size());
	
	for (auto user : users)
	{
		data += Helper::getPaddedNumber(user.length(), 2);
		data += MsgEncrypt::Encipher(user, KEY);
	}

	Helper::sendData(msg->getSock(), data);
}

void Server::handleFinishWork(ReceivedMessage* msg)
{
	thread findIps(&LocalNetFunctions::getUsersOnNetwork, msg->getSock());
	findIps.detach();
}

void Server::handleDeleteUserFromGroup(ReceivedMessage* msg)
{
	db->deleteUserFromGroup(msg->getValues()[0], msg->getValues()[1]);
}

void Server::handleGetAllFilesInfo(ReceivedMessage* msg)
{
	vector<vector<string>> values = db->getAllFilesInfo();
	string data = "106";
	data += Helper::getPaddedNumber(to_string(values.size()).length(), 2);
	data += to_string(values.size());
	
	for (auto it : values)
	{
		data += Helper::getPaddedNumber(it[0].length(), 3);
		data += MsgEncrypt::Encipher(it[0], KEY);
		data += Helper::getPaddedNumber(it[1].length(), 2);
		data += MsgEncrypt::Encipher(it[1], KEY);
		data += Helper::getPaddedNumber(it[2].length(), 2);
		data += MsgEncrypt::Encipher(it[2], KEY);
		data += Helper::getPaddedNumber(it[3].length(), 2);
		data += MsgEncrypt::Encipher(it[3], KEY);
	}

	Helper::sendData(msg->getSock(), data);
}

void Server::handleGetFileBack(ReceivedMessage* msg)
{
	SOCKADDR_IN target;
	SOCKET clientSocket;

	string fileName = msg->getValues()[0];
	string ip = LocalNetFunctions::usernameToIP(msg->getValues()[1]);
	if (ip.compare("") == 0)
	{
		Helper::sendData(msg->getSock(), "1071");
		return;
	}
	
	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons(22223); //Port to connect on
	target.sin_addr.s_addr = inet_addr(ip.c_str()); //Target IP

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (clientSocket == INVALID_SOCKET)
	{
		Helper::sendData(msg->getSock(), "1072");
		return;
	}

	//Try connecting...
	if (connect(clientSocket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		Helper::sendData(msg->getSock(), "1072");
		return;
	}
	else
	{
		string data = "108";
		data += Helper::getPaddedNumber(fileName.length(), 2);
		data += MsgEncrypt::Encipher(fileName, KEY);
		Helper::sendData(clientSocket, data);
	}
}

void Server::handleRequestToReturnFile(ReceivedMessage* msg)
{
	// TODO : Change the file name to point to the filepath
	LocalNetFunctions::sendFileToIp(strdup(msg->getIP().c_str()), strdup(msg->getValues()[0].c_str()));
}

void Server::handleExitGroup(ReceivedMessage* msg)
{
	string groupName = msg->getValues()[0];
	db->deleteUserFromGroup(this->username, groupName);
}

void Server::handleDeleteVHD(ReceivedMessage* msg)
{
	string vhdPath = Helper::getCurrentPath() + "\\cloudFile.vhd"; // Converting the string to LPCWSTR
	deleteVirtualHardDriver(vhdPath);
}

void Server::connectBlockingSocket()
{
	SOCKADDR_IN target;

	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons(22222); //Port to connect on
	target.sin_addr.s_addr = inet_addr("127.0.0.1"); //Target IP

	blockingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (blockingSocket == INVALID_SOCKET)
		return;
	if (connect(blockingSocket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		cout << "Error connecting blocking socket." << endl;
	}
}

void Server::handleAskPermission(ReceivedMessage* msg)
{
	string data = to_string(Protocol::ADD_USER_TO_GROUP);
	string message = msg->getValues()[0];
	data += Helper::getPaddedNumber(message.length(), 3);
	data += MsgEncrypt::Encipher(message, "cipher");
	Helper::sendBlockingData(blockingSocket, data);

	int typeCode = Helper::getMessageTypeCode(blockingSocket);
	int answer = Helper::getIntPartFromSocket(blockingSocket, 1);
	data = to_string(typeCode) + to_string(answer);

	Helper::sendData(msg->getSock(), data);
}

void Server::handleInsertNewGroup(ReceivedMessage* msg)
{
	string groupName = msg->getValues()[0];
	vector<string> users = Helper::split(msg->getValues()[1], '/');

	db->insertNewGroup(groupName, "null");
	for (string username : users)
	{
		db->addUserToGroup(groupName, username);
	}
}