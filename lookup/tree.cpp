
#include "tree.h"
using namespace std;

/*

passwords, once decrypted will be stored in a binary tree to allow
for easy lookup when called by their ID
left child = parent * 2
right child = parent * 2 + 1
will be a linked structure to allow for easy scaling at cost of speed
instructions to reach specified id & info will be stored in a stack

*/

BinaryTree::BinaryTree()
{
	this->start = new node;
	//start->parent = nullptr;
	start->id = 1;
	start->left = nullptr;
	start->right = nullptr;
}
BinaryTree::BinaryTree(std::string value)
{
	this->start = new node;
	//start->parent = nullptr;
	start->id = 1;
	start->left = nullptr;
	start->right = nullptr;
	start->value = value;
}

bool BinaryTree::insert(int id, string value)
{
	//dont allow empty nodes to be inserted
	if (value == "")
		return false;

	node* location = lookup(id);
	location->value = value;
	return true;
}

BinaryTree::node* BinaryTree::insert(node* parent, bool right, string value)
{
	//meant exclusively for creating nodes necessary for lookup
	//create new node & set it as the appropriate child
	// set value
	//then return ptr to new node
	node* child = new node;
	if (right == this->RIGHT)
		parent->right = child;
	else
		parent->left = child;
	child->value = value;
	return child;
}

BinaryTree::node* BinaryTree::lookup(int id)
{
	//get instructions to reach specified id
	//if a nullptr is reached, insert new node

	//getting instructions:
	//see if its even (yes: append 0 to stack | no: append 1 to stack)

	if (id == 1)
	{
		return start;
	}

	stack instructions = getInstructions(id);

	//now that instructions exist, need to go back through them
	//and create new nodes when needed
	node* temp = getStart();
	while (!instructions.empty())
	{
		//create new child as needed & navigate along instructions
		if (instructions.pop())
		{
			if (temp->right == nullptr)
			{
				temp->right = new node; //insert(temp, this->RIGHT, "");
				temp->right->value = "";
			}
			//set temp to its child
			temp = temp->right;
		}
		else
		{
			if (temp->left == nullptr)
			{
				temp->left = new node; //insert(temp, this->LEFT, "");
				temp->left->value = "";
			}
			temp = temp->left;
		}
	}
	return temp;
}

string BinaryTree::getValue(int id)
{
	return lookup(id)->value;
}

stack BinaryTree::getInstructions(int id)
{
	stack instructions;
	if (isEven(id))
		instructions = stack(this->LEFT);
	else
		instructions = stack(this->RIGHT);
	//id--; // not necessary because floats round down
	id /= 2;
	while (id > 1)
	{
		if (isEven(id))
			instructions.append(this->LEFT);
		else
			instructions.append(this->RIGHT);
		id /= 2;
	}
	return instructions;
}