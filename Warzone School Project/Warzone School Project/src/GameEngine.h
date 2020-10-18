#pragma once
#include <vector>

namespace WZ
{
	class Player;
	class Deck;

	/* Main singleton class which is responsible for the state of the game. 
	   Contains the game loop, the current players and so on
	*/
	class GameManager
	{
	public:

		/* Given that GameManager is a singleton we delete both the copy constructor 
		  and the assignment operator to avoid having other objects of type GameManager 
		  other than the single instance provided
		*/
		GameManager(const GameManager& other) = delete;
		GameManager& operator=(const GameManager& other) = delete;

		/*returns true if the two players provided are negotiating this turn, false otherwise

		  returns: true if the two players provided are negotiating this turn, false otherwise
		*/
		static bool isNegotiating(const Player* p1, const Player* p2);

		/* Adds a pair of players that are negotiating this turn

		  p1: the first player involved in the negotiation this turn
		  p2: the second player involved in the negotiation this turn
		*/
		static void addNegotiatingPlayers(const Player* p1, const Player* p2);

		/* allows players to draw a card if they did not draw one this turn
		*/
		static void drawCard(Player* p);

		/*getter function which returns the neutral Player

		  returns: a ptr to the neutral player;
		*/
		static Player* getNeutralPlayer();

	private:

		//private constructor of the GameManager class
		GameManager(); 

		//private constructor of the GameManager class
		~GameManager(); 

		/*private getter function which returns the single instance of the GameManager class.
		  This getter is private because every static function of the class uses it to get 
		  access to that instance so there is no need to allow user to access it (better encapsulation)

		  returns: the single instance of the GameManager class 
		*/
		static GameManager& GetManager();

		/*Underlying implementation of the isNegotiating function

		  returns: true if the two players provided are negotiating this turn, false otherwise
		*/
		bool isNegotiatingImpl(const Player* p1, const Player* p2);

		/* Underlying implementation of the addNegotiatingPlayers function

		  p1: the first player involved in the negotiation this turn
		  p2: the second player involved in the negotiation this turn
		*/
		void addNegotiatingPlayersImpl(const Player* p1, const Player* p2);

		/* Underlying implementation of the drawCard function
		*/
		void drawCardImpl(Player* p);

		/*Underlying implementation of the isNegotiating function

		  returns: a ptr to the neutral player;
		*/
		Player* getNeutralPlayerImpl();

		/*private helper function which sets the "hasDrawnCard" 
		  bool of each players to false to allow them to draw again
		*/
		void resetPlayerDrawCard();

		std::vector<Player*> m_activePlayers;
		std::vector<std::pair<const Player*, const Player*>> m_negotiatingPlayers;
		Deck* m_deck;
		Player* m_neutralPlayer;
	};
}