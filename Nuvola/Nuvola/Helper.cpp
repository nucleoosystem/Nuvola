#include "Helper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// recieves the type code of the message from socket (3 bytes)
// and returns the code. if no message found in the socket returns 0 (which means the client disconnected)
int Helper::getMessageTypeCode(SOCKET sc)
{
	char* s = "";

	try
	{
		s = getPartFromSocket(sc, 3);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}

	std::string msg(s);

	if (msg == "")
		return 0;

	int res = std::atoi(s);
	delete s;
	return  res;
}

// recieve data from socket according byteSize
// returns the data as int
int Helper::getIntPartFromSocket(SOCKET sc, int bytesNum)
{
	char* s = "";
	try
	{
		s = getPartFromSocket(sc, bytesNum, 0);
	}
	catch (std::exception e)
	{
		cout << e.what() << endl;
	}
	return atoi(s);
}

// recieve data from socket according byteSize
// returns the data as string
string Helper::getStringPartFromSocket(SOCKET sc, int bytesNum)
{
	char* s = "";
	try
	{
		s = getPartFromSocket(sc, bytesNum, 0);
	}
	catch (std::exception e)
	{
		cout << e.what() << endl;
	}
	string res(s);
	return res;
}

// return string after padding zeros if necessary
string Helper::getPaddedNumber(int num, int digits)
{
	std::ostringstream ostr;
	ostr << std::setw(digits) << std::setfill('0') << num;
	return ostr.str();

}

// recieve data from socket according byteSize
// this is private function
char* Helper::getPartFromSocket(SOCKET sc, int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

char* Helper::getPartFromSocket(SOCKET sc, int bytesNum, int flags)
{
	if (bytesNum == 0)
	{
		return "";
	}

	char* data = new char[bytesNum + 1];
	int res = recv(sc, data, bytesNum, flags);

	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}

	data[bytesNum] = 0;
	return data;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, std::string message)
{
	const char* data = message.c_str();

	if (send(sc, data, message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}

// Uses the transmit file function to transmit a file
int Helper::TransmitFileFunction(SOCKET sc, HANDLE hFile, DWORD dwFlags, DWORD nNumberOfBytesToWrite, 
	DWORD nNumberOfBytesPerSend, LPOVERLAPPED lpOverlapped, LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers)
{	
	DWORD fileSize = GetFileSize(hFile, NULL);
	if (nNumberOfBytesToWrite == 0)
		nNumberOfBytesToWrite = fileSize;

	/**
	 *	Sending the file size so that the client will know the size of the file being sent to him
	 */

	Helper::sendData(sc, to_string(fileSize)); 

	int res = 0;
	try
	{
		/**
		 * Transmiting the file to the client
		 */
		res = TransmitFile(sc, hFile, nNumberOfBytesToWrite, 1024, lpOverlapped, lpTransmitBuffers, dwFlags);
		closesocket(sc);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << endl;
		return 0;
	}
	
	return res;
}

void Helper::receiveFile(ReceivedMessage* msg)
{
	std::ofstream file;
	int size = std::atoi(msg->getValues()[0].c_str());
	int currentSize = 0;
	int received = 0;
	file.open("received.jpg", std::ios::out | std::ofstream::trunc);
	if (file.is_open())
	{
		while (currentSize < size)
		{
			char* buffer = new char[1024];
			received = recv(msg->getSock(), buffer, 1024, 0);
			file.write(buffer, received);
			currentSize += received;
		}
	}

	file.close();
}