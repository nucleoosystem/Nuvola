#include "FileEncrypt.h"


void FileEncrypt::encryptFile(std::string inputFile, std::string outputFile, std::string key)
{
	CMyCNGCryptFile* c = new CMyCNGCryptFile();
	if (!c->CryptFile(true, inputFile.c_str(), outputFile.c_str(), key.c_str()))
		std::cout << "Error in encryptFile: " << c->GetLastError() << std::endl;
}

void decryptFile(std::string inputFile, std::string outputFile, std::string key)
{
	CMyCNGCryptFile* c = new CMyCNGCryptFile();
	if (!c->CryptFile(false, inputFile.c_str(), outputFile.c_str(), key.c_str()))
		std::cout << "Error in decryptFile: " << c->GetLastError() << std::endl;
}