#include "GameEngine.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "Utils.h"

#include <assert.h>
#include <sstream>
#include <random>
using namespace std; 

#define DEF_WIN_RATE 0.7f
#define ATK_WIN_RATE 0.6f

using std::cout;
using std::endl;
using std::cin;

namespace WZ
{
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

	//void GameEngine::startupPhase(const Player* p, const Territory* t, int armies) { GetManager().startupPhaseImpl(p, t, armies); }

	GameManager::GameManager() : m_neutralPlayer(new Player("Neutral")), m_deck(new Deck()),
		map(nullptr), currentphase(GamePhase::Reinforcement), m_lastOrder(nullptr)
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
		while(true){
			//will automatically ask the user to select a map file and generate the map from that file if it is valid
			map = loader.mapGenerator();
			if(map!=nullptr)
			{
				if(map->validate())
				{
					break;
				}
			}
			else
			{
				std::cout<<"Invalid map"<<std::endl;
			}
		}
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

	void GameManager::InitializePlayers(){
		int num=getUserNumPlayers();
		m_activePlayers.reserve(num);

		for(int i = 0 ; i < num ; i++){
			string name="";
			std::cout<<"Please enter player "<<(i+1)<<"name: ";
			std::cin>>name;
			m_activePlayers.push_back(new Player(name));
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
		std::vector<std::string> options;
		options.reserve(2);

		/*pushing back elements so we can modify them in the loop.
		  the value of those elements doesn't matter since they 
		  will be overwritten in the loop before being displayed
		*/
		options.push_back("");
		options.push_back("");

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
				if (count == (map->getContinent(i)->getCount() - 1))						//	if the number of checked territories equals the size of continent
					bonus += map->getContinent(i)->getBonus();									// >>	give reinforcement bonus
			}
		}
		if (currentPlayer->getNumOfTerritories() < 11)							//	if player has < 11 territories
			return (3 + bonus);														// >>fs	there is a minimum amount of reinforcements guaranteed (= 3)
		else
			return bonus + (currentPlayer->getNumOfTerritories() / 3);			//	returning the (int) number_of_territories/3 as reinforcement (+ bonus)
	}

	void GameManager::reinforcementPhase() {
		for (size_t i = 0; i < m_activePlayers.size(); i++) {					//	for all active players in the game
			m_activePlayers[i]->setReinforcements(reinforcementCalculator(m_activePlayers[i]));	//	generate reinforcements
		}
	}

	void GameManager::issueOrdersPhase()
	{
		//reset all the toDefend and toAttack vectors
		for (Player* p : m_activePlayers)
		{
			p->GenerateToTerritoryLists();
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
			for (size_t i = 0; i < issuingPlayers.size(); i++)
			{
				Order* currOrder = issuingPlayers[i]->issueOrder();

				if (currOrder == nullptr)
				{
					issuingPlayers.erase(issuingPlayers.begin() + i);
				}
				else
				{
					delete m_lastOrder;
					m_lastOrder = currOrder;
					issuingPlayers[i]->listOrders->addOrder(m_lastOrder);
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
		while (ordersLeft) {
			ordersLeft = false;			//	False value allows OR operations over each player
			for (Player* p : m_activePlayers) {	
					ordersLeft |= p->executeTopOrder();
					/*	if there are order executions happening, the loop will go on.
					Loop will run a last empty round when all players will have their 
							order lists empty to set variable ordersLeft to false*/
			}
		}
	}

	void GameManager::mainGameLoop() {
		while (m_activePlayers.size() > 1) {		//	
			reinforcementPhase();					//	
			issueOrdersPhase();						//	
			executeOrderPhase();					//	

		//	eliminate players with no territories left
			for (size_t i = 0; i < m_activePlayers.size(); i++) {	
				if (m_activePlayers[i]->getNumOfTerritories() == 0) {
					cout << m_activePlayers[i]->getPlayerName() << " has left the game. " << endl;
					m_activePlayers.erase(m_activePlayers.begin() + i);
				}
			}
		}
	}

	void GameManager::startupPhaseImpl() {
	
		  //Randomize the order of the player
		  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		   std::default_random_engine default_random_engine(seed);
		   std::shuffle(m_activePlayers.begin(), m_activePlayers.end(), default_random_engine);
		  
		  //Assign Players some territories
		   const std::vector<Territory*>* territories = map->getContinent();
		   std::shuffle(territories->begin(), territories->end(), default_random_engine);

			for (int i = 0; i < territories->size(); i++) {
				m_activePlayers.at(i % m_activePlayers.size())->addTerritory(territories->at(i));
			}

		  switch (m_activePlayers.size())
		  {
			  case 2:
				  std::cout << "Each player will be given 40 armies\n ";
				  armies = 40;
				  break;

			  case 3:
				  std::cout << "Each player will be given 35 armies\n ";
				  armies = 35;
				  break;

  			case 4:

	  			std::cout << "Each player will be given 30 armies\n ";
				armies = 30;
			  	break;

  			case 5:
	  			std::cout << "Each player will be given 25 armies\n ";
				armies = 25;
			  	break;

  			default:
	  			std::cout << "Only 2 to 5 players are accepted in this game";

		}
 
		//determine the order of players randomly

		//Randomly assign territories to players one by one in a round-robin fashion
  }
}