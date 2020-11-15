#include "GameEngine.h"

extern void WZ::externalCleanup();

static WZ::Map* CreateMap()
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
    void startupPhaseSetup(std::vector<Player*>& players)
    {
        externalCleanup();

		GameManager& demo = GameManager::GetManager();
		demo.m_activePlayers = players;
		demo.m_deck = new Deck();
		demo.map = CreateMap();

		std::cout << "Starting map:\n\n" << *demo.map;
    }
}

void startupPhaseDriver()
{
	std::cout << "2 player example\n\n";
	std::vector<WZ::Player*> players = { new WZ::Player("P1"), new WZ::Player("P2") };
	WZ::startupPhaseSetup(players);
	WZ::GameManager::startupPhase();
	
	std::cout << "3 player example\n\n";
	players = { new WZ::Player("P1"), new WZ::Player("P2"), new WZ::Player("P3") };
	WZ::startupPhaseSetup(players);
	WZ::GameManager::startupPhase();

	std::cout << "5 player example\n\n";
	players = { new WZ::Player("P1"), new WZ::Player("P2"), new WZ::Player("P3"), new WZ::Player("P4"), new WZ::Player("P5") };
	WZ::startupPhaseSetup(players);
	WZ::GameManager::startupPhase();
}