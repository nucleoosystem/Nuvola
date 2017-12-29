#pragma once

#include <iostream>
#include <string>

#include "../Networking/WSAInitializer.h"
#include "Helper.h"

class User
{
public:
	User(std::string username, SOCKET socket);
	void send(string message); 
	std::string getUserName();
	SOCKET getSocket();

private:
	std::string username;
	SOCKET sock;
};