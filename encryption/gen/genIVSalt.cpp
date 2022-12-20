#include "genIVSalt.h"
#include "..\hash\Sha.h"
#include "..\util\strUtil.h"

std::string genSalt(std::string str)
{
	return strUtil::int8ToStr(sha256(str), 32);
}
uint8_t* genIV(std::string str)
{
	return sha256(str);
}

// returns 32 byte array
uint8_t* genSalt(uint8_t* in, unsigned int size)
{
	//return sha256(in, size);
	return sha256(strUtil::int8ToStr(in, size));
}

// returns 16 byte array
uint8_t* genIV(uint8_t* in, unsigned int size)
{
	uint8_t* out = new uint8_t[16];
	uint8_t* mid = sha256(strUtil::int8ToStr(in, size));

	for (int i = 0; i < 16; i++)
		out[i] = mid[i];

	return out;
}

uint8_t* genKey(std::string pass, std::string salt)
{
	// combine pass and salt & pepper if there is one then hash
	return sha256(pepper + pass + salt);
}

uint8_t* genKey(std::string pass, uint8_t* salt, unsigned int size)
{
	unsigned int midSize = pass.size() + size;
	uint8_t* mid = new uint8_t[midSize];
	uint8_t* upass = strUtil::strToint8(pass).arr;

	for (int i = 0; i < pass.size(); i++)
	{
		mid[i] = upass[i];
	}
	for (int i = pass.size(); i < midSize; i++)
	{
		mid[i] = salt[i - pass.size()];
	}

	uint8_t* out = sha256(strUtil::int8ToStr(mid, midSize));
	delete[] upass;
	delete[] mid;

	return out;
}