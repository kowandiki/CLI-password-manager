#pragma once
#include <iostream>
class passwordGenerator
{
private:
	//constants for different types of characters
	const std::string lower = "abcdefghijklmnopqrstuvwxyz";
	const std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const std::string number = "0123456789";
	const std::string special = "`~!@#$%^&*()-_+=[]{}\\|;:'\",<.>/?";
	
	//size of each array
	const unsigned int alphaSize = 26;
	const unsigned int numSize = 10;
	const unsigned int specialSize = 32;
public:
	//possibly use regex for this
	std::string custom;
	passwordGenerator()
	{
		srand((unsigned)time(0));
		this->custom = "";
	}
	passwordGenerator(std::string custom)
	{
		srand((unsigned)time(0));
		this->custom = custom;
	}
	wchar_t generateLower();
	wchar_t generateUpper();
	wchar_t generateNumber();
	wchar_t generateSpecial();
	wchar_t generateCustom();
	std::string generatePassword(unsigned int size, unsigned int maxRepeats, bool lower, bool upper, bool number, bool special, bool custom);
};