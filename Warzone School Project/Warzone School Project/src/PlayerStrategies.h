#pragma once
#include "Orders.h"
#include <vector>

namespace WZ
{
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
		friend class Card;
		
		//temp friend function for player strategies driver
		friend void StrategyDriver();

	public:

		/*base constructor of PlayerStrategy to be used by 
		  sub classes, takes a ptr to the player who owns this Strategy object

		  player: the player who owns this object
		*/
		PlayerStrategy(Player* player);

		/* copy constructor of the PlayerStrategy object to be called by it's subclasses
		   
		   other: the other PlayerStrategy object to copy
		*/
		PlayerStrategy(const PlayerStrategy& other);

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

		/* Sets the owning player of this Strategy object

		  p: the new player who owns this object
		*/
		void SetPlayer(Player* p);

		/*assignment operator of the PlayerStrategy class to be called by it's subclasses

		  other: the other PlayerStrategy object to assign to this one
		  returns: this PlayerStrategy object after assignment
		*/
		PlayerStrategy& operator=(const PlayerStrategy& other);

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

	class HumanPlayerStrategy : public PlayerStrategy
	{
	public:
		/*base constructor of HumanPlayerStrategy, 
		  takes a ptr to the player who owns this Strategy object

		  player: the player who owns this object
		*/
		HumanPlayerStrategy(Player* player);

		/* copy constructor of the HumanPlayerStrategy class

		   other: the other HumanPlayerStrategy object to copy
		*/
		HumanPlayerStrategy(const HumanPlayerStrategy& other);

		//deconstructor of the HumanPlayerStrategy class
		~HumanPlayerStrategy();

		/* returns the next order to issue or null if this player is done issuing orders for this turn

		  returns: the next order to issue or null if this player is done issuing orders for this turn
		*/
		virtual Order* issueOrder() override;

		/* Fills the vector of territories to defend to the territories 
		   owned by the player and the vector of territories to attack to 
		   all territories accessible from the owned territories that are 
		   not owned by the player
		*/
		virtual void generateTerritoryLists() override;

		/*assignment operator of the HumanPlayerStrategy class

		  other: the other HumanPlayerStrategy object to assign to this one
		  returns: this HumanPlayerStrategy object after assignment
		*/
		HumanPlayerStrategy& operator=(const HumanPlayerStrategy& other);

	protected:
		/* provides a copy of this HumanPlayerStrategy obj (like the clone method in java)
		   will only be used by the Player class to perform a deep copy of a Player object
		*/
		virtual PlayerStrategy* copy() const override;

	private:

		/* provides a menu to the user so they can select a card to play.

		  returns: the order created by the card or nullptr if the user selects the back option
		*/
		Order* PlayCard();

		/* prompts the user to select one of their own territories allowing the user 
		  to select a back option only if the boolean provided is true

		  backMessage: defaults to true, determines if the user should be able to select a back option in the menu
		  returns: the territory choosen or nullptr if the user picks the back option (and it's available)
		*/
		Territory* SelectOwnedTerritory(bool backMessage = true);
		
		/* prompts the user to enter the information needed to create a deploy order and returns it.

		  returns: the deploy order created from the date provided by the user.
		*/
		DeployOrder* DeployArmies();

		/* prompts the order to create an advance order and returns it or returns null if the user picked back
		
		  returns: the advance order created from the data provided by the user or null if the user picked back
		*/
		AdvanceOrder* Advance();
		
		/* prompts the user to play a Blockade card

		   c: the card to play
		   returns: the order made when playing the card or null if the user selects the back option
		*/
		BlockadeOrder* PlayBlockadeCard(Card* c);

		/* prompts the user to play a Bomb card
		   
		   c: the card to play
		   returns: the order made when playing the card or null if the user selects the back option
		*/
		BombOrder* PlayBombCard(Card* c);
		
		/* prompts the user to play an Airlift card

		   c: the card to play
		   returns: the order made when playing the card or null if the user selects the back option
		*/
		AirliftOrder* PlayAirliftCard(Card* c);

		/* prompts the user to play a Diplomacy card

		   c: the card to play
		   returns: the order made when playing the card or null if the user selects the back option
		*/
		NegotiateOrder* PlayDiplomacyCard(Card* c);
	};

	//stream insertion operator for the HumanPlayerStrategy class
	std::ostream& operator<<(std::ostream& stream, const HumanPlayerStrategy&);

	class NeutralPlayerStrategy : public PlayerStrategy
	{
	public:
		/*base constructor of NeutralPlayerStrategy, 
		  takes a ptr to the player who owns this Strategy object

		  player: the player who owns this object
		*/
		NeutralPlayerStrategy(Player* player);

		/* copy constructor of the NeutralPlayerStrategy class

		   other: the other NeutralPlayerStrategy object to copy
		*/
		NeutralPlayerStrategy(const NeutralPlayerStrategy& other);

		//deconstructor of the NeutralPlayerStrategy class
		~NeutralPlayerStrategy();

		/*returns nullptr since the neutral player never does anything
		*/
		virtual Order* issueOrder() override;

		/* does nothing since the neutral player does not do anything 
		   so no need to generate any territory lists
		*/
		virtual void generateTerritoryLists() override;

		/*assignment operator of the NeutralPlayerStrategy class 

		  other: the other NeutralPlayerStrategy object to assign to this one
		  returns: this NeutralPlayerStrategy object after assignment
		*/
		NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& other);

	protected:
		/* provides a copy of this NeutralPlayerStrategy obj (like the clone method in java)
		   will only be used by the Player class to perform a deep copy of a Player object
		*/
		virtual PlayerStrategy* copy() const override;
	};

	//stream insertion operator for the NeutralPlayerStrategy class
	std::ostream& operator<<(std::ostream& stream, const NeutralPlayerStrategy&);

	class AggressivePlayerStrategy : public PlayerStrategy
	{
	public: 
		/*base constructor of AggressivePlayerStrategy,
		  takes a ptr to the player who owns this Strategy object

		  player: the player who owns this object
		*/
		AggressivePlayerStrategy(Player* player);

		/* copy constructor of the AggressivePlayerStrategy class

		   other: the other AggressivePlayerStrategy object to copy
		*/
		AggressivePlayerStrategy(const AggressivePlayerStrategy& other);

		//deconstructor of the AggressivePlayerStrategy class
		~AggressivePlayerStrategy();

		/*assignment operator of the NeutralPlayerStrategy class

		  other: the other NeutralPlayerStrategy object to assign to this one
		  returns: this NeutralPlayerStrategy object after assignment
		*/
		AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);

		/* returns the order issued by the player or nullptr if the player is done issuing orders for this turn 

		   returns: the order issued by the player or nullptr if the player is done issuing orders for this turn
		*/
		virtual Order* issueOrder() override;

		/* generates the list of territories to attack and to defend for this turn.
		*/
		virtual void generateTerritoryLists() override;

	protected:
		/* provides a copy of this NeutralPlayerStrategy obj (like the clone method in java)
		   will only be used by the Player class to perform a deep copy of a Player object
		*/
		virtual PlayerStrategy* copy() const override;

	private:
		/*sorts the provided vector of territory such that the territory with 
		  the most armies is at the start of the vector (index 0) and the one 
		  with the least armies is at the end of the vector (index size - 1)

		  list: the list of territories to sort
		  start: the starting index of the currently processed sublist
		  end: one past the last index of the currently processed sublist. the last valid index is end - 1, not end 
		*/
		void quickSortTerritories(std::vector<Territory*>& list, int start, int end);

		/* helper function which simply swaps the territories at the indices provided inside the provided vector

		   list: the vector containing the territories to swap
		   i: the index of the first territory to swap
		   j: the index of the second territory to swap
		*/
		void swapTerritories(std::vector<Territory*>& list, int i, int j);

		/* returns a player that should be negotiated with given the local territories near the provided territory

		   t: the territory that is used to evaluate who to negotiate with
		   returns: the player to negotiate with or null if none was found
		*/
		Player* GetNegotiatingTarget(Territory* t) const;

		/*returns a viable territory to get armies from, if the provided target territory is not null, 
		  the territory returned must also have access to the territory provided in one move

		  target: the territory to send the armies to
		  returns: a viable territory to get armies from, if the provided target territory is not null, 
		  the territory returned must also have access to the territory provided in one move
		*/
		Territory* GetSourceTerritory(Territory* target = nullptr) const;

		/* issues an offensive order
		*/
		Order* offensivePlay();

		/* issues a defensive order
		*/
		Order* defensivePlay();
	};

	//stream insertion operator for the AggressivePlayerStrategy class
	std::ostream& operator<<(std::ostream& stream, const AggressivePlayerStrategy&);

	class BenevolentPlayerStrategy : public PlayerStrategy {

	public:
		/*base constructor of BenevolentPlayerStrategy,
		  takes a ptr to the player who owns this Strategy object

		  player: the player who owns this object
		*/
		BenevolentPlayerStrategy(Player* player);

		/* copy constructor of the BenevolentPlayerStrategy class

		   other: the other BenevolentPlayerStrategy object to copy
		*/
		BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);

		//deconstructor of the BenevolentPlayerStrategy class
		~BenevolentPlayerStrategy();

		/* returns the order issued by the player or nullptr if the player is done issuing orders for this turn

		   returns: the order issued by the player or nullptr if the player is done issuing orders for this turn
		*/
		virtual Order* issueOrder() override;

		/*assignment operator of the BenevolentPlayerStrategy class

		  other: the other BenevolentPlayerStrategy object to assign to this one
		  returns: this BenevolentPlayerStrategy object after assignment
		*/
		BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);

		/* generates the list of territories to attack and to defend for this turn.
		*/
		virtual void generateTerritoryLists() override;


	protected:
		/* provides a copy of this BenevolentPlayerStrategy obj (like the clone method in java)
		   will only be used by the Player class to perform a deep copy of a Player object
		*/
		virtual PlayerStrategy* copy() const override;


	private:

		/* helper function which returns an order or null following 
		   the a defensive play from the player

		   returns: the order issued or nullptr if the player is done playing for this turn
		*/
		Order* defensivePlay();

		/* helper function which issues a deploy order 
		   
		   returns: the deploy order issued by the player
		*/
		DeployOrder* issueDeployOrder();

		/* helper function which issues an advance order

		   returns: the advance order issued by the player
		*/
		AdvanceOrder* Advance();

		/* helper function responsible to find a source territory owned by the player
		   that is to send armies to the territory provided

		   returns: a territory owned by the player capable of sending armies to the 
		   provided territory in one move or null if none was found
		*/
		Territory* getSourceTerritory(Territory* target);

		/*sorts the provided vector of territory such that the territory with
		  the least armies is at the start of the vector (index 0) and the one
		  with the most armies is at the end of the vector (index size - 1)

		  list: the list of territories to sort
		  start: the starting index of the currently processed sublist
		  end: one past the last index of the currently processed sublist. the last valid index is end - 1, not end
		*/
		void quickSortTerritories(std::vector<Territory*>& list, int start, int end);
		
		/* helper function which simply swaps the territories at the indices provided inside the provided vector

		   list: the vector containing the territories to swap
		   i: the index of the first territory to swap
		   j: the index of the second territory to swap
		*/
		void swapTerritories(std::vector<Territory*>& list, int i, int j);

	};

	//stream insertion operator for the BenevolentPlayerStrategy class
	std::ostream& operator<<(std::ostream& stream, const BenevolentPlayerStrategy& strategy);

}