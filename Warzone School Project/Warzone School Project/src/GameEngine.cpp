#include "GameEngine.h"
#include "Player.h"
#include "Cards.h"
#include "Utils.h"

namespace WZ
{
	// initializing static member variables ////////////////////////////////////
	std::vector<std::pair<const Player*, const Player*>> GameManager::s_negotiatingPlayers;
	Deck* GameManager::s_deck = new Deck();
	Player* GameManager::s_neutralPlayer = new Player("Neutral");
	std::vector<Player*> GameManager::s_activePlayers;
	////////////////////////////////////////////////////////////////////////////

	void GameManager::init()
	{
		//init deck here
		Random::Init();
	}

	void GameManager::close()
	{
		delete s_deck;
		delete s_neutralPlayer;
	}


	bool GameManager::isNegotiating(const Player* p1, const Player* p2)
	{
		for (std::pair<const Player*, const Player*> pair : s_negotiatingPlayers)
		{
			if ((pair.first == p1 && pair.second == p2) || (pair.first == p2 && pair.second == p1))
			{
				return true;
			}
		}
		return false;
	}

	void GameManager::addNegotiatingPlayers(const Player* p1, const Player* p2)
	{
		s_negotiatingPlayers.push_back({ p1, p2 });
	}

	void GameManager::drawCard(Player* p)
	{
		if (p->hasDrawnCard)
		{
			return;
		}

		Card* c = s_deck->draw();
		if (c == nullptr)
		{
			return;
		}
		p->getHand()->addCardToHand(c);
		p->hasDrawnCard = true;
	}

	Player* GameManager::getNeutralPlayer()
	{
		return s_neutralPlayer;
	}

	void GameManager::resetPlayerDrawCard()
	{
		for (Player* p : s_activePlayers)
		{
			p->hasDrawnCard = false;
		}
	}
}