#include "Orders.h"
#include "Player.h"
#include "Map.h"

#include <vector>


static WZ::Map* CreateMap(WZ::Player* players)
{
	std::vector<WZ::Continent*> v;

	//do continent 1
	WZ::Continent* c1 = new WZ::Continent("C1", 5);

	WZ::Territory* t1 = new WZ::Territory("T1", 2, c1);
	WZ::Territory* t2 = new WZ::Territory("T2", 3, c1);
	WZ::Territory* t3 = new WZ::Territory("T3", 4, c1);
	WZ::Territory* t4 = new WZ::Territory("T4", 5, c1);
	WZ::Territory* t5 = new WZ::Territory("T5", 6, c1);

	players[0].addTerritory(t1);
	players[1].addTerritory(t2);
	players[2].addTerritory(t3);
	players[3].addTerritory(t4);
	players[3].addTerritory(t5);
	

	t1->setArmies(20);
	t5->setArmies(20);

	t1->addAdjTerritory(t2);
	t1->addAdjTerritory(t5);
	t2->addAdjTerritory(t1);
	t2->addAdjTerritory(t3);
	t3->addAdjTerritory(t1);
	t3->addAdjTerritory(t4);
	t4->addAdjTerritory(t2);
	t5->addAdjTerritory(t1);

	c1->addTerritory(t1);
	c1->addTerritory(t2);
	c1->addTerritory(t3);
	c1->addTerritory(t4);

	v.push_back(c1);

	return new WZ::Map(v);
}

void OrdersDriver()
{
	//create players and set up map
	WZ::Player players[] = { WZ::Player("P1"), WZ::Player("P2"), WZ::Player("P3"), WZ::Player("P4"), };
	WZ::Map* m = CreateMap(players);

	//creating orders
	WZ::DeployOrder deploy = WZ::DeployOrder(&players[3], players[3].getTerritory("T4"), 5);
	WZ::AdvanceOrder advance = WZ::AdvanceOrder(&players[0], players[0].getTerritory("T1"), players[1].getTerritory("T2"), 5);
	WZ::AirliftOrder airlift = WZ::AirliftOrder(&players[0], players[0].getTerritory("T1"), players[2].getTerritory("T3"), 5);
	WZ::BombOrder bomb = WZ::BombOrder(&players[2], players[0].getTerritory("T1"));
	WZ::NegotiateOrder negotiate = WZ::NegotiateOrder(&players[0], &players[3]);
	WZ::AdvanceOrder advance2 = WZ::AdvanceOrder(&players[3], players[3].getTerritory("T5"), players[0].getTerritory("T1"), 5);

	//Test orders

	std::cout << "Initial state of the Map:\n";
	std::cout << *m << "\n";

	//deploy
	std::cout << "\n" << players[3].getPlayerName() << " deploys 5 armies on T4\n\n";
	deploy.execute();
	std::cout << *m << "\n";

	//advance
	std::cout << "\n" << players[0].getPlayerName() << " advances 5 armies from T1 to T2\n\n";
	advance.execute();
	std::cout << *m << "\n";

	//airlift
	std::cout << "\n" << players[0].getPlayerName() << " airlifts 5 armies from T1 to T3\n\n";
	airlift.execute();
	std::cout << *m << "\n";

	//bomb
	std::cout << "\n" << players[2].getPlayerName() << " bombs T1\n\n";
	bomb.execute();
	std::cout << *m << "\n";

	//negotiate
	std::cout << "\n" << players[0].getPlayerName() << " negotiates with " << players[3].getPlayerName() << "\n\n";
	negotiate.execute();
	std::cout << "\nnow " << players[3].getPlayerName() << " advances 5 armies from T5 to T1 which is owned by " 
		<< players[0].getPlayerName() << "\n\n";
	advance2.execute();
	std::cout << *m << "\n";


	delete m;
}	
