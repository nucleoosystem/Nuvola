#include "Database.h"

unordered_map<string, vector<string>> results;

int callback(void* notUsed, int argc, char** argv, char** azCol);

const char* fileName = "DB\\database.db";

Database::Database()
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	bool flag = true;

	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		string s = "Can't open database: ";
		s += sqlite3_errmsg(db);
		throw(exception(s.c_str()));
	}
}

Database::~Database()
{

}

bool Database::doesUserExists(string name)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	bool flag = true;
	string sqlQuery = " ";

	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
	}

	unordered_map<string, vector<string>>::iterator it;

	sqlQuery = "SELECT * FROM t_users";  
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return (1);
	}

	it = results.begin();

	for (size_t i = 0; it != results.end() && i < it->second.size(); i++)
	{
		if (it->second.at(i) == name)
		{
			results.clear();
			return true;  
		}
	}

	results.clear();
	sqlite3_close(db);

	return false;
}

bool Database::addNewUser(string username, string password, string email, string cloudSize)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return false;
	}

	sqlQuery = "INSERT INTO t_users (username, password, email, cloudSize) VALUES(\'";
	sqlQuery += username + "\',\'";
	sqlQuery += password + "\',\'";
	sqlQuery += email + "\',";
	sqlQuery += cloudSize + ")";

	rc = sqlite3_exec(db, sqlQuery.c_str(), NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return false;
	}

	sqlite3_close(db);

	return true;
}

bool Database::isUserAndPassMatch(string username, string password)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return false;
	}

	sqlQuery = "SELECT password FROM t_users WHERE username = '" + username + "'";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return false;
	}

	unordered_map<string, vector<string>>::iterator it;

	it = results.begin();
	if (it != results.end() && it->second.at(0) == password)
	{
		results.clear();
		return true;
	}

	results.clear();
	sqlite3_close(db);

	return false;
}

void Database::insertNetworkUser(string name, string email, string cloudSize)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
	}

	sqlQuery = "INSERT INTO t_otherUsers (username, email, cloudSize, active) VALUES(\'";
	sqlQuery += name + "\',\'";
	sqlQuery += email + "\',\'";
	sqlQuery += cloudSize + "\',";
	sqlQuery += "1)";

	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
	}

	sqlite3_close(db);
}

vector<string> Database::getInfoNetUser(string username)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	vector<string> values;

	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return values;
	}

	sqlQuery = "SELECT * FROM t_otherUsers where username = \"" + username + "\"";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return values;
	}

	unordered_map<string, vector<string>>::iterator it;

	for (auto it = results.begin(); it != results.end(); it++)
	{
		values.push_back(it->second.front());
	}
	results.clear();
	sqlite3_close(db);

	return values;
}

void Database::insertNewGroup(string name, string password)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
	}

	string thisUser = Server::getCurrentUsername();
	string cloudSize = Server::getDriveFreeSpace();

	sqlQuery = "INSERT INTO t_groups (group_name, users, password, storage, available_storage) VALUES(\'";
	sqlQuery += name + "\',\'";
	sqlQuery += thisUser + "\',\'";
	sqlQuery += password + "\',\'";
	sqlQuery += cloudSize + "\',\'";
	sqlQuery += cloudSize + "\')";

	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
	}

	sqlite3_close(db);
}

void Database::addUserToGroup(string groupName, string username)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
	}
	
	sqlQuery = "SELECT users FROM t_groups WHERE group_name = \"" + groupName + "\"";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
	}

	auto it = results.begin();
	if (it->second.size() > 0)
	{
		bool wasUserFound = false;
		string currentUsers = it->second[0];
		vector<string> usersSplit = Helper::split(currentUsers, '/');
		for (string user : usersSplit)
		{
			if (user.compare(username) == 0)
				wasUserFound = true;
		}
		if (!wasUserFound) // If the user is not located already in the group
		{
			currentUsers += "/" + username;

			sqlQuery = "UPDATE t_groups SET users = \"" + currentUsers + "\" WHERE group_name = \"" + groupName + "\"";
			rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
			if (rc != SQLITE_OK)
			{
				cout << "SQL error: " << zErrMsg << endl;
				sqlite3_free(zErrMsg);
				system("Pause");
			}
		}
	}

	results.clear();
	sqlite3_close(db);
}

vector<pair<string,string>> Database::getInfoAboutGroups()
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	vector<vector<string>> values;
	vector<pair<string, string>> groups;


	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return groups;
	}

	sqlQuery = "SELECT group_name, users FROM t_groups";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return groups;
	}
	
	int length = results.begin()->second.size();
	for (int i = 0; i < length; i++)
	{
		values.push_back(vector<string>());
	}

	unordered_map<string, vector<string>>::iterator it;
	int current = 0;
	for (auto it = results.begin(); it != results.end(); it++)
	{
		for (int i = 0; i < length; i++)
		{
			values[i].push_back(it->second.at(i));
		}
	}
	results.clear();

	for (int i = 0; i < values.size(); i++)
	{
		groups.push_back(pair<string, string>(values.at(i)[0], values.at(i)[1]));
	}

	sqlite3_close(db);

	return groups;
}

string Database::getUserEmail(string username)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return " ";
	}

	sqlQuery = "SELECT email FROM t_users WHERE username = '" + username + "'";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return " ";
	}

	unordered_map<string, vector<string>>::iterator it = results.begin();

	if (it != results.end())
	{
		string userEmail = it->second.at(0);
		results.clear();
		return userEmail;
	}	

	results.clear();
	sqlite3_close(db);

	return " ";
}

void Database::deleteAllNetworkUsers()
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
	}

	sqlQuery = "DELETE FROM t_otherUsers";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
	}

	results.clear();
	sqlite3_close(db);
}

vector<string> Database::getAllNetUsersInfo()
{
	vector<string> users;
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return users;
	}

	sqlQuery = "SELECT username FROM t_otherUsers";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return users;
	}

	unordered_map<string, vector<string>>::iterator it = results.begin();
	if (it != results.end())
	{
		for (int i = 0; i < it->second.size(); i++)
		{
			if (std::find(users.begin(), users.end(), it->second.at(i)) == users.end())
				users.push_back(it->second.at(i));
		}
	}

	results.clear();
	sqlite3_close(db);

	return users;
}

void Database::deleteUserFromGroup(string username, string groupName)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
	}

	sqlQuery = "SELECT users FROM t_groups WHERE group_name = \"" + groupName + "\"";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
	}

	auto it = results.begin();
	if (it->second.size() > 0)
	{
		string currentUsers = it->second[0]; // Get the currrent list of users
		vector<string> users = Helper::split(currentUsers, '/'); // Split them into a vector
		currentUsers = "";
		for (string user : users)
		{
			if (username.compare(user) != 0) // Insert all users but the user given as an argument
				currentUsers += user += "/";
		}
		if (currentUsers.length() > 0)
			currentUsers.pop_back(); // Pop the last '/' enterted

		sqlQuery = "UPDATE t_groups SET users = \"" + currentUsers + "\" WHERE group_name = \"" + groupName + "\"";
		rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			cout << "SQL error: " << zErrMsg << endl;
			sqlite3_free(zErrMsg);
			system("Pause");
		}
	}

	results.clear();
	sqlite3_close(db);
}

bool Database::addFileToDB(string name, string type, string user, string size)
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return false;
	}

	sqlQuery = "INSERT INTO t_files (filename, type, at_user, filesize) VALUES(\'";
	sqlQuery += name + "\',\'";
	sqlQuery += type + "\',\'";
	sqlQuery += user + "\',";
	sqlQuery += size + ")";

	rc = sqlite3_exec(db, sqlQuery.c_str(), NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return false;
	}

	sqlite3_close(db);

	return true;
}

vector<vector<string>> Database::getAllFilesInfo()
{
	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	string sqlQuery = " ";
	vector<vector<string>> values;


	rc = sqlite3_open(fileName, &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		system("Pause");
		return values;
	}

	sqlQuery = "SELECT * FROM t_files";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		system("Pause");
		return values;
	}

	int length = results.begin()->second.size();
	for (int i = 0; i < length; i++)
	{
		values.push_back(vector<string>());
	}

	unordered_map<string, vector<string>>::iterator it;
	int current = 0;
	for (auto it = results.begin(); it != results.end(); it++)
	{
		for (int i = 0; i < length; i++)
		{
			values[i].push_back(it->second.at(i));
		}
	}
	results.clear();

	sqlite3_close(db);

	return values;
}

int Database::callback(void* notUsed, int argc, char** argv, char** azCol)
{
	int i;

	for (i = 0; i < argc; i++)
	{
		auto it = results.find(azCol[i]);
		if (it != results.end())
		{
			it->second.push_back(argv[i]);
		}
		else
		{
			pair<string, vector<string>> p;
			p.first = azCol[i];
			p.second.push_back(argv[i]);
			results.insert(p);
		}
	}

	return 0;
}