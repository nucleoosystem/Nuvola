#include "Validator.h"

int Validator::isPasswordValid(std::string password)
{
	std::vector<char> forbidden = { '&', '<', '>', '\"', '\'', '/', '\\' };
	
	for (size_t i = 0; i < password.length(); i++)
	{
		if (std::find(forbidden.begin(), forbidden.end(), password[i]) != forbidden.end())
		{
			return 0;
		}
	}

	return 1;
}


int Validator::isUsernameValid(std::string username)
{
	std::vector<char> forbidden = { '&', '<', '>', '\"', '\'', '/', '\\' };

	for (size_t i = 0; i < username.length(); i++)
	{
		if (std::find(forbidden.begin(), forbidden.end(), username[i]) != forbidden.end())
		{
			return 0;
		}
	}

	return 1;
}