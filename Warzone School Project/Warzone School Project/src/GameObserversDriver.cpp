#include "GameObservers.h"
#include "GameEngine.h"
#include <iostream>
using std::cout;
using std::endl;

extern void WZ::externalCleanup();

static WZ::Map* CreateMap(std::vector<WZ::Player*>& players)
{
	std::vector<WZ::Continent*> v;

	//do continent 1
	WZ::Continent* c1 = new WZ::Continent("C1", 5);

	WZ::Territory* t1 = new WZ::Territory("T1", 2, c1);
	WZ::Territory* t2 = new WZ::Territory("T2", 3, c1);
	WZ::Territory* t3 = new WZ::Territory("T3", 4, c1);


	WZ::Continent* c2 = new WZ::Continent("C2", 4);

	WZ::Territory* t4 = new WZ::Territory("T4", 5, c2);
	WZ::Territory* t5 = new WZ::Territory("T5", 6, c2);
	WZ::Territory* t6 = new WZ::Territory("T6", 7, c2);

	players[0]->addTerritory(t1);
	players[0]->addTerritory(t4);
	players[0]->addTerritory(t6);
	players[1]->addTerritory(t2);
	players[1]->addTerritory(t5);
	players[2]->addTerritory(t3);

	t1->setArmies(10);
	t2->setArmies(3);
	t3->setArmies(2);
	t4->setArmies(10);
	t5->setArmies(3);
	t6->setArmies(10);


	t1->addAdjTerritory(t2);
	t1->addAdjTerritory(t5);
	t2->addAdjTerritory(t1);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t1);
	t3->addAdjTerritory(t4);
	t4->addAdjTerritory(t2);
	t5->addAdjTerritory(t1);
	t5->addAdjTerritory(t6);
	t6->addAdjTerritory(t4);
	t4->addAdjTerritory(t6);
	t5->addAdjTerritory(t4);


	c1->addTerritory(t1);
	c1->addTerritory(t2);
	c1->addTerritory(t3);
	c2->addTerritory(t4);
	c2->addTerritory(t5);
	c2->addTerritory(t6);

	v.push_back(c1);
	v.push_back(c2);


	return new WZ::Map(v);
}


namespace WZ
{
	void gameObserversSetup()
	{

		externalCleanup();

		GameManager& demo = GameManager::GetManager();
		demo.m_activePlayers = { new WZ::Player("P1"), new WZ::Player("P2"), new WZ::Player("P3") };
		demo.m_deck = new Deck();
		demo.map = CreateMap(demo.m_activePlayers);
		demo.PhaseObsOn = true;
		demo.StatsObsOn = true;

		std::cout << "Starting map:\n\n" << *demo.map;
	}
}

void gameObserversDriver() {
	WZ::gameObserversSetup();
	WZ::GameManager::callMainGameLoop();
}