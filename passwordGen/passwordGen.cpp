#include "passwordGen.h"
/*

In order for the computer to generate a random number, use rand() function from cstdlib
Use srand((unsigned) time(0)); for seeding a different random number every time you start a new
game

*/
wchar_t passwordGenerator::generateLower()
{
	//generate random num in alphaSize range then return its index from lower character array
	return lower[(rand() % alphaSize)];
}
wchar_t passwordGenerator::generateUpper()
{
	return upper[(rand() % alphaSize)];
}
wchar_t passwordGenerator::generateNumber()
{
	return number[(rand() % numSize)];
}
wchar_t passwordGenerator::generateSpecial()
{
	return special[(rand() % specialSize)];
}
wchar_t passwordGenerator::generateCustom()
{
	if (custom.size() == 0)
	{
		return '\0';
	}
	return custom[(rand() % custom.size())];
}
std::string passwordGenerator::generatePassword(unsigned int size, unsigned int maxRepeats, bool lower, bool upper, bool number, bool special, bool custom)
{
	//if max repeats is 0 allow infinite repeats
	int index = 0;
	std::string character = "";
	character += lower ? this->lower : "";
	character += upper ? this->upper : "";
	character += number ? this->number : "";
	character += special ? this->special : "";
	character += custom ? this->custom : "";
	
	if ((index = character.size()) == 0)
		return "";

	std::string word = "";
	char* prev = new char[maxRepeats];
	for (int i = 0; i < maxRepeats; i++)
	prev[i] = '\0';
	char c;
	int count;
	for (int i = 0; i < size; i++)
	{
		count = 0;
		c = character[(rand() % index)];
		
		if (maxRepeats != 0)
		{
			for (int j = 0; j < maxRepeats; j++)
			{
				if (c == prev[j])
				{
					count++;
				}
			}
			if (count == maxRepeats)
			{
				i--;
				continue;
			}
			prev[i % maxRepeats] = c;
		}
		word += c;
		
	}
	delete[] prev;
	return word;
}