//include libraries
#include <iostream>

// include parts 
#include "Cards.h"
#include "Orders.h"

namespace WZ
{
	class Territory;

	class Player {
	private:
		std::vector<Territory*> territories;
		Hand* hand;
		std::string playerName;
		OrderList* listOrders;
	public:
		Player();
		Player(const Player&); //copy constructor
		~Player();
		Player(std::string, std::vector<Territory*>);

		//Getters
		std::vector<Territory*> getTerritories() const;
		Territory* getTerritory(std::string n);
		Hand* getHand();
		std::string getPlayerName() const;
		std::vector<Territory*>::iterator begin();
		std::vector<Territory*>::iterator end();

		//Setters
		void setPlayerName(std::string name);
		void setHand();
		void setTerritories();

		//Methods 
		void addTerritory(Territory* newTerritory);
		void removeTerritory(Territory* oldTerritory);
		std::vector<Territory*>  toDefend(Player* p, Territory* t);
		std::vector<Territory*> toAttack(Player* p, Territory* t);
		void issueOrder(Order* order);

		Player& operator=(const Player&); //assignment operator 
	};

	std::ostream& operator<<(std::ostream&, const Player&);
}