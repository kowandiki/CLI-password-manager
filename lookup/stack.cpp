#include "stack.h"
#include <iostream>
using namespace std;

stack::stack()
{
	node* start = new node;
	start->prev = nullptr;
	this->start = start;
}

stack::stack(bool right)
{
	node* temp = new node;
	temp->right = right;
	start = temp;
	start->prev = nullptr;
	size++;
}
stack::stack(node* start)
{
	this->start = start;
}

int stack::pop()
{
	if (empty())
	{
		return -1;
	}

	size--;
	bool val = start->right;

	node* temp;
	temp = start;

	start = temp->prev;

	delete temp;

	return val;
}

void stack::append(bool right)
{
	node* toadd = new node;

	toadd->prev = start;
	toadd->right = right;

	start = toadd;

	this->size++;
}