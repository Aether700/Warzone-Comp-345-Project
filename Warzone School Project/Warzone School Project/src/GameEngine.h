#pragma once
#include <vector>
#include "Map.h"
#include "MapLoader.h"

namespace WZ
{
	class Player;
	class Deck;
	class Territory;

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

		//destructor of the GameManager class
		~GameManager(); 
		
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

		/*Method startupPhase is a method that will randomise the order of play of the player.
		where the number of players can be between 2 and 5 inclusively. The territories in the
		map will be randomly assigned to the players in a round-robin fashion.
		And finally, depending on the number of players, each player will start with a number of
		initial armies.  This method will return the player, the number of armies and its territories.*/;
		static void startupPhase(const Player* p, const Territory* t, int armies);
	
	private:

		//private constructor of the GameManager class
		GameManager(); 

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
	
	class GameEngine 
	{

	public:
	
		//default constructor
		GameEngine();								
		//destructor				
		~GameEngine();
		//Constructor that takes the map name and number of players
		GameEngine(string,int);									
		//accessor to get the selected map name
		string getUserMap();					
		//accessor to get the selected number of players
		int getUserNumPlayers();			
		//mutator to set prefered map name
		void setUserMap(string);				
		//mutator to set prefered number of players
		void setUserNumPlayers(int);		
		// function to initialize the game with the user's preferences
		void gameStart();
		//assignment operator	
		GameEngine& operator = (const GameEngine&);							
		int armies;

	private:

		//user selected map
		string userMap;						
		//user selected # of players
		int userNumPlayers;				
		//initialized map
		Map* map;									
		MapLoader* map_loader;						
		//initialized players list
		Player* listOfPlayers;			
		// initialized game deck
		Deck* deck;		

		/*Method startupPhase is a method that will randomise the order of play of the player.
		where the number of players can be between 2 and 5 inclusively. The territories in the
		map will be randomly assigned to the players in a round-robin fashion.
		And finally, depending on the number of players, each player will start with a number of
		initial armies.  This method will return the player, the number of armies and its territories.*/;
		void startupPhaseImpl(const Player* p, const Territory* t, int armies); 
					
};

}