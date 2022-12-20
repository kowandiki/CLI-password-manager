#include "managerTest.h"

managerTest::managerTest()
{
	m = Manager("first key");
	std::cout << m.getKey() << std::endl;
	m.setKey("this is the key");
	std::cout << m.getKey() << std::endl;
}

void managerTest::addItems()
{
	std::cout << "add items" << std::endl;
	m.addItem(Manager("item 0"));
	//*
	m.addItem(Manager("item 1"));
	m.addItem(Manager("item 2"));
	m.addItem(Manager("item 3"));
	m.addItem(Manager("item 4"));
	m.addItem(Manager("item 5"));
	m.addItem(Manager("item 6"));
	m.addItem(Manager("item 7"));
	m.addItem(Manager("item 8"));
	m.addItem(Manager("index 3"), 3);

	//*/
}

void managerTest::removeItems()
{
	std::cout << "remove items" << std::endl;
	m.removeItem("item 5");
	m.removeItem(m[3]);

	while (!m.empty())
	{
		m.removeItem(0);
	}
}

void managerTest::modifyItem(int index)
{
	std::string str = m[index].getKey();
	str += " modded";
	m[index].setKey(str);
}

void managerTest::displayItems()
{
	std::cout << m.getKey() << std::endl;
	for (int i = 0; i < m.size(); i++)
	{
		std::cout << "\t" << m[i].getKey() << std::endl;
		if (m[i].size() > 0)
		{
			for (int j = 0; j < m[i].size(); j++)
			{
				std::string key;
				key = m[i][j].getKey();
				//key = m.values[i].values[j].getKey();
				std::cout << "\t\t" << key << std::endl;

				if (m[i][j].size() > 0)
				{
					for (int k = 0; k < m[i][j].size(); k++)
					{
						std::cout << "\t\t\t" << m[i][j][k].getKey() << std::endl;
					}
				}
			}
		}
	}
}

void managerTest::everything()
{
	addItems();
	std::cout << "add nested item" << std::endl;
	
	m[0].addItem(Manager("new item 1"));
	m[0].addItem(Manager("new item 2"));
	m[0].addItem(Manager("new item 3"));

	m[0][0].addItem(Manager("new new item 1"));
	m[0][0].addItem(Manager("new new item 2"));
	m[0][0].addItem(Manager("new new item 3"));

	m[4].addItem(Manager("new item 1"));
	m[4].addItem(Manager("new item 2"));
	m[4].addItem(Manager("new item 3"));

	m[4][0].addItem(Manager("new new item 1"));
	m[4][0].addItem(Manager("new new item 2"));
	m[4][0].addItem(Manager("new new item 3"));

	std::cout << "\n========================\n" << std::endl;

	displayItems();
	std::cout << "\n========================\n" << std::endl;

	modifyItem(0);
	modifyItem(5);
	m[0][0][0].setKey(m[0][0][0].getKey() + " uwu");
	displayItems();
	std::cout << "\n========================\n" << std::endl;

	//removeItems();
	displayItems();
}