#include "GameEngine.h"
#include "Player.h"
#include "Cards.h"
#include "Utils.h"

namespace WZ
{
	bool GameManager::isNegotiating(const Player* p1, const Player* p2) { return GetManager().isNegotiatingImpl(p1, p2); }

	void GameManager::addNegotiatingPlayers(const Player* p1, const Player* p2) { GetManager().addNegotiatingPlayersImpl(p1, p2); }
	
	void GameManager::drawCard(Player* p) { GetManager().drawCardImpl(p); }

	Player* GameManager::getNeutralPlayer() { return GetManager().getNeutralPlayerImpl(); }

	void GameManager::startupPhase(const Player* p, const Territory* t) { GetManager().startupPhaseImpl(p, t); }

	GameManager::GameManager() : m_neutralPlayer(new Player("Neutral")), m_deck(new Deck())
	{
		Random::Init();
	}
	
	GameManager::~GameManager()
	{
		delete m_neutralPlayer;
		delete m_deck;
	}

	
	GameManager& GameManager::GetManager()
	{
		//since the manager variable is static it will be kept in 
		//static memory allowing use to reuse it everytime we call the GetManager function
		static GameManager manager;
		return manager;
	}

	bool GameManager::isNegotiatingImpl(const Player* p1, const Player* p2)
	{
		for (std::pair<const Player*, const Player*> pair : m_negotiatingPlayers)
		{
			if ((pair.first == p1 && pair.second == p2) || (pair.first == p2 && pair.second == p1))
			{
				return true;
			}
		}
		return false;
	}

	void GameManager::addNegotiatingPlayersImpl(const Player* p1, const Player* p2)
	{
		m_negotiatingPlayers.push_back({ p1, p2 });
	}

	void GameManager::drawCardImpl(Player* p)
	{
		if (p->hasDrawnCard)
		{
			std::cout << p->getPlayerName() << " tries to draws a card but they already drew this turn\n";
			return;
		}

		std::cout << p->getPlayerName() << " draws a card\n";
		p->hasDrawnCard = true;

		Card* c = m_deck->draw();
		if (c == nullptr)
		{
			return;
		}
		p->getHand()->addCardToHand(c);
	}

	Player* GameManager::getNeutralPlayerImpl()
	{
		return m_neutralPlayer;
	}

	void GameManager::resetPlayerDrawCard()
	{
		for (Player* p : m_activePlayers)
		{
			p->hasDrawnCard = false;
		}
	}

	//to be implemented
	void GameManager::startupPhaseImpl(const Player* p, const Territory* t) {

	}

	GameEngine::GameEngine() 
	{ 
		this->userMap = "";
		this->userNumPlayers = 0;
}
	GameEngine::~GameEngine() 
	{
		delete map; 
		delete map_loader; 
		delete[] listOfPlayers; 
		delete deck; 

	}

	GameEngine::GameEngine(string map, int numplyrs) 
	{
		this->userMap = map;
		this->userNumPlayers = numplyrs;
		//gameStart();
	}
	
	string GameEngine::getUserMap() 
	{
		return this->userMap;
	}

	int GameEngine::getUserNumPlayers() 
	{
		return this->userNumPlayers;
	}

	void GameEngine::setUserMap(string mapname) 
	{
		this->userMap = mapname;
	}

	void GameEngine::setUserNumPlayers(int playersnum) 
	{
		this->userNumPlayers = playersnum;
	}

	void GameEngine::gameStart() 
	{
	}

	GameEngine& GameEngine::operator=(const GameEngine& Other)
	{
    userMap = Other.userMap;
    userNumPlayers = Other.userNumPlayers;
    map = Other.map;
    map_loader = Other.map_loader;
    listOfPlayers = Other.listOfPlayers;
    deck = Other.deck;
    return *this;
	}
}