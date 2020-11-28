#pragma once

//include libraries
#include <iostream>
#include <vector>
#include "Cards.h"
#include "PlayerStrategies.h"

namespace WZ
{
	//forward class declaration
	class Territory;
	class OrderList;
	class Order;
	
	// Player class
	class Player {
		friend class GameManager;
		friend class Card;

		//temp friend function for player strategies driver
		friend void StrategyDriver();

	private:
		std::vector<Territory*> territories;
		Hand* hand;
		std::string playerName;
		OrderList* listOrders;
		bool hasDrawnCard;
		PlayerStrategy* m_strategy;

		/* returns a random strategy between an Aggressive or Benevolant strategy
		*/
		PlayerStrategy* GetRandomStrategy();

	public:
		//default constructor of the Player class
		Player();
		// Copy constructor of the Player class 
		Player(const Player&); 
		//default destructor of the Player class
		~Player();
		Player(std::string, PlayerStrategy* strategy = nullptr, std::vector<Territory*> = {});

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
		void SetStrategy(PlayerStrategy* strategy);

		//Methods 
		bool ownsTerritory(Territory* t) const; //method to check if a territory is owned or not.
		void addTerritory(Territory* newTerritory); // method to add a territory
		void removeTerritory(Territory* oldTerritory);// method to remove a territory
		
		/* calls the void generateTerritoryLists function from this player's strategy object
		*/
		void generateTerritoryLists();

		/* returns the territories to defend by this player

		   returns: the territories to defend by this player
		*/
		std::vector<Territory*> toDefend();
		
		/* returns the territories to attack by this player

		   returns: the territories to attack by this player
		*/
		std::vector<Territory*> toAttack();


		/* generates an order to add to the order list, returns null if no more orders should be played this turn

		  returns: an order to add to the order list or null if no more orders should be played this turn
		*/
		Order* issueOrder();
		
		/*	tries to execute the top order and returns that order. 
			Returns null if there are no orders left in the order list of this player
		*/
		Order* executeTopOrder();

		/* returns true if this player has a card of the specified type in their hand
		  
		  type: the type of card we are looking for
		  returns: true if this player has a card of the specified type in their hand, false otherwise
		*/
		bool hasCardType(Card::Type type) const;

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
