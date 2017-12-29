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
	cout << doesUserExists("username") << endl;
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

	return false;
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
