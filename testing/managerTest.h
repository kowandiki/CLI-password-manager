#include "..\manager\manager.h"

class managerTest
{
private:
	Manager m;
public:
	managerTest();
	void addItems();
	void removeItems();
	void displayItems();
	void modifyItem(int index);
	void everything();
};