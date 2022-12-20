#include "strUtil.h"
#include <string>
#include <sstream>
#include "..\..\manager\mStack.h"
#include "..\..\testing/display.h"

strUtil::intArr strUtil::strToint8(std::string str)
{
	// create array of uint8_t, that is a multiple of 16 bytes
	// if it doesn't work perfectly into a multiple of 16, add 0s to end as padding	
	uint8_t* ret;
	unsigned int nSize;
	unsigned short remainder = str.size() % 16;
	nSize = ((bool)remainder * (16 - (str.size() % 16))) + str.size();

	ret = new uint8_t[nSize];

	for (int i = 0; i < str.size(); i++)
	{
		ret[i] = ((uint8_t)(0xff & str[i]));
	}
	for (int i = str.size(); i < nSize; i++)
	{
		ret[i] = 0x00;
	}

	return intArr{ nSize, ret};
}

std::string strUtil::int8ToStr(uint8_t* arr, unsigned int size)
{
	std::string str = "";

	for (unsigned int i = 0; i < size; i++)
	{
		str += (char)arr[i];
	}

	return str;
}

strUtil::intArr strUtil:: encryptStr(std::string str, uint8_t* key, uint8_t* iv, unsigned int cycles)
{
	intArr arr = strToint8(str);

	for (int i = 0; i < cycles; i++)
	{
		struct AES_ctx ctx;
		AES_init_ctx_iv(&ctx, key, iv);
		AES_CBC_encrypt_buffer(&ctx, arr.arr, arr.size);
	}

	return arr;
}


strUtil::intArr strUtil::decryptStr(std::string str, uint8_t* key, uint8_t* iv, unsigned int cycles)
{
	intArr arr = strToint8(str);

	for (int i = 0; i < cycles; i++)
	{
		struct AES_ctx ctx;
		AES_init_ctx_iv(&ctx, key, iv);
		AES_CBC_decrypt_buffer(&ctx, arr.arr, arr.size);
	}

	return arr;
}

/*
takes a byte array and encrypts it with AES CBC. key size is 32 bytes, iv size is 16 bytes
data length must be a multiple of 16. Length is NOT checked
*/
void strUtil::encrypt(uint8_t* arr, int size, uint8_t* key, uint8_t* iv, unsigned int cycles)
{
	for (int i = 0; i < cycles; i++)
	{
		struct AES_ctx ctx;
		AES_init_ctx_iv(&ctx, key, iv);
		AES_CBC_encrypt_buffer(&ctx, arr, size);
	}
}
void strUtil::encrypt(intArr arr, uint8_t* key, uint8_t* iv, unsigned int cycles)
{
	encrypt(arr.arr, arr.size, key, iv, cycles);
}

/*
takes a byte array and decrypts it with AES CBC. key size is 32 bytes, iv size is 16 bytes
data length must be a multiple of 16. Length is NOT checked
*/
void strUtil::decrypt(uint8_t* arr, int size, uint8_t* key, uint8_t* iv, unsigned int cycles)
{
	for (int i = 0; i < cycles; i++)
	{
		struct AES_ctx ctx;
		AES_init_ctx_iv(&ctx, key, iv);
		AES_CBC_decrypt_buffer(&ctx, arr, size);
	}
}
void strUtil::decrypt(intArr arr, uint8_t* key, uint8_t* iv, unsigned int cycles)
{
	decrypt(arr.arr, arr.size, key, iv, cycles);
}

/*
	Save an array in the intArr struct to increase ease of use.
	Automatically adds padding if needed
*/
strUtil::intArr strUtil::convertArr(uint8_t* arr, unsigned int size)
{
	addPadding(arr, size);
	return {size, arr};
}

/*
Add padding to arrays that are NOT multiples of 16
*/
void strUtil::addPadding(uint8_t* arr, int size)
{
	int mod = size % 16;
	if (mod == 0)
	{
		return;
	}
	uint8_t* newArr;

	int newSize = size;
	newSize += (16 - mod);

	// create new array
	newArr = new uint8_t[newSize];

	// assign all values from old array
	for (int i = 0; i < size; i++)
	{
		newArr[i] = arr[i];
	}
	// fill in everything else with padding
	for (int i = size; i < newSize; i++)
	{
		newArr[i] = 0;
	}

	// delete old array, change pointers
	delete[] arr;
	arr = newArr;

	return;
}


/*
Convert a manager to a string
*/
#define start 2
#define closer 1
std::string strUtil::managerToString(Manager m)
{
	// update metadata as needed for anything additional added to Manager
	// have 7 more bits to work with, first one will denote whether an entry is censored or not
	char metadata = -1 & (4 * m.censor);

	std::string str = std::string(1, start) + metadata + m.getKey();
	for (int i = 0; i < m.size(); i++)
	{
		str += managerToString(m[i]);
	}
	str += std::string(1, closer);
	return str;
}


/*
convert a string to a Manager
*/
Manager* strUtil::strToManager(std::string decryptedStr)
{
	if (decryptedStr.size() < 3)
		return nullptr;
	if (decryptedStr[0] != start)
		return nullptr;
	Mstack s = Mstack();
	s.push(new Manager("initial"));
	std::string str;

	std::istringstream iss(decryptedStr);
	// iterate through decrypted data using start as separator/delimiter
	while (std::getline(iss, str, (char)start))
	{
		int endCount = 0;
		// remove padding

		int i;

		for (i = str.size() - 1; i >= 0; i--)
		{
			if (str[i] == '\0')
				endCount++;
			else
				break;
		}
		str = str.substr(0, str.size() - endCount);
		endCount = 0;

		// see how many managers are opened/closed
		for (i = str.size() - 1; i >= 0; i--)
		{
			if (str[i] == (char)closer)
				endCount++;
			else
				break;
		}
		char metadata = str[0];

		// remove closer from ends of string, remove metadata too
		if (str.size() > 0)
			str = str.substr(1, str.size() - endCount - 1);
		
		if (endCount == 0)
		{
			// make child of top, add child to stack
			Manager m = Manager(str, metadata);
			s.top()->addItem(m);
			Manager* toAdd = s.top();
			toAdd = &(*toAdd)[toAdd->size() - 1];
			s.push(toAdd);
		}
		else if (endCount == 1)
		{
			// make child of top
			Manager m = Manager(str, metadata);
			s.top()->addItem(m);
		}
		else if (endCount > 1)
		{
			// make child of top, pop (endCount - 1) times
			Manager m = Manager(str, metadata);
			
			s.top()->addItem(m);
			for (i = endCount; i > 1; i--)
			{
				s.pop();
			}
		}
	}


	s.push(&(*s.top())[0]);
	return s.top();
}