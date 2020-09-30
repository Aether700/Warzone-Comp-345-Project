//include libraries
#include <iostream>
#include <string>
#include <vector>

// include parts 
#include "Map.h"
#include "Cards.h"
#include "Territory.h"
#include "Orders.h"

namespace WZ
{
	class Territory;
	class Order;

	class Player {
	private:
		vector<Territory*> territorries;
		Hand* hand;
	public:
		Player() {}
		~Player();
		Player(string, vector<Territory*>);

		//Getters
		vector<Territory*> getTerritorries();
		Territory* getTerritory(string n);
		Hand* getHand();

		//Setters
		void setTerritorries();
		void setHand();

		//Methods 
		void addTerritory(Territory* newTerritory);
		void removeTerritory(Territory* oldTerritory);
		void toDefend(Player* p, Territory* t);
		void toAttack()(Player* p, Territory* t);
		void issueOrder();
	};
}