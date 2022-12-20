#pragma once
class stack
{
public:
	struct node
	{
		node* prev;
		bool right;
	};
private:
	int size = 0;
	stack::node* start = nullptr;
public:
	stack();
	stack(bool right);
	stack(node* start);
	int pop();
	void append(bool right);
	bool top() { return start->right; }
	bool empty() { return size == 0; }
	node* ptr() { return start; }
};