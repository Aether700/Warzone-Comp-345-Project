#pragma once

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
	
	// Player class
	class Player {
		friend class GameManager;
	private:
		std::vector<Territory*> territories;
		Hand* hand;
		std::string playerName;
		OrderList* listOrders;
		bool hasDrawnCard;
	public:
		//default constructor of the Player class
		Player();
		// Copy constructor of the Player class 
		Player(const Player&); 
		//default destructor of the Player class
		~Player();
		Player(std::string, std::vector<Territory*> = {});

		//Getters
		std::vector<Territory*> getTerritories() const;
		Territory* getTerritory(std::string n);
		size_t getNumOfTerritories() const;
		Hand* getHand();
		std::string getPlayerName() const;
		const OrderList* getOrderList() const;
		
		/* default begin function to allow use of ranged for loops
		returns: an iterator pointing to the beginning of the list
		*/
		std::vector<Territory*>::iterator begin();
		/* default end function to allow use of ranged for loops
			returns: an iterator pointing to the end of the list
			*/
		std::vector<Territory*>::iterator end();
		/* default begin function to allow use of ranged for loops in a const context
			returns: an const_iterator pointing to the beginning of the list
			*/
		std::vector<Territory*>::const_iterator begin() const;
		/* default end function to allow use of ranged for loops in a const context
			returns: an const_iterator pointing to the end of the list */
		std::vector<Territory*>::const_iterator end() const;
		

		//Setters
		void setPlayerName(std::string name);
		void setHand();

		//Methods 
		bool ownsTerritory(Territory* t) const; //method to check if a territory is owned or not.
		void addTerritory(Territory* newTerritory); // method to add a territory
		void removeTerritory(Territory* oldTerritory);// method to remove a territory
		std::vector<Territory*>  toDefend(); //method toDefend that returns a list of territories that are defended.
		std::vector<Territory*> toAttack();//method toAttack that returns a list of territories that are attacked.
		void issueOrder(int&);

		// assignement operator. Assigns the values of the provided Player object and returns the modified object 
		Player& operator=(const Player&);
		/* equals operator. returns true if the two players share the same 
		   name and the same orderlist, false otherwise
		*/
		bool operator==(const Player&) const;
		/*Standard != operator. 
		 Returns true if the == operator was false and vice versa
		 returns: true if the == operator was false and vice versa
		*/
		bool operator!=(const Player&) const;
	};
	//insertion operator for every Player object
	std::ostream& operator<<(std::ostream&, const Player&);
}
