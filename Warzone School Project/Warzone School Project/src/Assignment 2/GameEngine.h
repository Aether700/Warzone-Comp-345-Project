#pragma once
#include <vector>

namespace WZ
{
	class Player;
	class Deck;

	/* Main static class which is responsible for the state of the game. 
	   Contains the game loop, the current players and so on
	*/
	class GameManager
	{
	public:
		//delete constructor since this class is 
		//fully static (we don't want GameManager objects)
		GameManager() = delete;

		/* Initializes the basic variables of the GameManager as well as 
		   some other small tasks such as setting the seed of the Random class.
		*/
		static void init();

		/*returns true if the two players provided are negotiating this turn, false otherwise

		  returns: true if the two players provided are negotiating this turn, false otherwise
		*/
		static bool isNegotiating(const Player* p1, const Player* p2);

		/* Adds a pair of players that are negotiating this turn

		  p1: the first player involved in the negotiation this turn
		  p2: the second player involved in the negotiation this turn
		*/
		static void addNegotiatingPlayers(const Player* p1, const Player* p2);


	private:
		static std::vector<std::pair<const Player*, const Player*>> s_negotiatingPlayers;
		static Deck* s_deck;
	};
}