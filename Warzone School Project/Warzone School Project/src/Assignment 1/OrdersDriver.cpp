#include "Orders.h"
#include "Player.h"
#include "Map.h"


void OrdersDriver()
{
	//set up variables
	WZ::Player* p1 = new WZ::Player("Player 1", {});
	WZ::Player* p2 = new WZ::Player("Player 2", {});
	WZ::Player* p3 = new WZ::Player("Player 3", {});

	WZ::Territory* t1 = new WZ::Territory("T1", 1, nullptr, 5);
	WZ::Territory* t2 = new WZ::Territory("T2", 2, nullptr, 10);
	WZ::Territory* t3 = new WZ::Territory("T3", 3, nullptr, 15);
	
	t1->addAdjTerritory(t2);
	t1->addAdjTerritory(t3);
	t2->addAdjTerritory(t1);
	t3->addAdjTerritory(t1);

	

	p1->addTerritory(t1);
	t1->setOwner(p1);

	p2->addTerritory(t2);
	t2->setOwner(p2);

	p3->addTerritory(t3);
	t3->setOwner(p3);

	WZ::DeployOrder deployVal = WZ::DeployOrder(p1, t1, 5);

	//create Orders
	WZ::DeployOrder* deploy = new WZ::DeployOrder(p1, t1, 5);
	WZ::AdvanceOrder* advance = new WZ::AdvanceOrder(p2, t2, t1, 5);
	WZ::BombOrder* bomb = new WZ::BombOrder(p1, t2);
	WZ::BlockadeOrder* blockade = new WZ::BlockadeOrder(p1, t1);
	WZ::AirliftOrder* airlift = new WZ::AirliftOrder(p2, t2, t1, 2);
	WZ::NegotiateOrder* negotiate = new WZ::NegotiateOrder(p1, p3);
	
	
	//Set OrderList

	WZ::OrderList* list = new WZ::OrderList();

	list->addOrder(deploy);
	list->addOrder(advance);
	list->addOrder(bomb);
	list->addOrder(blockade);
	list->addOrder(airlift);
	list->addOrder(negotiate);

	//////////////////////////////
	//Test OrderList//////////////
	//////////////////////////////

	//operator<< test + deleteOrder test
	std::cout << "Initial OrderList: " << *list << "\n\n";
	
	{
		WZ::Order* toRemove = (*list)[3];

		std::cout << "deleting order: " << *toRemove << "\n\n";
		list->deleteOrder(3);

		std::cout << "list after delete operation: " << *list << "\n\n";
		
		list->addOrder(toRemove);
	}

	//move operation test
	std::cout << "orders at index 0 & 3 before move operation:"
		"\nindex 0: " << *(*list)[0] <<
		"\nindex 3: "<< *(*list)[3] << "\n\n";

	std::cout << "Moving order at index 0 to index 3\n\n";
	list->move(0, 3);

	std::cout << "orders at index 0 & 3 after move operation:"
		"\nindex 3: " << *(*list)[3] << "\n\n";

	////////////////////////////////////
	//Test Order functions//////////////
	////////////////////////////////////

	//operator<< test (before executed)
	std::cout << "printing Orders (before execution):\n";

	for (WZ::Order* o : *list)
	{
		std::cout << *o << "\n";
	}


	//validate test (should be true)
	std::cout << "\nChecking validity (all orders should be valid):\n";
	for (WZ::Order* o : *list)
	{
		std::cout << *o << " is valid: " << o->validate() << "\n";
	}

	//execute test
	std::cout << "\nexecuting orders:\n";
	for (WZ::Order* o : *list)
	{
		o->execute();
	}

	//operator<< test (after executed)
	std::cout << "\nprinting executed orders:\n";
	for (WZ::Order* o : *list)
	{
		std::cout << *o << "\n";
	}
	

	//validity test (should be false)

	//initialize invalid orders
	WZ::DeployOrder* invalidDeploy = new WZ::DeployOrder(p1, t2, 5);
	WZ::AdvanceOrder* invalidAdvance = new WZ::AdvanceOrder(p2, t1, t3, 5);
	WZ::BombOrder* invalidBomb = new WZ::BombOrder(p2, t2);
	WZ::BlockadeOrder* invalidBlockade = new WZ::BlockadeOrder(p1, t2);
	WZ::AirliftOrder* invalidAirlift = new WZ::AirliftOrder(p2, t2, t1, 200);
	WZ::NegotiateOrder* invalidNegotiate = new WZ::NegotiateOrder(p1, p1);

	std::cout << "\nTesting validate function with invalid orders (should all be false):\n";

	std::cout << "Order being tested: " << *invalidDeploy << "\n";
	std::cout << "is valid: " << invalidDeploy->validate() << "\n\n";

	std::cout << "Order being tested: " << *invalidAdvance << "\n";
	std::cout << "is valid: " << invalidAdvance->validate() << "\n\n";

	std::cout << "Order being tested: " << *invalidBomb << "\n";
	std::cout << "is valid: " << invalidBomb->validate() << "\n\n";

	std::cout << "Order being tested: " << *invalidBlockade << "\n";
	std::cout << "is valid: " << invalidBlockade->validate() << "\n\n";

	std::cout << "Order being tested: " << *invalidAirlift << "\n";
	std::cout << "is valid: " << invalidAirlift->validate() << "\n\n";

	std::cout << "Order being tested: " << *invalidNegotiate << "\n";
	std::cout << "is valid: " << invalidNegotiate->validate() << "\n\n";

	//clean up
	delete p1;
	delete p2;
	delete p3;

	delete t1;
	delete t2;
	delete t3;

	delete deploy;
	delete advance;
	delete bomb;
	delete blockade;
	delete airlift;
	delete negotiate;

	delete invalidDeploy;
	delete invalidAdvance;
	delete invalidBomb;
	delete invalidBlockade;
	delete invalidAirlift;
	delete invalidNegotiate;

	delete list;
}	
