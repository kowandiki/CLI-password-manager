#pragma once
#include <iostream>
#include "stack.h"

/*

passwords, once decrypted will be stored in a binary tree to allow
for easy lookup when called by their ID
left child = parent * 2
right child = parent * 2 + 1

*/

class BinaryTree
{
public:
	struct node
	{
		unsigned int id;
		std::string value;
		node* left = nullptr;
		node* right = nullptr;
		//parent isn't needed
		//node* parent = nullptr;
	};
private:
	const bool LEFT = false;
	const bool RIGHT = true;
	node* start = nullptr;
	node* insert(node* parent, bool right, std::string value);
	
public:
	BinaryTree();
	BinaryTree(std::string value);
	node* lookup(int id);
	bool insert(int id, std::string value);
	bool remove(int id);
	std::string getValue(int id);
	node* getStart() { return start; }
	
	stack getInstructions(int id);
	bool isEven(int num) { return num % 2 == 0; }
};