#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../Logic/Validator.h"
#include "sqlite3.h"
#include "../Logic/Helper.h"
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

	static int callback(void*, int, char**, char**);

private:
	sqlite3* db;
};