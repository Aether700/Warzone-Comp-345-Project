#pragma once
#include <vector>
#include "Orders.h"

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

		/* does nothing since the all the decisions are made 
		   by the human user so no need to generate territory lists
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

	class NeutralPlayerStrategy : public PlayerStrategy
	{
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


	class AggressivePlayerStrategy : public PlayerStrategy
	{
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

	private:

	protected:
	
	};


	class BenevolentPlayerStrategy : public PlayerStrategy{


		/*base constructor of BenevolentPlayerStrategy, 
		  takes a ptr to the player who owns this Strategy object

		  player: the player who owns this object
		*/
		BenevolentPlayerStrategy(Player* player);

		//deconstructor of the BenevolentPlayerStrategy class
		~BenevolentPlayerStrategy();

		virtual Order* issueOrder() override;


		/* copy constructor of the BenevolentPlayerStrategy class

		   other: the other BenevolentPlayerStrategy object to copy
		*/
		BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);

		/*assignment operator of the BenevolentPlayerStrategy class 

		  other: the other BenevolentPlayerStrategy object to assign to this one
		  returns: this BenevolentPlayerStrategy object after assignment
		*/
		BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);


		virtual void generateTerritoryLists() override;



		protected:
		/* provides a copy of this BenevolentPlayerStrategy obj (like the clone method in java)
		   will only be used by the Player class to perform a deep copy of a Player object
		*/
		virtual PlayerStrategy* copy() const override;


		private:

		DeployOrder* DeployArmies();

		AdvanceOrder* Advance();



	};



}