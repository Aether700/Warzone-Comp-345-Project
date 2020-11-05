#include "GameEngine.h"
#include "Player.h"
#include "Cards.h"
#include "Utils.h"

#include <assert.h>

#define DEF_WIN_RATE 0.7f
#define ATK_WIN_RATE 0.6f

namespace WZ
{
	bool GameManager::isNegotiating(const Player* p1, const Player* p2) { return GetManager().isNegotiatingImpl(p1, p2); }

	void GameManager::addNegotiatingPlayers(const Player* p1, const Player* p2) { GetManager().addNegotiatingPlayersImpl(p1, p2); }
	
	void GameManager::drawCard(Player* p) { GetManager().drawCardImpl(p); }

	Player* GameManager::getNeutralPlayer() { return GetManager().getNeutralPlayerImpl(); }

	void GameManager::startupPhase(const Player* p, const Territory* t) { GetManager().startupPhaseImpl(p, t); }

	GameManager::GameManager() : m_neutralPlayer(new Player("Neutral")), m_deck(new Deck()),
		map(nullptr), currentphase(GamePhase::Reinforcement)
	{
		Random::Init();
	}
	
	GameManager::~GameManager()
	{
		delete m_neutralPlayer;
		delete m_deck;
		delete map;
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

	GamePhase GameManager::getCurrentPhase() {
		return GetManager().currentphase;
	}

	const Player* GameManager::getCurrentPlayer() {
		return GetManager().m_activePlayers[GetManager().CurrentPlayerIndex];
	}

	void GameManager::getUserMap()
	{
		MapLoader loader;
		delete map;
		//will automatically ask the user to select a map file and generate the map from that file if it is valid
		map = loader.mapGenerator(); 
	}
	
	int GameManager::getUserNumPlayers()
	{
		while(true)
		{
			std::cout << "Player with how many players?\n";
			int num = Clamp(2, 5, AskInt());

			std::cout << "Play with " << num << " players?\n";
			if (AskYN())
			{
				return num;
			}
		}
	}

	//PHASE OBSERVER IMPLEMENTATION
	void GameManager::AddPhaseObserver(PhaseObserver* p){
		GetManager().AddPhaseObserverImpl(p);
	}

	void GameManager::RemovePhaseObserver(PhaseObserver* p){
		GetManager().RemovePhaseObserverImpl(p);
	}
	
	void GameManager::NotifyPhaseObserver() {
		GetManager().NotifyPhaseObserversImpl();
	}

	void GameManager::AddPhaseObserverImpl(PhaseObserver* p){
		Subject<PhaseObserver>::AddObserver(p);
	}

	void GameManager::RemovePhaseObserverImpl(PhaseObserver* p){
		Subject<PhaseObserver>::removeObserver(p);
	}

	void GameManager::NotifyPhaseObserversImpl() const{
		Subject<PhaseObserver>::notifyObservers();
	}

	//STATISTICS OBSERVER IMPLEMENTATION
	void GameManager::AddStatisticsObserver(StatisticsObserver* p){
		GetManager().AddStatisticsObserverImpl(p);
	}

	void GameManager::RemoveStatisticsObserver(StatisticsObserver* p){
		GetManager().RemoveStatisticsObserverImpl(p);
	}

	void GameManager::NotifyStatisticsObserver()
	{
		GetManager().NotifyStatisticsObserverImpl();
	}

	void GameManager::AddStatisticsObserverImpl(StatisticsObserver* p){
		Subject<StatisticsObserver>::AddObserver(p);
	}

	void GameManager::RemoveStatisticsObserverImpl(StatisticsObserver* p){
		Subject<StatisticsObserver>::removeObserver(p);
	}

	void GameManager::NotifyStatisticsObserverImpl() const {
		Subject<StatisticsObserver>::notifyObservers();
	}


	const std::vector <Player*>& GameManager::getActivePlayers(){
		return GetManager().getActivePlayersImpl();
	}

	const std::vector <Player*>& GameManager::getActivePlayersImpl() const{
		return m_activePlayers;
	}

	
	const Map* GameManager::getMap(){
		return GetManager().getMapImpl();
	}

	const Map* GameManager::getMapImpl() const{
		return map;
	}

	bool GameManager::Attack(Territory* source, Territory* target, unsigned int amount)
	{
		assert(amount != 0);
		assert(source != nullptr);
		assert(target != nullptr);

		unsigned int initialAmount = amount;

		unsigned int atkCasualties = 0;
		unsigned int defCasualties = 0;

		//atk casualties (defenders killing attackers)
		for (size_t i = 0; i < target->getArmies() && amount > atkCasualties; i++)
		{
			if (Random::GetFloat() <= DEF_WIN_RATE)
			{
				atkCasualties++;
			}
		}


		//def casualties (attackers killing defenders)
		for (size_t i = 0; i < amount && target->getArmies() > defCasualties; i++)
		{
			if (Random::GetFloat() <= ATK_WIN_RATE)
			{
				defCasualties++;
			}
		}

		//apply casualties to defenders
		target->setArmies(target->getArmies() - defCasualties);
		amount -= atkCasualties;


		if (target->getArmies() == 0)
		{
			target->getOwner()->removeTerritory(target);
			source->getOwner()->addTerritory(target);
			target->setArmies(amount);
			source->setArmies(source->getArmies() - initialAmount);

			GameManager::drawCard(source->getOwner());
			GameManager::NotifyStatisticsObserver();
			return true;
		}

		source->setArmies(source->getArmies() - initialAmount + amount);

		return false;
	}


}