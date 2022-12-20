#pragma once

#include "manager.h"

struct node
{
	Manager* m;
	node* prev;
};

class Mstack
{
private:
	node* _top;
	int _size;
public:
	Mstack(Manager* m);
	Mstack();
	Manager* top();
	Manager* pop();
	void push(Manager* m);
	int size();
	bool empty();
	std::string toStr();
};