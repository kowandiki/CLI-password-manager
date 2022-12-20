#pragma once
#include "..\aes\aes.hpp"
#include "..\..\manager\manager.h"
#include <iostream>

namespace strUtil
{
	struct intArr
	{
		unsigned int size;
		uint8_t* arr;
	};

	intArr strToint8(std::string str);

	std::string int8ToStr(uint8_t* arr, unsigned int size);


///	std::string encryptStr(std::string str, uint8_t* key, uint8_t* iv, unsigned int cycles);
	intArr encryptStr(std::string str, uint8_t* key, uint8_t* iv, unsigned int cycles);
//	std::string decryptStr(std::string str, uint8_t* key, uint8_t* iv, unsigned int cycles);
	intArr decryptStr(std::string str, uint8_t* key, uint8_t* iv, unsigned int cycles);

	// ones that don't take strings
	void encrypt(uint8_t* arr, int size, uint8_t* key, uint8_t* iv, unsigned int cycles);
	void encrypt(intArr arr, uint8_t* key, uint8_t* iv, unsigned int cycles);
	void decrypt(uint8_t* arr, int size, uint8_t* key, uint8_t* iv, unsigned int cycles);
	void decrypt(intArr arr, uint8_t* key, uint8_t* iv, unsigned int cycles);

	void addPadding(uint8_t* arr, int size);

	intArr convertArr(uint8_t* arr, unsigned int size);

	Manager* strToManager(std::string str);

	std::string managerToString(Manager m);

}

