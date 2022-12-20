#include "mStack.h"

Mstack::Mstack(Manager* m)
{
	_top = new node{ m, nullptr };
	_size = 1;
}
Mstack::Mstack()
{
	_size = 0;
	_top = nullptr;
}

Manager* Mstack::top()
{
	return _top->m;
}

Manager* Mstack::pop()
{
	if (_top == nullptr)
	{
		return nullptr;
	}
	_size--;
	Manager* ret = _top->m;
	node* p = _top->prev;

	delete[] _top;
	_top = p;

	return ret;
}
void Mstack::push(Manager* m)
{
	_size++;
	node* n = new node;
	n->m = m;
	n->prev = _top;
	_top = n;
}
int Mstack::size() { return _size; }
bool Mstack::empty() { return _size == 0; }

std::string Mstack::toStr()
{
	std::string str = "";
	node* c = _top;
	for (int i = 0; i < _size; i++)
	{
		str.insert(0, c->m->getKey() + "/");

		c = c->prev;
	}
	return str;
}