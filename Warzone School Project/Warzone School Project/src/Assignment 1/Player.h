//include libraries
#include <iostream>
#include <vector>

namespace WZ
{
	//forward class declaration
	class Territory;
	class OrderList;
	class Order;
	class Hand;

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
		std::vector<Player*> getNegotiatingPlayers() const;

		std::vector<Territory*>::iterator begin();
		std::vector<Territory*>::iterator end();
		std::vector<Territory*>::const_iterator begin() const;
		std::vector<Territory*>::const_iterator end() const;

		//Setters
		void setPlayerName(std::string name);
		void setHand();

		//Methods 
		bool ownsTerritory(Territory* t) const;
		void addTerritory(Territory* newTerritory);
		void removeTerritory(Territory* oldTerritory);
		std::vector<Territory*>  toDefend(Player* p, Territory* t);
		std::vector<Territory*> toAttack(Player* p, Territory* t);
		void issueOrder(Order* order);

		Player& operator=(const Player&); //assignment operator 
		bool operator==(const Player&) const;
		bool operator!=(const Player&) const;
	};

	std::ostream& operator<<(std::ostream&, const Player&);
}