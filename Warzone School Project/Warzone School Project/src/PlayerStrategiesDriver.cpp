#include "PlayerStrategies.h"
#include "GameEngine.h"
#include "Utils.h"
#include "Map.h"
#include "Player.h"

#include <iostream>

static WZ::Map* CreateMap(WZ::Player* p, WZ::Player* p2)
{
	std::vector<WZ::Continent*> v;

	//do continent 1
	WZ::Continent* c1 = new WZ::Continent("C1", 5);
	//****
	WZ::Territory* t1 = new WZ::Territory("T1", 2, c1);
	WZ::Territory* t2 = new WZ::Territory("T2", 3, c1);
	WZ::Territory* t3 = new WZ::Territory("T3", 4, c1);


	WZ::Continent* c2 = new WZ::Continent("C2", 4);
	//*****
	WZ::Territory* t4 = new WZ::Territory("T4", 5, c2);
	WZ::Territory* t5 = new WZ::Territory("T5", 6, c2);
	WZ::Territory* t6 = new WZ::Territory("T6", 7, c2);

	p2->addTerritory(t1);
	p->addTerritory(t2);
	p->addTerritory(t3);
	p->addTerritory(t4);
	p2->addTerritory(t5);
	p2->addTerritory(t6);

	t1->addAdjTerritory(t2);
	t2->addAdjTerritory(t1);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t1);
	t3->addAdjTerritory(t2);
	t3->addAdjTerritory(t4);
	t3->addAdjTerritory(t5);
	t4->addAdjTerritory(t2);
	t4->addAdjTerritory(t3);
	t4->addAdjTerritory(t5);
	t5->addAdjTerritory(t4);
	t5->addAdjTerritory(t6);
	t6->addAdjTerritory(t4);

	t1->setArmies(5);
	t2->setArmies(3);
	t3->setArmies(10);
	t4->setArmies(8);
	t5->setArmies(9);
	t6->setArmies(11);
	
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

static WZ::PlayerStrategy* ChooseStrategy(WZ::Player* p)
{
	constexpr std::array<const char*, 4> choices = { "Human", "Neutral", "Aggressive", "Benevolant" };

	std::cout << "Please choose a Strategy to use for the player\n";
	switch(WZ::AskInput(choices, "Exit Driver"))
	{
	case 1:
		return new WZ::HumanPlayerStrategy(p);

	case 2:
		return new WZ::NeutralPlayerStrategy(p);

	case 3:
		return new WZ::AggressivePlayerStrategy(p);

	case 4:
		return new WZ::BenevolentPlayerStrategy(p);

	case -1:
		return nullptr;
	}
}

static void GiveCards(WZ::Player* p) 
{
	p->getHand()->addCardToHand(new WZ::Card(WZ::Card::Type::Airlift));
	p->getHand()->addCardToHand(new WZ::Card(WZ::Card::Type::Blockade));
	p->getHand()->addCardToHand(new WZ::Card(WZ::Card::Type::Bomb));
	p->getHand()->addCardToHand(new WZ::Card(WZ::Card::Type::Diplomacy));
	p->getHand()->addCardToHand(new WZ::Card(WZ::Card::Type::Reinforcement));
}

namespace WZ
{
	void StrategyDriver()
	{
		WZ::Player* p = new  WZ::Player("P1");
		WZ::Player* p2 = new WZ::Player("P2");
		GiveCards(p);
		WZ::PlayerStrategy* strategy;
		WZ::Map* map = CreateMap(p, p2);

		//set map to the game manager since the strategies get the map through the manager
		GameManager::GetManager().map = map;

		//delete old players and set the current ones as the active players 
		//(the strategies get players through the manager)
		for (Player* p : GameManager::GetManager().m_activePlayers)
		{
			delete p;
		}
		GameManager::GetManager().m_activePlayers = { p, p2 };


		strategy = ChooseStrategy(p);
		if (strategy != nullptr)
		{
			p->SetStrategy(strategy);
			p->generateTerritoryLists();
			map->resetAvailableArmies();
		}

		while (strategy != nullptr)
		{

			p->SetStrategy(strategy); //deletes old strategy
			std::cout << *map << "\n\n";

			//give player reinforcements to show that can only do deploy orders when has some
			std::cout << "Give player reinforcements?\n";
			if (WZ::AskYN())
			{
				p->m_strategy->m_reinforcements = 10;
			}
			std::cout << "\n";

			WZ::Order* o = p->issueOrder();
			if (o == nullptr)
			{
				std::cout << "The player is done for this turn (will not issue orders for the rest of the turn\n";
				strategy->generateTerritoryLists();
				map->resetAvailableArmies();
			}
			else
			{
				std::cout << "The player issued the following order:\n" << *o << "\n";
				delete o;
				GiveCards(p);
			}

			//reset stategy reinforcements to 0 to allow none deploy orders to be made
			p->m_strategy->m_reinforcements = 0;
			std::cout << "\nKeep the same strategy for next order?\n";
			if (!WZ::AskYN())
			{
				strategy = ChooseStrategy(p);
				if (strategy != nullptr)
				{
					map->resetAvailableArmies();
					strategy->generateTerritoryLists();
				}
			}

			std::cout << "\n";
		}
	}
}