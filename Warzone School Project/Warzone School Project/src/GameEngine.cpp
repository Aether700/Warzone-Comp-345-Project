#include "GameEngine.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "Utils.h"

#include <assert.h>
#include <sstream>
#include <random>
#include <chrono>
using namespace std; 

#define DEF_WIN_RATE 0.7f
#define ATK_WIN_RATE 0.6f

using std::cout;
using std::endl;
using std::cin;

namespace WZ
{
	std::string GetRandomPlayerName()
	{
		std::string names[] = { "Harry", "Timmy", "Dinkleberg", 
			"He Who Must Not Be Named", "Corvo", "Link" };

		return names[Random::GetInt() % (sizeof(names) / sizeof(std::string))];
	}

	std::string FormatBoolOption(const std::string& option, bool val)
	{
		std::stringstream ss;
		ss << option << ": ";

		if (val)
		{
			ss << "On";
		}
		else
		{
			ss << "Off";
		}

		return ss.str();
	}

	bool GameManager::isNegotiating(const Player* p1, const Player* p2) { return GetManager().isNegotiatingImpl(p1, p2); }

	void GameManager::addNegotiatingPlayers(const Player* p1, const Player* p2) { GetManager().addNegotiatingPlayersImpl(p1, p2); }
	
	void GameManager::drawCard(Player* p) { GetManager().drawCardImpl(p); }

	Player* GameManager::getNeutralPlayer() { return GetManager().getNeutralPlayerImpl(); }

	void GameManager::startupPhase() { GetManager().startupPhaseImpl(); }

	GameManager::GameManager() : m_neutralPlayer(new Player("Neutral")), m_deck(new Deck()),
		map(nullptr), currentphase(GamePhase::Reinforcement), m_lastOrder(nullptr)
	{
		Random::Init();
		Subject<PhaseObserver>::AddObserver(new PhaseObserver());
		Subject<StatisticsObserver>::AddObserver(new StatisticsObserver());
	}
	
	GameManager::~GameManager()
	{
		delete m_neutralPlayer;
		delete m_deck;
		delete m_lastOrder;
		delete map;

		for (Player* p : m_activePlayers)
		{
			delete p;
		}
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
		m_bufferList.push_back({ p1, p2 });
	}

	void GameManager::drawCardImpl(Player* p)
	{
		if (p->hasDrawnCard)
		{
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

	GamePhase GameManager::getCurrentPhase() {
		return GetManager().currentphase;
	}

	const Player* GameManager::getCurrentPlayer() {
		return GetManager().CurrentPlayer;
	}

	bool GameManager::gameStart()
	{
		if (GameManager::GetManager().getUserMap())
		{
			if (GameManager::GetManager().InitializePlayers())
			{
				return true;
			}
		}
		return false;
	}

	bool GameManager::getUserMap()
	{
		delete map;
		while(true){
			//loads map of both file format (asks user to pick file)
			map = LoaderInterface::LoadMap();
			if (map == nullptr)
			{
				return false;
			}

			if (map->validate())
			{
				break;
			}
			else
			{
				std::cout << "The Map chosen is not valid. Please pick a new one.\n";
			}
		}
		return true;
	}
	
	int GameManager::getUserNumPlayers()
	{
		while(true)
		{
			std::cout << "Player with how many players?\n";
			int num = Clamp(2, 5, AskInt());

			std::cout << "\nPlay with " << num << " players?\n";
			if (AskYN())
			{
				return num;
			}

			std::cout << "\nExit player selection?\n";

			if (AskYN())
			{
				return -1;
			}
		}
	}

	bool GameManager::InitializePlayers(){
		
		for (Player* p : m_activePlayers) 
		{
			delete p;
		}
		m_activePlayers.clear();

		int num=getUserNumPlayers();

		if (num == -1)
		{
			return false;
		}

		m_activePlayers.reserve(num);

		string name = "";
		std::cout << "Please enter your player name: ";
		std::cin >> name;

		Player* human = new Player(name);
		human->SetStrategy(new HumanPlayerStrategy(human));
		m_activePlayers.push_back(human);

		for(int i = 1 ; i < num ; i++){
			name = GetRandomPlayerName();
			m_activePlayers.push_back(new Player(name));
		}

		return true;
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
		if(PhaseObsOn)
		{
			Subject<PhaseObserver>::notifyObservers();
		}
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
		if (StatsObsOn)
		{
			Subject<StatisticsObserver>::notifyObservers();
		}
	}

	const Order* GameManager::getLastOrder(){
		return GetManager().getLastOrderImpl();
	}

	void GameManager::SettingsMenu()
	{
		GetManager().SettingsMenuImpl();
	}

	const Order* GameManager::getLastOrderImpl() const{
		return m_lastOrder;
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

	void GameManager::SettingsMenuImpl()
	{
		std::vector<std::string> options = { "", "" };
		
		bool done = false;

		while (!done)
		{
			//update display of values for the booleans
			options[0] = FormatBoolOption("Phase Observers", PhaseObsOn);
			options[1] = FormatBoolOption("Statistic Observers", StatsObsOn);

			std::cout << "\tSettings\n\n";

			//ask user input and toggle the correct phase observer bool or exit the menu
			switch(AskInput(options, "Back"))
			{
			case 1:
				PhaseObsOn = !PhaseObsOn;
				break;

			case 2:
				StatsObsOn = !StatsObsOn;
				break;

			case -1:
				done = true;
				break;
			}

		}
	}

	bool GameManager::Attack(Territory* source, Territory* target, unsigned int amount)
	{
		assert(amount != 0);
		assert(source != nullptr);
		assert(target != nullptr);
		assert(source->getOwner() != GameManager::getNeutralPlayer());

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

	unsigned int GameManager::reinforcementCalculator(Player* currentPlayer) {
		int bonus = 0;															//	bonus base
		for (size_t i = 0; i < map->getContinentCount(); i++) {								//	loop for each continent in current map
			for (size_t count = 0; count < map->getContinent(i)->getCount(); count++) {		//	loop for each territory in current continent
				if (!(currentPlayer->ownsTerritory(map->getContinent(i)->getTerritory(count))))	//	if the territory is not hold by player
					break;																			// >>	break the loop, go to next continent													
				if (count == (map->getContinent(i)->getCount() - 1))					//	if the number of checked territories equals the size of continent
				{
					bonus += map->getContinent(i)->getBonus();									// >>	give reinforcement bonus
				}
			}
		}
		if (currentPlayer->getNumOfTerritories() < 11)							//	if player has < 11 territories
			return (3 + bonus);														// >>fs	there is a minimum amount of reinforcements guaranteed (= 3)
		else
			return bonus + (currentPlayer->getNumOfTerritories() / 3);			//	returning the (int) number_of_territories/3 as reinforcement (+ bonus)
	}

	void GameManager::reinforcementPhase() {
		delete m_lastOrder;
		m_lastOrder = nullptr;
		//	for all active players in the game
		for (size_t i = 0; i < m_activePlayers.size(); i++) {					
			CurrentPlayer = m_activePlayers[i];
			m_activePlayers[i]->m_strategy->m_reinforcements = reinforcementCalculator(m_activePlayers[i])
				+ m_activePlayers[i]->m_strategy->m_reinforcements;	//	generate reinforcements

			GameManager::NotifyPhaseObserver();
		}
	}

	void GameManager::issueOrdersPhase()
	{
		//reset all the toDefend and toAttack vectors
		for (Player* p : m_activePlayers)
		{
			p->generateTerritoryLists();
		}

		//reset the available armies of all the territories on the map
		for (auto pair : *map)
		{
			Territory* curr = pair.second;

			curr->m_availableArmies = curr->m_armies;
		}

		std::vector<Player*> issuingPlayers = m_activePlayers;

		while (!issuingPlayers.empty())
		{
			std::cout << *map << "\n\n"; //prints map for user
			for (size_t i = 0; i < issuingPlayers.size(); i++)
			{
				Order* currOrder = issuingPlayers[i]->issueOrder();

				if (currOrder == nullptr)
				{
					issuingPlayers.erase(issuingPlayers.begin() + i);
				}
				else
				{
					CurrentPlayer = issuingPlayers[i];
					m_lastOrder = currOrder;
					issuingPlayers[i]->listOrders->addOrder(m_lastOrder);
					GameManager::NotifyPhaseObserver();
				}
			}
		}
	}

	/*	This function will execute orders in priority order - from each player 
			in a Round-Robin sequence. Each succesfull execution will return a true value
			which will set the control variable to true - keeping the loop active*/
	void GameManager::executeOrderPhase(){
		bool ordersLeft = true;		/*	control variable for the Round-Robin execution loop. 
										True initial value starts the loop.*/

		m_lastOrder = nullptr;
		while (ordersLeft) {
			ordersLeft = false;			//	False value allows OR operations over each player


			for (Player* p : m_activePlayers) {	
				CurrentPlayer = p;
				Order* currOrder = p->executeTopOrder();
				ordersLeft |= currOrder != nullptr;
				/*	if there are order executions happening, the loop will go on.
				Loop will run a last empty round when all players will have their 
						order lists empty to set variable ordersLeft to false*/

				if (currOrder != nullptr)
				{
					delete m_lastOrder;
					m_lastOrder = currOrder;
					GameManager::NotifyPhaseObserver();
				}
			}
		}
	}

	bool GameManager::mainGameLoop() {
		bool done = false;
		while (m_activePlayers.size() > 1 && !done) {

			currentphase = GamePhase::Reinforcement;
			reinforcementPhase();					
			std::cout << "\n";

			currentphase = GamePhase::IssuingOrders;
			issueOrdersPhase();				
			std::cout << "\n";

			currentphase = GamePhase::OrderExecution;
			executeOrderPhase();						
			std::cout << "\n";

			//	eliminate players with no territories left
			for (size_t i = 0; i < m_activePlayers.size(); i++) {	
				if (m_activePlayers[i]->getNumOfTerritories() == 0) {
					cout << m_activePlayers[i]->getPlayerName() << " was eliminated. " << endl;
					Player* eliminated = m_activePlayers[i];

					if (m_humanPlayer == eliminated)
					{
						done = true;
						break;
					}

					m_activePlayers.erase(m_activePlayers.begin() + i);
					GameManager::NotifyStatisticsObserver();
					delete eliminated;
				}
			}

			//reset hasDrawnCard
			for (Player* p : m_activePlayers)
			{
				p->hasDrawnCard = false;
			}

			//update negotiating players
			m_negotiatingPlayers = m_bufferList;
			m_bufferList.clear();
			std::cout << "=======================New Turn==========================\n";
		}

		//we are only done if the human player lost therefore return !done
		return !done;
	}

	void GameManager::startupPhaseImpl() {

		//Randomize the order of the player
		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine default_random_engine(seed);
		std::shuffle(m_activePlayers.begin(), m_activePlayers.end(), default_random_engine);
		
		//get territories
		std::vector<Territory*> territories;
		territories.reserve(map->getTerritoryCount());
		for (std::pair<unsigned int, Territory*> pair : *map)
		{
			territories.push_back(pair.second);
		}
		//randomize the territories
		std::shuffle(territories.begin(), territories.end(), default_random_engine);

		//assign territories
		int playerIndex = 0;
		for (Territory* curr : territories)
		{
			m_activePlayers[playerIndex]->addTerritory(curr);
			playerIndex = (playerIndex + 1) % m_activePlayers.size();
		}

 		int armies;
		switch (m_activePlayers.size())
		{
			case 2:
				armies = 40;
				break;

			case 3:
				armies = 35;
				break;

  			case 4:
				armies = 30;
			  	break;

  			case 5:
				armies = 25;
			  	break;
		}

		for (Player* p : m_activePlayers)
		{
			p->m_strategy->m_reinforcements = armies;
		}
	}

	void GameManager::RunGame()
	{
		while (true)
		{
			if (gameStart())
			{
				startupPhase();
				if (!GetManager().mainGameLoop())
				{
					//this print statement will merge with the one below
					std::cout << "You lost. "; 
				}

				std::cout << "Play again?\n\n";
				if (!AskYN())
				{
					break;
				}
			}
		}
	}

	void GameManager::MainMenu()
	{
		std::array<const char*, 2> choices = { "Play", "Settings" };

		while (true)
		{
			std::cout << "\tWelcome to Warzone\n\n";

			switch(AskInput(choices, "Exit"))
			{
			case 1:
				RunGame();
				break;

			case 2: 
				SettingsMenu();
				break;

			case -1:
				return;
			}
		}

	}

}