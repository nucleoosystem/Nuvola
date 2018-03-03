#pragma once

#include <iostream>
#include <string>

#include "Crypt.h"

class FileEncrypt
{
public:
	static void encryptFile(std::string inputFile, std::string outputFile, std::string key);
	static void decryptFile(std::string inputFile, std::string outputFile, std::string key);
};