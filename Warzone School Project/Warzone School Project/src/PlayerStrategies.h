#pragma once
#include <vector>

namespace WZ
{
	class Order;
	class Territory;
	class Player;

	/* Base class for all Strategy object. 
	  Serves as an interface to the Player object to 
	  perform decisions (note that the implementation 
	  is not an actual c++ interface)
	*/
	class PlayerStrategy
	{
		friend class GameManager;
		friend class Player;
	public:

		PlayerStrategy(Player* player);

		//default virtual deconstructor to avoid memory leaks
		virtual ~PlayerStrategy();

		/* returns the next order to issue or null if this player is done issuing orders for this turn

		  returns: the next order to issue or null if this player is done issuing orders for this turn
		*/
		virtual Order* issueOrder() = 0;
		
		/* getter function which returns the vector of territory to defend this turn

		  return: the vector of territory to defend this turn
		*/
		std::vector<Territory*>& toDefend();
		
		/* getter function which returns the vector of territory to attack this turn

		  return: the vector of territory to attack this turn
		*/
		std::vector<Territory*>& toAttack();

		/* function called by the GameManager at the start of every issuing order phase to 
		  update which territories should be attacked or defended by this player. 
		  Calling this function should allow the player to fill the list of territories 
		  to defend and to attack based on the new situation of the map
		*/
		virtual void generateTerritoryLists() = 0;

		void SetPlayer(Player* p);

	protected: 
		/* provides a copy of this PlayerStrategy obj (like the clone method in java)
		   will only be used by the Player class to perform a deep copy of a Player object
		*/
		virtual PlayerStrategy* copy() const = 0;

		/* fields to be managed by all child class of the PlayerStrategy class
		   These vectors represent the list of territories to defend and to attack respectively.

		   The unsigned int represents the reinforcements available to the Player this turn 
		   and the player is the player who owns this Strategy object
		*/
		std::vector<Territory*> m_toDef;
		std::vector<Territory*> m_toAtk;
		unsigned int m_reinforcements;
		Player* m_player;
	};
}