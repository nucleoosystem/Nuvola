#include <iostream>
#include <string>
#include <ctype.h>

using namespace std;

class MsgEncrypt
{
public:
	MsgEncrypt();
	~MsgEncrypt();

	static string Encipher(string input, string key);
	static string Decipher(string input, string key);

private:
	static int Mod(int a, int b);
	static string Cipher(string input, string key, bool encipher);
};
