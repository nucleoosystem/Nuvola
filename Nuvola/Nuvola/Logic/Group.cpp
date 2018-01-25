#include "Group.h"

Group::Group(std::string name, std::string password)
{
	this->name = name;
	this->password = password;
}

Group::~Group()
{

}

const std::string& Group::getGroupName()
{
	return this->name;
}

const std::vector<std::string>& Group::getUsers()
{
	return this->users;
}

void Group::addNewUser(const std::string& username)
{
	this->users.push_back(username);
}