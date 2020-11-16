#include "Map.h"
#include "Player.h"
#include <iostream>

WZ::Map* CreateValidMap()
{
	std::vector<WZ::Continent*> v;

	//do continent 1
	WZ::Continent* c1 = new WZ::Continent("C1", 5);
	
	WZ::Territory* t1 = new WZ::Territory("T1", 2, c1);
	WZ::Territory* t2 = new WZ::Territory("T2", 3, c1);
	WZ::Territory* t3 = new WZ::Territory("T3", 4, c1);
	
	t1->addAdjTerritory(t2);
	t1->addAdjTerritory(t3);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t1);

	c1->addTerritory(t1);
	c1->addTerritory(t2);
	c1->addTerritory(t3);

	v.push_back(c1);

	//do continent 2
	WZ::Continent* c2 = new WZ::Continent("C2", 4);

	WZ::Territory* t4 = new WZ::Territory("T4", 5, c2);
	WZ::Territory* t5 = new WZ::Territory("T5", 6, c2);
	WZ::Territory* t6 = new WZ::Territory("T6", 7, c2);

	t4->addAdjTerritory(t5);
	t5->addAdjTerritory(t6);
	t6->addAdjTerritory(t4);

	c2->addTerritory(t4);
	c2->addTerritory(t5);
	c2->addTerritory(t6);

	v.push_back(c2);

	//do continent 3
	WZ::Continent* c3 = new WZ::Continent("C3", 5);

	WZ::Territory* t8 = new WZ::Territory("T8", 8, c3);
	WZ::Territory* t7 = new WZ::Territory("T7", 9, c3);
	WZ::Territory* t9 = new WZ::Territory("T9", 10, c3);

	t7->addAdjTerritory(t9);
	t8->addAdjTerritory(t9);
	t9->addAdjTerritory(t8);
	
	c3->addTerritory(t7);
	c3->addTerritory(t8);
	c3->addTerritory(t9);

	v.push_back(c3);


	//inter connect the different continents

	t1->addAdjTerritory(t8);
	t8->addAdjTerritory(t2);
	t5->addAdjTerritory(t2);
	t2->addAdjTerritory(t5);

	return new WZ::Map(v);
}

WZ::Map* CreateInvalidMap1()
{
	std::vector<WZ::Continent*> v;

	//do continent 1
	WZ::Continent* c1 = new WZ::Continent("C1", 5);

	WZ::Territory* t1 = new WZ::Territory("T1", 2, c1);
	WZ::Territory* t2 = new WZ::Territory("T2", 3, c1);
	WZ::Territory* t3 = new WZ::Territory("T3", 4, c1);

	//t3 is not connected to the rest of the continent so the map is invalid
	t1->addAdjTerritory(t2);
	t2->addAdjTerritory(t1);

	c1->addTerritory(t1);
	c1->addTerritory(t2);
	c1->addTerritory(t3);

	v.push_back(c1);

	//do continent 2
	WZ::Continent* c2 = new WZ::Continent("C2", 4);

	WZ::Territory* t4 = new WZ::Territory("T4", 2, c2);
	WZ::Territory* t5 = new WZ::Territory("T5", 3, c2);
	WZ::Territory* t6 = new WZ::Territory("T6", 4, c2);

	t4->addAdjTerritory(t5);
	t5->addAdjTerritory(t6);
	t6->addAdjTerritory(t4);

	c2->addTerritory(t4);
	c2->addTerritory(t5);
	c2->addTerritory(t6);

	v.push_back(c2);

	//do continent 3
	WZ::Continent* c3 = new WZ::Continent("C3", 5);

	WZ::Territory* t8 = new WZ::Territory("T8", 3, c3);
	WZ::Territory* t7 = new WZ::Territory("T7", 2, c3);
	WZ::Territory* t9 = new WZ::Territory("T9", 4, c3);

	t7->addAdjTerritory(t9);
	t8->addAdjTerritory(t9);
	t9->addAdjTerritory(t8);

	c3->addTerritory(t7);
	c3->addTerritory(t8);
	c3->addTerritory(t9);

	v.push_back(c3);


	//inter connect the different continents

	t1->addAdjTerritory(t8);
	t8->addAdjTerritory(t2);
	t5->addAdjTerritory(t2);
	t2->addAdjTerritory(t5);

	return new WZ::Map(v);
}

WZ::Map* CreateInvalidMap2()
{
	std::vector<WZ::Continent*> v;

	//do continent 1
	WZ::Continent* c1 = new WZ::Continent("C1", 5);

	WZ::Territory* t1 = new WZ::Territory("T1", 2, c1);
	WZ::Territory* t2 = new WZ::Territory("T2", 3, c1);
	WZ::Territory* t3 = new WZ::Territory("T3", 4, c1);

	t1->addAdjTerritory(t2);
	t1->addAdjTerritory(t3);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t1);

	c1->addTerritory(t1);
	c1->addTerritory(t2);
	c1->addTerritory(t3);

	v.push_back(c1);

	//do continent 2
	WZ::Continent* c2 = new WZ::Continent("C2", 4);

	WZ::Territory* t4 = new WZ::Territory("T4", 2, c2);
	WZ::Territory* t5 = new WZ::Territory("T5", 3, c2);
	WZ::Territory* t6 = new WZ::Territory("T6", 4, c2);

	t4->addAdjTerritory(t5);
	t5->addAdjTerritory(t6);
	t6->addAdjTerritory(t4);

	c2->addTerritory(t4);
	c2->addTerritory(t5);
	c2->addTerritory(t6);

	v.push_back(c2);

	//do continent 3
	WZ::Continent* c3 = new WZ::Continent("C3", 5);

	WZ::Territory* t8 = new WZ::Territory("T8", 3, c3);
	WZ::Territory* t7 = new WZ::Territory("T7", 2, c3);
	WZ::Territory* t9 = new WZ::Territory("T9", 4, c3);

	t7->addAdjTerritory(t9);
	t8->addAdjTerritory(t9);
	t9->addAdjTerritory(t8);

	//both c1 and c3 contain t1 so the map is invalid
	c3->addTerritory(t1);
	c3->addTerritory(t7);
	c3->addTerritory(t8);
	c3->addTerritory(t9);

	v.push_back(c3);


	//inter connect the different continents

	t1->addAdjTerritory(t8);
	t8->addAdjTerritory(t2);
	t5->addAdjTerritory(t2);
	t2->addAdjTerritory(t5);

	return new WZ::Map(v);
}

void MapDriver()
{
	WZ::Territory* t = new WZ::Territory("Example Territory", 7, nullptr, 10);
	WZ::Player* p = new WZ::Player("Example Player", nullptr, { t });
	t->setOwner(p);

	std::cout << "display of the Example Territory object:\n";
	std::cout << *t << "\n";

	std::cout << "\ndisplay of army count for the Example Territory:\n";
	std::cout << t->getArmies() << "\n";
	
	std::cout << "\ndisplay of owning Player of the Example Territory:\n";
	std::cout << *t->getOwner() << "\n";

	WZ::Map* validMap = CreateValidMap();

	std::cout << "\nPrinting valid map:\n";
	std::cout << *validMap << "\n";

	std::cout << "\nChecking validity of the valid map:\n";
	std::cout << "is Valid: " << validMap->validate() << "\n";

	WZ::Map* invalidMap1 = CreateInvalidMap1();

	std::cout << "\nChecking validity of the invalid map with continent which is not a connected graph:\n";
	std::cout << "is Valid: " << invalidMap1->validate() << "\n";

	WZ::Map* invalidMap2 = CreateInvalidMap1();

	std::cout << "\nChecking validity of the invalid map where a territory is owned by 2 continents:\n";
	std::cout << "is Valid: " << invalidMap2->validate() << "\n";


	delete t;
	delete p;
	
	delete validMap;
	delete invalidMap1;
	delete invalidMap2;
}