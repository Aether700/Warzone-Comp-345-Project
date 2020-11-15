#pragma once
#include "GameObservers.h"
#include "Map.h"
#include "MapLoader.h"

#include <vector>

namespace WZ
{
	class Player;
	class Deck;
	class Territory;

	/* Main singleton class which is responsible for the state of the game. 
	   Contains the game loop, the current players and so on
	*/
	class GameManager:public Subject <PhaseObserver>, public Subject<StatisticsObserver>
	{
	public:

		//temporary friend functions for the drivers
		friend void mainGameLoopSetup();
		friend void startupPhaseSetup(std::vector<Player*>& players);
		friend void externalCleanup();

		static void callMainGameLoop();		//	temporary function that calls the main game 
												//	>>	loop from outside

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
		static void startupPhase();
	
		//Returns the current game manager phase
		static GamePhase getCurrentPhase();

		static const Player* getCurrentPlayer();

		// function to initialize the game with the user's preferences
		static void gameStart();

		/* Adds a PhaseObserver to "listen" to the GameManager

		  p: phase observer to add
		*/
		static void AddPhaseObserver(PhaseObserver* p);

		/* Removes a PhaseObserver from the "listen" of the GameManager

		  p: phase observer to remove
		*/
		static void RemovePhaseObserver(PhaseObserver* p);
		

		/* Adds a StatisticObserver to "listen" to the GameManager

		  p: statistic observer to add
		*/
		static void AddStatisticsObserver(StatisticsObserver* p);

		/* Removes a StatisticsObserver from the "listen" of the GameManager

		  p: statistic observer to remove
		*/
		static void RemoveStatisticsObserver(StatisticsObserver* p);

		/* returns a const reference to the vector of currently active players in the game
		*/
		static const std::vector <Player*>& getActivePlayers();

		/* returns a const ptr to the map currently used by the game
		*/
		static const Map* getMap();


		static const Order* getLastOrder();

		/* displays a menu to the user allowing them to toggle the observers on and off
		*/
		static void SettingsMenu();

		/* Static function responsible of running an attack

		  source: the territory from which the attacking armies come from
		  target: the territory that is being attacked
		  amount: the number of armies that the source territory sends to attack

		  return: true if the attackers have successfully captured the territory, false otherwise
		*/
		static bool Attack(Territory* source, Territory* target, unsigned int amount);

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

		/* Underlying implementation of the startupPhase functions.

    	*/
		void startupPhaseImpl(); 

		/* Notifies all statistic observers who are registered to the GameManager
		*/
		static void NotifyStatisticsObserver();

		//asks the user to select a map file and creates that map (if the file is valid) and assigns 
		//it to the map attribute of the GameManager
		void getUserMap();

		//Asks the user how many player will play and returns that number
		int getUserNumPlayers();

		/* underlyin implementation of AddPhaseObserver
		*/
		void AddPhaseObserverImpl(PhaseObserver* p);
		
		/*function to get the number of players and initialize them in the m_activePlayers */
		void InitializePlayers();

		/* underlyin implementation of RemovePhaseObserver
		*/
		void RemovePhaseObserverImpl(PhaseObserver* p);

		/* underlyin implementation of NotifyPhaseObserver
		*/
		void NotifyPhaseObserversImpl() const;

		/* Notifies all Phase observers who are registered to the GameManager
		*/
		static void NotifyPhaseObserver();
		
		/* underlyin implementation of AddStatisticsObserver
		*/
		void AddStatisticsObserverImpl(StatisticsObserver* p);

		/* underlyin implementation of RemoveStatisticsObserver
		*/
		void RemoveStatisticsObserverImpl(StatisticsObserver* p);

		/* underlyin implementation of NotifyStatisticsObserver
		*/
		void NotifyStatisticsObserverImpl() const;

		/* underlyin implementation of getActivePlayers
		*/
		const std::vector <Player*>& getActivePlayersImpl() const;

		/* underlyin implementation of getMap
		*/
		const Map* getMapImpl() const;

		/* underlyin implementation of SettingsMenu function
		*/
		void SettingsMenuImpl();


		/*	Function that calculates the number of reinforcements for a given player
		 */
		unsigned int reinforcementCalculator(Player* p);

		/*	Calculate and distribute reinforcements for all active players in game.
			This phase is sole for the begining of the round. */
		void reinforcementPhase();

		/* runs the issue orders phase
		*/
		void issueOrdersPhase();
		
		/*	runs the execution order phase
		*/
		void executeOrderPhase();

		/* main game loop
		*/
		void mainGameLoop();

		const Order* getLastOrderImpl() const;

		std::vector<Player*> m_activePlayers;
		std::vector<std::pair<const Player*, const Player*>> m_negotiatingPlayers;
		std::vector<std::pair<const Player*, const Player*>> m_bufferList;
		Deck* m_deck;
		Player* m_neutralPlayer;
		GamePhase currentphase;
		Player* CurrentPlayer;
		Map* map;
		Order* m_lastOrder;
		bool PhaseObsOn=true;
		bool StatsObsOn=true;
		
	};
}