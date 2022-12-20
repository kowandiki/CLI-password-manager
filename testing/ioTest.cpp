#pragma once

#include "ioTest.h"

void ioTest::addItems()
{
	Manager m2 = Manager("key");
	m2.addItem(Manager("item1"));
	m2.addItem(Manager("item2"));
	m2.addItem(Manager("item3"));
	m2.addItem(Manager("item4"));

	m2[0].addItem(Manager("child 1"));
	m2[0].addItem(Manager("child 2"));
	m2[0].addItem(Manager("child 3"));

	m2[1].addItem(Manager("child 1"));
	m2[1].addItem(Manager("child 2"));
	m2[1].addItem(Manager("child 3"));

	m = m2;

	displayManager(m2,0, "");
}

void ioTest::toFile()
{
	//managerToFile(m, "text.txt", "password", 1);
}

void ioTest::fromFile()
{
	//m = *fileToManager("text.txt");
	displayManager(m, 0, "");
}

void ioTest::everything()
{
	std::cout << std::string(32, '-') << std::endl;
	addItems();

	std::cout << std::string(32, '-') << std::endl;
	toFile();
	std::cout << std::string(32, '-') << std::endl;
	fromFile();
	std::cout << std::string(32, '-') << std::endl;
}