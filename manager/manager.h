#pragma once

#include <iostream>
// building block of password storage,
// current setup allows for theoretically limitless nesting
// will realistically limit to profiles -> categories -> groups -> entries

class Manager
{
private:
	const unsigned int defaultSize = 8;
	unsigned int _arrSize;
	void resize(int size);
	void shiftRight(int index);
	void shiftLeft(int index);
protected:
	Manager* values;

	std::string key;

	unsigned int _size;

public:
	bool censor = false;

	Manager();
	Manager(std::string key);
	Manager(std::string key, char metadata);
	~Manager();
	std::string getKey();
	void setKey(std::string key);

	void addItem(Manager e);
	void addItem(Manager e, int index);

	/*
	return 1 when successful
	return 0 when there are children
	retrun -1 when doesn't exist
	*/
	int removeItem(Manager e);
	int removeItem(std::string key);
	int removeItem(int index);

	int size();
	bool empty();

	std::string toStr();

	Manager& operator [] (int i);
	void operator = (const Manager& e);
	bool operator == (Manager e);
};