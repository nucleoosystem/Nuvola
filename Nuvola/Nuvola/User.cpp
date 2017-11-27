#include "User.h"

User::User(std::string username, SOCKET socket)
{
	this->username = username;
	this->sock = socket;
}

void User::send(string message)
{
	Helper::sendData(sock, message);
}

std::string User::getUserName()
{
	return username;
}

SOCKET User::getSocket()
{
	return sock;
}