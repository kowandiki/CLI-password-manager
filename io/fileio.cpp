#include "fileio.h"
#include "..\manager\mStack.h"

#include <string>
#include <sstream>


#define start 1
#define closer 2

static const uint8_t FileHeader[16] = { 0x03, 0xa2, 0xf5, 0x25, 0x9c, 0x11, 0x0d, 0x79, 0x5b, 0x03, 0x23, 0x94, 0x49, 0xfd, 0x54, 0x03 };

bool managerToFile(Manager m, std::string path, std::string pass, unsigned int cycles, uint8_t* salt, uint8_t* iv)
{
	std::ofstream file(path, std::ios_base::binary);

	// failed to open
	if (!file.is_open())
		return false;

	// generate key by hashing pass, salt (& maybe pepper)
	uint8_t* key = genKey(pass, salt, 32);

	// add file header to file [16 bytes]
	file.write((char*)FileHeader, 16);// << FileHeader;

	// add salt to file [32 bytes]
	file.write((char*)salt, 32);// << salt;

	// add iv to file [16 bytes]
	file.write((char*)iv, 16);// << iv;

	//*
	// add cycle count [4 bytes]
	uint8_t t[4] = {0,0,0,0};
	for (int i = 0; i < 4; i++)
	{
		t[i] = cycles >> (8 * i);
	}
	file.write((char*)t, 4);

	// add padding for consistency [12 bytes]
	char* padding = new char[12]{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00 };
	file.write(padding, 12);
	delete[] padding;
	// */


	// add encrypted data
	// also re-adding file header to confirm when its been unencrypted
	std::string toEncrypt = "";
	for (int i = 0; i < 16; i++)
	{
		toEncrypt += (0xff & FileHeader[i]);
	}
	toEncrypt += strUtil::managerToString(m);

	strUtil::intArr encrypted = strUtil::strToint8(toEncrypt);


	strUtil::encrypt(encrypted, key, iv, cycles);

	file.write((char*)encrypted.arr, encrypted.size);
	file.close();

	delete[] encrypted.arr;

	return true;
}

unsigned int getFileSize(std::ifstream* file)
{
	unsigned int count = 0;


	return count;
}

managerAndGenerator fileToManager(std::string path, std::string pass)
{
	// open file
	std::ifstream file(path, std::ios_base::binary);

	// get size of file
	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	managerAndGenerator ret = { nullptr, 0, false, false, false,false,false,0};

	// check that the file is even big enough
	if (fileSize < 80)
		return ret;

	// get data from file
	uint8_t* data = new uint8_t[fileSize];
	file.read((char*)data, fileSize);


	int i;

	// check that the file header is correct
	for (i = 0; i < 16; ++i)
	{
		if (FileHeader[i] != data[i])
		{
			// std::cout << "invalid header" << std::endl;
			// cleanup memory
			delete[] data;
			return ret;
		}
	}
	// if it matches->
	// grab salt (following 32 bytes)
	uint8_t salt[32];
	for (i = 16; i < 48; i++)
	{
		salt[i - 16] = data[i];
	}

	// grab iv (next 16 bytes after salt)
	uint8_t iv[16];
	for (i = 48; i < 64; i++)
	{
		iv[i - 48] = data[i];
	}

	// grab cycles (4 bytes after iv, 12 bytes of padding)
	unsigned int cycles = 0;
	cycles |= data[67];
	cycles <<= 8;
	cycles |= data[66];
	cycles <<= 8;
	cycles |= data[65];
	cycles <<= 8;
	cycles |= data[64];
	
	// generate decryption key
	// std::string saltStr = strUtil::int8ToStr(salt, 32);
	uint8_t* key = genKey(pass, salt, 32);

	// create new ptr to 81st element in data, treat as new array to avoid unwanted info

	strUtil::intArr decrypted = { (int)fileSize - 80, &data[80] };

	// decrypt and update decrypted
	strUtil::decrypt(decrypted, key, iv, cycles);

	// check that file header matches to confirm decryption
	for (i = 0; i < 16; i++)
	{
		// force positive values
		if ((uint8_t)(0xff & decrypted.arr[i]) != FileHeader[i])
		{
			// std::cout << "failed to decrypt" << std::endl;
			return ret;
		}
	}
	// if they do match, create manager same as normal
	
	// convert byte array to a string, remove header
	std::string decryptedStr = strUtil::int8ToStr(&decrypted.arr[16], decrypted.size - 16);

	// cleanup memory
	delete[] data;
	
	file.close();
	ret.cycles = cycles;
	ret.manager = strUtil::strToManager(decryptedStr);

	return ret;
}

bool passToFile(passSettings settings, std::string path)
{
	std::ofstream file(path);

	if (!file.is_open())
		return false;

	file << "lower: " << (settings.lower ? "true" : "false");
	file << '\n';
	file << "upper: " << (settings.upper ? "true" : "false");
	file << '\n';
	file << "number: " << (settings.number ? "true" : "false");
	file << '\n';
	file << "special: " << (settings.special ? "true" : "false");
	file << '\n';
	file << "custom: " << (settings.custom ? "true" : "false");
	file << '\n';
	file << "maxRepeat: " << std::to_string(settings.maxRepeat);

	file.close();

	return true;
}

passSettings fileToPass(std::string path)
{
	std::ifstream file(path);

	if (!file.is_open())
		return passSettings{true,true,true,true,false,2};

	std::string str;
	std::string com;
	std::string res;
	passSettings ret = { true,true,true,true,false,2 };
	while (std::getline(file, str))
	{
		int pos = str.find(" ");
		com = str.substr(0, pos);
		res = str.substr(pos + 1, str.size() - pos);
		if (com == "lower:")
		{
			ret.lower = (res == "true");
		}
		else if (com == "upper:")
		{
			ret.upper = (res == "true");
		}
		else if (com == "number:")
		{
			ret.number = (res == "true");
		}
		else if (com == "special:")
		{
			ret.special = (res == "true");
		}
		else if (com == "custom:")
		{
			ret.custom = (res == "true");
		}
		else if (com == "maxRepeat:")
		{
			ret.maxRepeat = std::stoi(res);
		}
		else
		{
			std::cout << "\"" << com << "\"" << " doesn't match any commands" << std::endl;
		}
	}

	return ret;
}
