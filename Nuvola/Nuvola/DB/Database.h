#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <unordered_map>
#include <ctime>

using namespace std;

class Database
{
public:
	Database();
	~Database();
	bool doesUserExists(string name);
	bool addNewUser(string username, string password, string email, string cloudSize);
	bool isUserAndPassMatch(string username, string password);

	vector<string> getCurrentUserInfo();
	void insertNetworkUser(string name, string email, string cloudSize);
	vector<string> getInfoNetUser(string username);

	void insertNewGroup(string name, string password);
	void addUserToGroup(string groupName, string username);
	vector<pair<string,string>>  getInfoAboutGroups();

	static int callback(void*, int, char**, char**);

private:
	sqlite3* db;
};