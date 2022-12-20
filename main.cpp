#include "cli\cli.h"

/*
#include "encryption\aes\aes.hpp" // 

#include <iostream>
#include "encryption\util\strUtil.h"

#include "encryption\hash\Sha.h"

#include "testing\ioTest.h"
#include "manager\mStack.h"

#include <fstream>
// */

int main()
{
	/*
	Mstack s;
	Manager m("one");
	s.push(&m);

	m.addItem(Manager("two"));
	m.addItem(Manager("three"));
	m[0].addItem(Manager("fourer"));

	//*
	
	uint8_t* saltSeed = sha256("abc");
	uint8_t* IVSeed = sha256("cba");
	//managerToFile(*s.top(), "test.txt", "pass", 10000, genSalt(saltSeed, 32), genIV(IVSeed, 16));
	std::string extra = "hewwo";
	managerToFile(
		*s.top(), // m
		"test.txt", // path
		"pass", // pass
		1000, // cycles
		genSalt(sha256(extra.substr(0, extra.size() / 2)), 32),//salt
		genIV(sha256(extra.substr(extra.size() / 2, extra.size() / 2)), 16)); // iv


	Manager* m2 = fileToManager("test.txt", "pass");
	
	if (m2 == nullptr)
	{ 
		std::cout << "nullptr";
	}
	else
	{
		std::cout << "not nullptr" << std::endl;
		std::cout << "key: " << m2->getKey() << std::endl;
		std::cout << strUtil::managerToString(*m2);
	}
	// */
	takeInputs(); 
	return 0;
}
