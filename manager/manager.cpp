#include "manager.h"

/*

PRIVATE

*/

// resize values array to match specified size
void Manager::resize(int size)
{
	// incase its nullptr
	if (values == nullptr)
	{
		values = new Manager[size + defaultSize];
		_size = 0;
		_arrSize = size + defaultSize;
		return;
	}

	// create new array of size size
	Manager* temp = new Manager[size];

	// copy values from old array to new
	int s = size < _size ? size : _size;
	for (int i = 0; i < s; i++)
	{
		temp[i] = values[i];
	}

	// delete old array
	delete[] values;

	// set values to new array
	values = temp;

	// update _arrSize & size
	_arrSize = size;
	_size = s;

	return;
}

// shift every element starting at index to the right in the array
void Manager::shiftRight(int index)
{
	if (_size == _arrSize)
		resize(_arrSize * 2);

	for (int i = _size; i >= index; i--)
	{
		values[i] = values[i - 1];
	}

	values[index] = Manager();
	_size++;
}

void Manager::shiftLeft(int index)
{
	for (int i = index; i < _size; i++)
	{
		values[i] = values[i + 1];
	}
	_size--;
}

/*

PROTECTED

*/

/*

PUBLIC

*/

// constructor(s)
Manager::Manager()
{
	key = "";
	values = nullptr;
	_arrSize = 0;
	_size = 0;
	censor = false;
}
Manager::Manager(std::string key)
{
	this->key = key;
	values = new Manager[defaultSize];
	_arrSize = defaultSize;
	_size = 0;
	censor = false;
}
Manager::Manager(std::string key, char metadata)
{
	this->key = key;
	values = new Manager[defaultSize];
	_arrSize = defaultSize;
	_size = 0;
	this->censor = 0x4 & metadata;
	/*
	= 0x2 & metadata 
	= 0x4 & metadata
	= 0x8 & metadata
	= 0x10 & metadata
	= 0x20 & metadata
	= 0x40 & metadata
	= 0x80 & metadata
	*/
}

// deconstructor(s)
Manager::~Manager()
{
	/*
	try
	{
		if (values != nullptr)
			delete[] values;
		values = nullptr;
	}
	catch (...)
	{
		std::cout << "deconstructor failed" << std::endl;
	}
	// */ 
}

std::string Manager::getKey()
{
	return key;
}
void Manager::setKey(std::string key)
{
	this->key = key;
}

void Manager::addItem(Manager e)
{
	if (values == nullptr)
	{
		resize(defaultSize);
	}
	if (_size == _arrSize)
		resize(_arrSize * 2);

	values[_size].values = e.values;
	values[_size]._arrSize = e._arrSize;
	//values[_size]._size = e._size;
	values[_size].key = e.key;
	values[_size].censor = e.censor;

	_size += 1;
}

void Manager::addItem(Manager e, int index)
{
	shiftRight(index);

	values[index] = e;
}

/*
return 1 when successful
return 0 when there are children
return -1 when doesn't exist
*/
int Manager::removeItem(Manager e)
{
	// stuck iterating through unless I use a hashmap
	for (int i = 0; i < _size; i++)
	{
		if (e == values[i])
		{
			if (values[i].size() > 0)
				return 0;
			shiftLeft(i);
			return 1;
		}
	}
	return -1;
}

/*
return 1 when successful
return 0 when there are children
return -1 when doesn't exist
*/
int Manager::removeItem(std::string key)
{
	for (int i = 0; i < _size; i++)
	{
		if (values[i].key == key)
		{
			if (values[i].size() > 0)
				return 0;

			shiftLeft(i);
			return 1;
		}
	}
	return -1;
}

/*
return 1 when successful
return 0 when there are children
return -1 when doesn't exist
*/
int Manager::removeItem(int index)
{
	if (index > this->_size)
		return -1;
	if (values[index].size() > 0)
		return 0;
	shiftLeft(index);
	return 1;
}

int Manager::size()
{
	return _size;
}
bool Manager::empty()
{
	return _size == 0;
}

Manager& Manager::operator [] (int i)
{
	return values[i];
}

void Manager::operator = (const Manager& e)
{
	this->values = e.values;
	this->key = e.key;
	this->_size = e._size;
	this->_arrSize = e._arrSize;
}

bool Manager::operator==(Manager e)
{
	return e.values == this->values &&
		e.key == this->key &&
		e._size == this->_size &&
		e._arrSize == this->_arrSize;
}