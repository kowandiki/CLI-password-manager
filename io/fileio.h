#pragma once

#include "..\manager\manager.h"
#include <fstream>
#include "..\testing/display.h"
#include "..\encryption\aes\aes.hpp"
#include "..\encryption\util\strUtil.h"
#include "..\encryption\gen\genIVSalt.h"

struct passSettings
{
	bool lower;
	bool upper;
	bool number;
	bool special;
	bool custom;
	unsigned int maxRepeat;
};

struct managerAndGenerator
{
	Manager* manager;
	unsigned int cycles;
	bool lower;
	bool upper;
	bool number;
	bool special;
	bool custom;
	unsigned int maxRepeat;
};

// returns true if it saves it to a file
bool managerToFile(Manager m, std::string path, std::string pass, unsigned int cycles, uint8_t* salt, uint8_t* iv);
// returns nullptr if it fails to read anything from file
managerAndGenerator fileToManager(std::string path, std::string pass);

void appendToFile(Manager m, std::ofstream* f);
void appendToFile(Manager m, std::ofstream* f, AES_ctx ctx);

//*
bool passToFile(passSettings settings, std::string path);
passSettings fileToPass(std::string path);
//*/
