#pragma once

#include <iostream>
#include <vector>

class Validator {
public:
	static int isPasswordValid(std::string password);
	static int isUsernameValid(std::string username);
};