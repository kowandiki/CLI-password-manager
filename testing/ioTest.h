#pragma once

#include "..\manager\manager.h"
#include "..\io\fileio.h"
#include "display.h"

class ioTest
{
private:
	Manager m;
	std::string path;
public:
	void addItems();
	void toFile();
	void fromFile();
	void everything();
};