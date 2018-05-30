#include "FileEncrypt.h"


void FileEncrypt::encryptFile(std::string inputFile, std::string outputFile, std::string key)
{
	EncryptFileMain(inputFile, outputFile, key);
}

void FileEncrypt::decryptFile(std::string inputFile, std::string outputFile, std::string key)
{
	EncryptFileMain(inputFile, outputFile, key);
}