#include <iostream>

#include "Server.h"
#include "CreateVirtualDriver.h"
#include "Helper.h"

int main() 
{
	WSAInitializer wsaInit;
	Server* server;
	
	try
	{
		server = new Server();
		server->serve();

		string vhdPath = Helper::getCurrentPath() + "\\cloudFile.vhd";
		deleteVirtualHardDriver(vhdPath);
	}
	catch (exception& e)
	{
		std::cout << e.what() << std::endl;
		delete server;
	}

	delete server;

	return(0);
}