#include "GameEngine.h"
#include "Player.h"

static WZ::Map* CreateMap(std::vector<WZ::Player*>& players)
{
	std::vector<WZ::Continent*> v;

	//do continent 1
	WZ::Continent* c1 = new WZ::Continent("C1", 5);
	//****
	WZ::Territory* t1 = new WZ::Territory("T1", 2, c1);
	WZ::Territory* t2 = new WZ::Territory("T2", 3, c1);


	WZ::Continent* c2 = new WZ::Continent("C2", 4);
	//*****
	WZ::Territory* t3 = new WZ::Territory("T3", 4, c2);
	WZ::Territory* t4 = new WZ::Territory("T4", 5, c2);

	players[0]->addTerritory(t1);
	players[1]->addTerritory(t2);
	players[2]->addTerritory(t3);
	players[0]->addTerritory(t4);

	t1->addAdjTerritory(t2);
	t2->addAdjTerritory(t1);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t1);
	t3->addAdjTerritory(t4);
	t4->addAdjTerritory(t3);


	c1->addTerritory(t1);
	c1->addTerritory(t2);
	c2->addTerritory(t3);
	c2->addTerritory(t4);
	
	v.push_back(c1);
	v.push_back(c2);


	return new WZ::Map(v);
}

namespace WZ {
	void externalCleanup(){
		GameManager& demo = GameManager::GetManager();
		for (Player* p : demo.m_activePlayers) {
			delete p;
		}
		demo.m_activePlayers.clear();
		delete demo.m_deck;
		delete demo.map;
	}

	void mainGameLoopSetup() {
		externalCleanup();
		GameManager& demo = GameManager::GetManager();
		demo.m_activePlayers = { new Player("player1"), new Player("player2"), new Player("player3") };
		demo.m_deck = new Deck();
		demo.PhaseObsOn = true;
		demo.StatsObsOn = true;
		demo.map = CreateMap(demo.m_activePlayers);
	}
}

void mainGameLoopDriver() {
	WZ::mainGameLoopSetup();
	WZ::GameManager::callMainGameLoop();
}