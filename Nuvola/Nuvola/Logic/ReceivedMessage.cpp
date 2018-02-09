#include "ReceivedMessage.h"

ReceivedMessage::ReceivedMessage(SOCKET clientSocket, int num, string IP)
{
	this->_sock = clientSocket;
	this->_messageCode = num;
	this->ip = IP;
}

ReceivedMessage::ReceivedMessage(SOCKET clientSocket, int num, vector<string> vec, string IP)
{
	this->_sock = clientSocket;
	this->_messageCode = num;
	this->_values = vec;
	this->ip = IP;
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

string ReceivedMessage::getIP()
{
	return this->ip;
}