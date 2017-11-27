#include "ReceivedMessage.h"

ReceivedMessage::ReceivedMessage(SOCKET clientSocket, int num)
{
	this->_sock = clientSocket;
	this->_messageCode = num;
}

ReceivedMessage::ReceivedMessage(SOCKET clientSocket, int num, vector<string> vec)
{
	this->_sock = clientSocket;
	this->_messageCode = num;
	this->_values = vec;
}

ReceivedMessage::~ReceivedMessage()
{

}

SOCKET ReceivedMessage::getSock()
{
	return this->_sock;
}

int ReceivedMessage::getMessageCode()
{
	return this->_messageCode;
}

vector<string>& ReceivedMessage::getValues()
{
	return this->_values;
}