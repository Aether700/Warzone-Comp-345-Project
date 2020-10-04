//include libraries
#include <iostream>

// include parts 
#include "Cards.h"
#include "Orders.h"

namespace WZ
{
	class Territory;
	class Order;

	class Player {
	private:
		std::vector<Territory*> territorries;
		Hand* hand;
	public:
		Player() {}
		~Player();
		Player(std::string, std::vector<Territory*>);

		//Getters
		std::vector<Territory*> getTerritorries() const;
		Territory* getTerritory(std::string n);
		Hand* getHand();
		Player* getPlayerName() const;

		//Setters
		void setPlayerName();
		void setHand();
		void setTerritorries();

		//Methods 
		void addTerritory(Territory* newTerritory);
		void removeTerritory(Territory* oldTerritory);
		std::vector<Territory*>  toDefend(Player* p, Territory* t);
		std::vector<Territory*> toAttack(Player* p, Territory* t);
		void issueOrder();
	};
}