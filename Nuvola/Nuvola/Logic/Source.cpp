#include <iostream>

#include "Server.h"

int main() 
{
	WSAInitializer wsaInit;
	Server* server;

	try
	{
		server = new Server();
		server->serve();
	}
	catch (exception& e)
	{
		std::cout << e.what() << std::endl;
		delete server;
	}

	delete server;

	return(0);
}