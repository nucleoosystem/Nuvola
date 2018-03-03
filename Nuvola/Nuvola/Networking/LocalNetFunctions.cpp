#include "LocalNetFunctions.h"

const char* arpExe64Bit = "Logic\\arp-scan-64.exe";
const char* arpExe32Bit = "Logic\\arp-scan-86.exe";
const char* targetIpFileName = "..//..//NuvolaWPF//NuvolaWPF//bin//Debug//IP.txt";
const char* batFileName = "Networking\\GetLocalIps.bat";
const char* foundIpsFileName = "Networking\\foundIps.txt";

map<string, string> LocalNetFunctions::usersToIps;
vector<string> allLocalAddrs;

// Returns the local ip address and the ip mask of the computer
std::vector<std::pair<std::string, std::string>> LocalNetFunctions::getLocalIpAddress()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	std::vector<std::pair<std::string, std::string>> returnArr;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return returnArr;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return returnArr;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			string ipAddr(pAdapter->IpAddressList.IpAddress.String);
			string ipMask(pAdapter->IpAddressList.IpMask.String);
			string ipGateway(pAdapter->GatewayList.IpAddress.String);

			allLocalAddrs.push_back(ipAddr);

			/*if ((pAdapter->Type == IF_TYPE_IEEE80211 || pAdapter->Type == IF_TYPE_ETHERNET_CSMACD) && ipAddr != "0.0.0.0" && ipMask != "0.0.0.0")
			{
				if (split(ipAddr, '.')[0] == "192" || split(ipAddr, '.')[0] == "10")
				{
					std::pair<std::string, std::string> addrMask(ipAddr, ipMask);
					returnArr.push_back(addrMask);
				}
			}*/

			if (ipGateway.compare("0.0.0.0") != 0)
			{
				std::pair<std::string, std::string> addrMask(ipAddr, ipMask);
				returnArr.push_back(addrMask);
			}
			pAdapter = pAdapter->Next;
		}
	}
	else {
		printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);

	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return returnArr;
}

std::vector<std::string> LocalNetFunctions::split(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

void LocalNetFunctions::getIpAddrsFromFile(string fileName)
{
	vector<string> foundIps;
	ifstream file;
	string line;

	file.open(fileName);
	if (file.is_open())
	{
		while (file.eof() == false)
		{
			wchar_t wtext[20];
			file >> line;
			mbstowcs(wtext, line.c_str(), strlen(line.c_str()) + 1); /* Converting a string to a wchar_t */

			struct sockaddr_in sa;
			if (InetPton(AF_INET, wtext, &(sa.sin_addr)) != 0)
			{
				foundIps.push_back(line);
			}

		}
	}
	file.close();

	fstream guiIpsFile;
	guiIpsFile.open(targetIpFileName, fstream::out | fstream::trunc);

	vector<future<bool>> ipsAndResults;
	if (guiIpsFile.is_open())
	{
		for (auto&& ip : foundIps)
		{
			ipsAndResults.emplace_back(std::async(std::launch::async, isUserOnComputer, ip));
		}	

		for (unsigned int i = 0; i < ipsAndResults.size(); i++)
		{
			if (std::find(allLocalAddrs.begin(), allLocalAddrs.end(), foundIps[i]) == allLocalAddrs.end())
			{
				if (ipsAndResults[i].get()) // If active
				{
					vector<string> values = getUserInfo(foundIps[i]);
					if (values[0].compare("NULL") != 0)
					{
						guiIpsFile << foundIps[i] << " " << values[0] << " " << values[1] << " " << values[2] << endl;
						usersToIps[values[0]] = foundIps[i];

						Database* db = new Database();
						db->insertNetworkUser(values[0], values[1], values[2]);
					}
				}
			}
		}
	}
	guiIpsFile.close();
	cout << "\nFinished writing to file" << endl;
}

void LocalNetFunctions::getUsersOnNetwork(SOCKET userSocket)
{
	string exeName = arpExe64Bit;

	ofstream foundIpsFile;
	foundIpsFile.open(foundIpsFileName, ios::trunc);
	foundIpsFile.close();

	ofstream batFile;
	batFile.open(batFileName, ios::trunc);
	//batFile << "@echo off\n";
	batFile << "netsh interface ip delete arpcache\n";

	std::vector<pair<string, string>> ipAndSubnet; /* Gets the ip and the subnet of the local computer */

	ipAndSubnet = getLocalIpAddress();
	auto currentPair = ipAndSubnet.begin();
	for (currentPair; currentPair != ipAndSubnet.end(); currentPair++)
	{
		char* srcIpString = new char[strlen(currentPair->first.c_str()) + 1];
		char* subnetMask = new char[strlen(currentPair->second.c_str()) + 1];

		strncpy(srcIpString, currentPair->first.c_str(), strlen(currentPair->first.c_str()));
		srcIpString[strlen(currentPair->first.c_str())] = '\0';
		strncpy(subnetMask, currentPair->second.c_str(), strlen(currentPair->second.c_str()));
		subnetMask[strlen(currentPair->second.c_str()) + 1] = '\0';

		string dataToWrite = exeName + " -t " + srcIpString + "/" + getBitsInSubnet(subnetMask) + "\n";
		batFile << dataToWrite;
	}
	batFile << "arp -a > ";
	batFile << foundIpsFileName;
	batFile.close();
	cout << "starting" << endl;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.wShowWindow = SW_HIDE;

	LPCTSTR lpApplicationName = L"Networking\\GetLocalIps.bat";
	DWORD creationFlags = CREATE_NO_WINDOW;

	CreateProcess(lpApplicationName, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	cout << "Finished finding" << endl;
	getIpAddrsFromFile(foundIpsFileName);

	Helper::sendData(userSocket, "105");
}

string LocalNetFunctions::getBitsInSubnet(string subnet)
{
	vector<string> tokens = split(subnet, '.');
	int totalSum = 0;
	for (int i = 0; i < tokens.size(); i++)
	{
		string binary = std::bitset<8>(atoi(tokens[i].c_str())).to_string();
		size_t numberOfOnes = std::count(binary.begin(), binary.end(), '1');
		totalSum += numberOfOnes;
	}

	return to_string(totalSum);
}

bool LocalNetFunctions::isUserOnComputer(const string& ip)
{
	SOCKADDR_IN target; 
	SOCKET clientSocket;

	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons(22223); //Port to connect on
	target.sin_addr.s_addr = inet_addr(ip.c_str()); //Target IP

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (clientSocket == INVALID_SOCKET)
	{
		return false;
	}

	//Try connecting...

	if (connect(clientSocket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
}

vector<string> LocalNetFunctions::getUserInfo(const string& ip)
{
	Database* db = new Database();
	vector<string> values;
	SOCKADDR_IN target;
	SOCKET clientSocket;

	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons(22223); //Port to connect on
	target.sin_addr.s_addr = inet_addr(ip.c_str()); //Target IP

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (clientSocket == INVALID_SOCKET)
	{
		return values;
	}

	//Try connecting...
	if (connect(clientSocket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
	{
		return values;
	}
	else
	{
		string data = to_string(Protocol::GET_USER_INFO_REQUEST);
		data += Helper::getPaddedNumber(Server::getCurrentUsername().length(), 2);
		data += Server::getCurrentUsername();
		data += Helper::getPaddedNumber(db->getUserEmail(Server::getCurrentUsername()).length(), 2);
		data += db->getUserEmail(Server::getCurrentUsername());
		data += Helper::getPaddedNumber(Server::getDriveFreeSpace().length(), 2);
		data += Server::getDriveFreeSpace();

		Helper::sendData(clientSocket, data);
		Helper::getMessageTypeCode(clientSocket);
		int length = Helper::getIntPartFromSocket(clientSocket, 2);
		string username = Helper::getStringPartFromSocket(clientSocket, length);
		if (username.compare("NULL") != 0)
		{
			values.push_back(username);
			length = Helper::getIntPartFromSocket(clientSocket, 2);
			values.push_back(Helper::getStringPartFromSocket(clientSocket, length));
			length = Helper::getIntPartFromSocket(clientSocket, 2);
			values.push_back(Helper::getStringPartFromSocket(clientSocket, length));
		}
		else
		{
			values.push_back("NULL");
		}
		return values;
	}

	return values;
}

int LocalNetFunctions::uploadFileToGroup(string filePath, int encrypt, vector<string> ips)
{
	vector<pair<string, int>> ipsAndSizes;

	for (auto ip : ips)
		ipsAndSizes.push_back(pair<string, int>(ip, atoi(getUserInfo(ip)[2].c_str())));

	std::sort(ipsAndSizes.begin(), ipsAndSizes.end(), [](pair<string, int> &left, pair<string, int> &right) {
		return left.second > right.second;
	});

	sendFileToIp(strdup(ipsAndSizes[0].first.c_str()), strdup(filePath.c_str()));
	return 0;
}

int LocalNetFunctions::sendFileToIp(char* ip, char* path)
{
	WComm w;
	string rec = " ";
	const int port = 8888;

	// Connect To Server
	w.connectServer(ip, port);

	// Sending File
	w.sendData("FileSend");	w.recvData(strdup(rec.c_str()), 32);
	w.fileSend(path);

	// Send Close Connection Signal
	w.sendData("EndConnection"); w.recvData(strdup(rec.c_str()), 32);

	Database* db = new Database();
	db->addFileToDB(path, Helper::getFileExtension(path), LocalNetFunctions::IPToUsername(ip), to_string(Helper::getFileSize(path)));

	//delete rec;
	return 0;
}

string LocalNetFunctions::encryptFile(const string& path)
{
	return nullptr;
}

int LocalNetFunctions::receiveFile()
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
		cout << "Finished getting" << endl;
		// Disconnect client
		w.closeConnection();
	}

	return 0;
}

string LocalNetFunctions::usernameToIP(string username)
{
	if (usersToIps.find(username) != usersToIps.end())
	{
		return usersToIps[username];
	}
	else
	{
		return "Not Found";
	}
}

void LocalNetFunctions::addUserToList(string username, string IP)
{
	if (usersToIps.find(username) == usersToIps.end())
	{
		usersToIps[username] = IP;
	}
}

string LocalNetFunctions::IPToUsername(string ip)
{
	auto it = usersToIps.begin();
	for (it; it != usersToIps.end(); it++)
	{
		if (it->second.compare(ip) == 0)
			return it->first;
	}
	return "";
}

void LocalNetFunctions::writeToIpsFile(string ip, string username, string email, string cloudSize)
{
	fstream guiIpsFile;
	guiIpsFile.open(targetIpFileName, std::ios::app);

	if (guiIpsFile.is_open())
	{
		guiIpsFile << ip << " " << username << " " << email << " " << cloudSize << endl;
	}

	guiIpsFile.close();
}