#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>

#include "sqlite3.h"
#include "../Logic/Server.h"

using namespace std;

class Server;

class Database
{
public:
	Database();
	~Database();
	bool doesUserExists(string name);
	bool addNewUser(string username, string password, string email, string cloudSize);
	bool isUserAndPassMatch(string username, string password);

	string getUserEmail(string username);
	string getUserCloudSize(string username);
	void insertNetworkUser(string name, string email, string cloudSize);
	vector<string> getInfoNetUser(string username);
	void deleteAllNetworkUsers();
	vector<string> getAllNetUsersInfo();

	void insertNewGroup(string name, string password);
	void addUserToGroup(string groupName, string username);
	vector<pair<string,string>>  getInfoAboutGroups();
	void deleteUserFromGroup(string username, string groupName);

	bool addFileToDB(string name, string type, string user, string size);
	vector<vector<string>> getAllFilesInfo();

	static int callback(void*, int, char**, char**);

private:
	sqlite3* db;
};