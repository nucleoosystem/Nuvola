#include <iostream>
#include <string>
#include <vector>

class Group
{
private:
	std::string name;
	std::vector<std::string> users;
	std::string password;

public:
	Group(std::string name, std::string password = nullptr);
	~Group();

	const std::string& getGroupName();
	const std::vector<std::string>& getUsers();

	void addNewUser(const std::string& username);
};