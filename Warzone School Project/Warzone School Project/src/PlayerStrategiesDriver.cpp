#include "PlayerStrategies.h"
#include "Utils.h"
#include "Map.h"
#include "Player.h"

#include <iostream>

static WZ::Map* CreateMap(WZ::Player* p)
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

	p->addTerritory(t1);
	p->addTerritory(t2);
	p->addTerritory(t3);
	p->addTerritory(t4);

	t1->addAdjTerritory(t2);
	t2->addAdjTerritory(t1);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t1);
	t3->addAdjTerritory(t4);
	t4->addAdjTerritory(t3);
	t4->addAdjTerritory(t5);
	t5->addAdjTerritory(t4);
	t5->addAdjTerritory(t6);
	t6->addAdjTerritory(t4);


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
	constexpr std::array<const char*, 5> choices = { "Human", "Neutral", "Aggressive", "Benevolant" };

	std::cout << "Please choose a Strategy to use for the player";
	switch(WZ::AskInput(choices, "Exit Driver"))
	{
	case 1:
		return new WZ::HumanPlayerStrategy(p);
		break;

	case 2:
		return new WZ::NeutralPlayerStrategy(p);
		break;

	case 3:
		return new WZ::AggressivePlayerStrategy(p);
		break;

	case 4:
		return new WZ::BenevolentPlayerStrategy(p);
		break;

	case -1:
		return nullptr;
		break;
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

void StrategyDriver()
{
	WZ::Player p = WZ::Player("P");
	GiveCards(&p);
	WZ::PlayerStrategy* strategy;
	WZ::Map* map = CreateMap(&p);


	strategy = ChooseStrategy(&p);
	p.SetStrategy(strategy);
	while (strategy != nullptr)
	{
		std::cout << *map << "\n\n";
		WZ::Order* o = p.issueOrder();
		if (o == nullptr)
		{
			std::cout << "The player is done for this turn (will not issue orders for the rest of the turn";
			strategy->generateTerritoryLists();
		}
		std::cout << "The player issued the following order:\n" << *o << "\n";
		delete o;

		std::cout << "Keep the same strategy for next order?";
		if (!WZ::AskYN())
		{
			strategy = ChooseStrategy(&p);
			p.SetStrategy(strategy); //deletes the old strategy
		}
		GiveCards(&p);
	}

	delete map;
}

