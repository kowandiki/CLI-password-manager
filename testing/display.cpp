#pragma once

#include "display.h"
#include <string>

void displayManager(Manager m, int spaces, std::string prefix)
{
	std::cout << std::string(spaces, ' ') + prefix + (m.censor ? "********" : m.getKey()) << std::endl;
	for (int i = 0; i < m.size(); i++)
	{
		displayManager(m[i], spaces + 2, std::to_string(i) + " ");
	}
}