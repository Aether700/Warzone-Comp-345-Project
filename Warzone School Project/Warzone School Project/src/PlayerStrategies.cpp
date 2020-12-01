#include "PlayerStrategies.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Utils.h"

#include <assert.h>
#include <array>
#include <sstream>
#include <random>
#include <chrono>

#define STRONGEST_THRESHOLD 20
#define BLOCKADE_THRESHOLD 15
#define CARD_PLAY 0.5f

namespace WZ
{
	//Helper functions ///////////////////////////////////////////////////

	const char* CardTypeToStr(Card::Type type)
	{
		switch (type)
		{
		case Card::Type::Bomb:
			return "Bomb";

		case Card::Type::Reinforcement:
			return "Reinforcement";

		case Card::Type::Blockade:
			return "Blockade";

		case Card::Type::Airlift:
			return "Airlift";

		case Card::Type::Diplomacy:
			return "Diplomacy";
		}
	}

	// PlayerStrategy /////////////////////////////////////////////////////

	PlayerStrategy::PlayerStrategy(Player* player) : m_player(player), m_reinforcements(0)
	{
		assert(player != nullptr);
	}

	PlayerStrategy::PlayerStrategy(const PlayerStrategy& other) 
		: m_player(other.m_player), m_reinforcements(other.m_reinforcements)
	{
		m_toAtk.reserve(other.m_toAtk.size());

		for (Territory* t : other.m_toAtk)
		{
			m_toAtk.push_back(t);
		}

		m_toDef.reserve(other.m_toDef.size());

		for (Territory* t : other.m_toDef)
		{
			m_toDef.push_back(t);
		}
	}

	PlayerStrategy::~PlayerStrategy() { }

	std::vector<Territory*>& PlayerStrategy::toDefend() { return m_toDef; }
	
	std::vector<Territory*>& PlayerStrategy::toAttack() { return m_toAtk; }

	void PlayerStrategy::SetPlayer(Player* p) 
	{
		assert(p != nullptr);
		m_player = p; 
	}

	PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& other)
	{
		m_player = other.m_player;
		m_reinforcements = other.m_reinforcements;

		m_toAtk.clear();
		m_toAtk.reserve(other.m_toAtk.size());

		for (Territory* t : other.m_toAtk)
		{
			m_toAtk.push_back(t);
		}

		m_toDef.clear();
		m_toDef.reserve(other.m_toDef.size());

		for (Territory* t : other.m_toDef)
		{
			m_toDef.push_back(t);
		}

		return *this;
	}

	// HumanPlayerStrategy ///////////////////////////////////////////////////////////////

	HumanPlayerStrategy::HumanPlayerStrategy(Player* player) : PlayerStrategy(player) { }

	HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& other) : PlayerStrategy(other) { }

	HumanPlayerStrategy::~HumanPlayerStrategy() { }

	Order* HumanPlayerStrategy::issueOrder()
	{
		constexpr  std::array<const char*, 2> choices = { "Play Card", "Advance Order"};

		Order* o;
		while(true)
		{
			if (m_reinforcements != 0)
			{
				return DeployArmies();
			}
			else
			{
				int choice = AskInput(choices, "End Turn");
				std::cout << "\n";

				switch (choice)
				{
				case 1:
					o = PlayCard();
					if (o != nullptr)
					{
						return o;
					}
					break;

				case 2:
					o = Advance();
					if (o != nullptr)
					{
						return o;
					}
					break;
				
				case -1:
					return nullptr;
				}
			}
		}
	}

	void HumanPlayerStrategy::generateTerritoryLists()
	{
		m_toDef = m_player->getTerritories();
		m_toAtk.clear();

		for (Territory* t : m_player->getTerritories())
		{
			for (Territory* adj : t->getAdjList())
			{
				if (adj->getOwner() != m_player)
				{
					m_toAtk.push_back(adj);
				}
			}
		}
	}

	HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& other)
	{
		PlayerStrategy::operator=(other);
		return *this;
	}

	PlayerStrategy* HumanPlayerStrategy::copy() const { return new HumanPlayerStrategy(*this); }

	Order* HumanPlayerStrategy::PlayCard()
	{
		if (m_player->getHand()->getCount() == 0)
		{
			std::cout << "You do not own any cards at this moment\n";
			return nullptr;
		}

		std::vector<std::string> cardStr;
		int size = m_player->getHand()->getCount() < 5 ? m_player->getHand()->getCount() : 5;
		cardStr.reserve(size);

		int count = 0;

		for(Card* c : *m_player->getHand())
		{
			//early break if all card types have been found
			if (count == size)
			{
				break;
			}

			for (std::string type : cardStr)
			{
				if (type == CardTypeToStr(c->getType()))
				{
					continue;
				}
			}
			cardStr.push_back(CardTypeToStr(c->getType()));
			count++;
		}

		while (true)
		{
			int choice = AskInput(cardStr, "Back");

			if (choice == -1)
			{
				return nullptr;
			}

			Card* toPlay = (*m_player->getHand())[choice - 1];

			Order* o;

			switch (toPlay->getType())
			{
			case Card::Type::Reinforcement:
				toPlay->play(nullptr, nullptr, m_player, nullptr, 0);
				m_player->getHand()->removeCardFromHand(toPlay);
				return issueOrder();

			case Card::Type::Bomb:
				o = PlayBombCard(toPlay);
				if (o != nullptr)
				{
					m_player->getHand()->removeCardFromHand(toPlay);
					return o;
				}
				break;

			case Card::Type::Airlift:
				o = PlayAirliftCard(toPlay);
				if (o != nullptr)
				{
					m_player->getHand()->removeCardFromHand(toPlay);
					return o;
				}
				break;

			case Card::Type::Diplomacy:
				o = PlayDiplomacyCard(toPlay);
				if (o != nullptr)
				{
					m_player->getHand()->removeCardFromHand(toPlay);
					return o;
				}
				break;

			case Card::Type::Blockade:
				o = PlayBlockadeCard(toPlay);
				if (o != nullptr)
				{
					m_player->getHand()->removeCardFromHand(toPlay);
					return o;
				}
				break;
			}
		}
	}

	Territory* HumanPlayerStrategy::SelectOwnedTerritory(bool backMessage)
	{
		std::vector<std::string> strList;
		strList.reserve(toDefend().size());


		for (Territory* t : toDefend())
		{
			strList.push_back(t->getName());
		}

		int choice;
		
		if (backMessage)
		{
			choice = AskInput(strList, "Back");

			if (choice == -1)
			{
				return nullptr;
			}
		}
		else
		{
			choice = AskInput(strList);
		}

		return toDefend()[choice - 1];
	}
	
	DeployOrder* HumanPlayerStrategy::DeployArmies()
	{
		assert(m_reinforcements != 0);

		std::cout << "Which territory do you wish to deploy armies on?\n";

		Territory* target = SelectOwnedTerritory(false);

		int numArmies;

		while (true)
		{
			std::cout << "How many armies do you wish to deploy on " << target->getName() << "?\n";

			numArmies = Clamp(1, m_reinforcements, AskInt());

			std::cout << "Deploy " << numArmies << " Armies?\n";
			if (AskYN())
			{
				break;
			}
		}

		return new DeployOrder(m_player, target, numArmies);
	}

	AdvanceOrder* HumanPlayerStrategy::Advance()
	{
		while (true)
		{
			std::cout << "From which territory do you want to advance?\n";

			Territory* src = SelectOwnedTerritory();

			if (src == nullptr)
			{
				return nullptr;
			}
			//make sure the territory has some available armies
			else if (src->getAvailableArmies() == 0)
			{
				std::cout << "This territory has no available armies. Please choose another one.\n";
				continue;
			}
			//make sure territory is not a trap territory
			else if (src->getAdjList().size() == 0)
			{
				std::cout << "This territory does not allow armies to leave it. Please choose another one.\n";
				continue;
			}

			std::vector<std::string> targetList;

			for (Territory* t : src->getAdjList())
			{
				targetList.push_back(t->getName());
			}

			std::cout << "Which territory should be the target of your advance?\n";

			int choice = AskInput(targetList, "Back");

			if (choice == -1)
			{
				//allow user to choose another source territory if selects back
				continue;
			}

			unsigned int numArmies;

			while (true)
			{
				std::cout << "How many armies do you wish to send?\t" 
					<< src->getName() << ": " << src->getAvailableArmies() << " Armies\n";
				numArmies = Clamp(1, src->getAvailableArmies(), AskInt());

				std::cout << "Are you sure you want to send " << numArmies << " Armies?\n";
				if (AskYN())
				{
					break;
				}
			}

			src->m_availableArmies -= numArmies;
			return new AdvanceOrder(m_player, src, src->getAdjList()[choice -1], numArmies);
		}
	}

	BlockadeOrder* HumanPlayerStrategy::PlayBlockadeCard(Card* c)
	{
		assert(c->getType() == Card::Type::Blockade);
		std::cout << "Which territory do you want to blockade?\n";

		Territory* target = SelectOwnedTerritory();

		if (target == nullptr)
		{
			return nullptr;
		}

		return new BlockadeOrder(m_player, target);
	}

	BombOrder* HumanPlayerStrategy::PlayBombCard(Card* c)
	{
		assert(c->getType() == Card::Type::Bomb);

		//create a list of territories that can be bombed
		std::vector<Territory*> targetList = toAttack();
	
		//make a string vector to pass to AskInput to prompt the user to select a territory

		std::vector<std::string> targetListStr;
		targetListStr.reserve(targetList.size());

		for (Territory* t : targetList)
		{
			targetListStr.push_back(t->getName());
		}

		int choice = AskInput(targetListStr, "Back");

		//if user didn't back return the BombOrder
		if (choice == -1)
		{
			return nullptr;
		}

		return new BombOrder(m_player, targetList[choice - 1]);
	}

	AirliftOrder* HumanPlayerStrategy::PlayAirliftCard(Card* c)
	{
		assert(c->getType() == Card::Type::Airlift);

		while (true)
		{
			std::cout << "From which Territory do you want to send the armies?\n\n";

			Territory* src = SelectOwnedTerritory();

			if (src == nullptr)
			{
				return nullptr;
			}
			//make sure there are some available armies on the choosen territory
			else if (src->getAvailableArmies() == 0)
			{
				std::cout << "There is no available armies on this territory. Please choose another one.\n";
				continue;
			}

			Territory* target;

			if (src == nullptr)
			{
				return nullptr;
			}

			//shortened scope to free vector memory as early as possible
			{
				//create a vector of all the territories not owned by the player owning this Strategy object
				std::vector<Territory*> targetList;

				const Map* map = GameManager::getMap();

				for (auto pair : *map)
				{
					Territory* curr = pair.second;

					if (curr != src)
					{
						targetList.push_back(curr);
					}
				}

				//make a string vector to provide as input to AskInput function
				std::vector<std::string> targetListStr;
				targetListStr.reserve(targetList.size());

				for (Territory* t : targetList)
				{
					targetListStr.push_back(t->getName());
				}

				std::cout << "\nChoose a target territory:\n";

				int choice = AskInput(targetListStr, "Back");

				if (choice == -1)
				{
					//return to top of loop to allow user to select another src territory
					continue;
				}

				target = targetList[choice - 1];
			}

			unsigned int numArmies;

			while (true)
			{
				std::cout << "\nHow many armies do you want to send?\t"
					<< src->getName() << ": " << src->getAvailableArmies() << " Armies\n";
				numArmies = Clamp(1, src->getAvailableArmies(), AskInt());

				std::cout << "\nAre you sure you want to send " << numArmies << " Armies?\n";
				if (AskYN())
				{
					break;
				}
			}

			src->m_availableArmies -= numArmies;
			return new AirliftOrder(m_player, src, target, numArmies);
		}
	}
	
	NegotiateOrder* HumanPlayerStrategy::PlayDiplomacyCard(Card* c)
	{
		assert(c->getType() == Card::Type::Diplomacy);

		//make a vector of players to choose from
		std::vector<Player*> targetPlayers;

		for (Player* p : GameManager::getActivePlayers())
		{
			if (p != m_player && p != GameManager::getNeutralPlayer())
			{
				targetPlayers.push_back(p);
			}
		}

		//make vector of string to pass as input to AskInput
		std::vector<std::string> targetStr;
		targetStr.reserve(targetPlayers.size());

		for (Player* p : targetPlayers)
		{
			targetStr.push_back(p->getPlayerName());
		}

		int choice = AskInput(targetStr, "Back");

		if (choice == -1)
		{
			return nullptr;
		}

		return new NegotiateOrder(m_player, targetPlayers[choice - 1]);
	}

	std::ostream& operator<<(std::ostream& stream, const HumanPlayerStrategy&)
	{
		stream << "Human Player Strategy";
		return stream;
	}

	// NeutralPlayerStrategy ////////////////////////////////////////////////////////////////

	NeutralPlayerStrategy::NeutralPlayerStrategy(Player* player) : PlayerStrategy(player) { }

	NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& other) : PlayerStrategy(other) { }

	NeutralPlayerStrategy::~NeutralPlayerStrategy() { }

	Order* NeutralPlayerStrategy::issueOrder() { return nullptr; }

	void NeutralPlayerStrategy::generateTerritoryLists() { }

	NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& other)
	{
		PlayerStrategy::operator=(other);
		return *this;
	}

	PlayerStrategy* NeutralPlayerStrategy::copy() const { return new NeutralPlayerStrategy(*this); }

	std::ostream& operator<<(std::ostream& stream, const NeutralPlayerStrategy&)
	{
		stream << "Neutral Player Strategy";
		return stream;
	}
	
	// AggressivePlayerStrategy ///////////////////////////////////////////////////////////////

	AggressivePlayerStrategy::AggressivePlayerStrategy(Player* player) : PlayerStrategy(player) { }

	AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& other) : PlayerStrategy(other) { }

	AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

	AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& other)
	{
		PlayerStrategy::operator=(other);
		return *this;
	}

	Order* AggressivePlayerStrategy::issueOrder() {
		std::vector<Territory*> defend = toDefend();
		std::vector<Territory*> attack = toAttack();

		if(m_reinforcements > 0){
			Territory* territory;

			if (defend.empty())
			{
				std::vector<Territory*> owned = m_player->getTerritories();
				territory = owned[0];
				for (int i = 0; i < owned.size(); i++)
				{
					if (territory->getArmies() < owned[i]->getArmies())
					{
						territory = owned[i];
					}
				}
			}
			else
			{
				territory = toDefend().at(0);
			}

			int armies = m_reinforcements;
			m_reinforcements = 0;
			return new DeployOrder(m_player, territory, armies);
		}

		//if no strongest territory to attack from or reinforce return null and end turn
		if (defend.empty())
		{
			return nullptr;
		}

		if (Random::GetFloat() <= 0.8f){ //80% of the time (defense play)
			if(attack.empty()){
				return NULL;
			}

			if((m_player->hasCardType(Card::Type::Bomb) || m_player->hasCardType(Card::Type::Airlift))
				&& Random::GetFloat() < 0.5f){
				Card* card;
				if (!m_player->hasCardType(Card::Type::Bomb)){
					card = m_player->getCardType(Card::Type::Airlift);
				}
				else if (!m_player->hasCardType(Card::Type::Airlift)){
					card = m_player->getCardType(Card::Type::Bomb);
				}
				else{
					if(Random::GetFloat() <=0.5){  //pick 50% between both
						card = m_player->getCardType(Card::Type::Airlift);
					}
					else
					{
						card = m_player->getCardType(Card::Type::Bomb);
					}
				}

				//remove card from player hand
				m_player->getHand()->removeCardFromHand(card);

				Territory* target = attack.at(0);
				attack.erase(attack.begin());
				if(card->getType() == (Card::Type::Bomb)){
					return card->play(nullptr, target, m_player, nullptr, 0);
				}
				Territory *source = defend.at(0);
				int armies = source->getAvailableArmies();
				source->m_availableArmies = 0;
				defend.erase(defend.begin());
				return card->play(source, target, m_player, nullptr, armies);
			}
			
			if (m_player->hasCardType(Card::Type::Reinforcement))
			{
				Card* toPlay = m_player->getCardType(Card::Type::Reinforcement);
				m_player->getHand()->removeCardFromHand(toPlay);
				toPlay->play(nullptr, nullptr, m_player, nullptr, 0);
				delete toPlay;
				return issueOrder();
			}
			Territory *strongest = defend.at(0);

			if (m_player->hasCardType(Card::Type::Blockade))
			{
				//check if we blockade
				if (ShouldBlockade(strongest))
				{
					Card* toPlay = m_player->getCardType(Card::Type::Blockade);
					return toPlay->play(nullptr, strongest, m_player, nullptr, 0);
				}
			}

			Player* playerTarget = nullptr;
			if (m_player->hasCardType(Card::Type::Diplomacy))
			{
				playerTarget = GetNegotiatingTarget(strongest);
			}

			Territory* src = nullptr;

			if (m_player->hasCardType(Card::Type::Airlift))
			{
				src = GetSourceTerritory(strongest);
			}

			if ((src != nullptr || playerTarget != nullptr) && Random::GetFloat() <= CARD_PLAY)
			{
				//play card
				Card::Type typeToPlay;
				if (src == nullptr)
				{
					typeToPlay = Card::Type::Diplomacy;
				}
				else if (playerTarget == nullptr)
				{
					typeToPlay = Card::Type::Airlift;
				}
				else
				{
					//if we have both cards pick one card randomly
					if (Random::GetFloat() <= 0.5f)
					{
						typeToPlay = Card::Type::Diplomacy;
					}
					else
					{
						typeToPlay = Card::Type::Airlift;
					}
				}

				Card* toPlay = m_player->getCardType(typeToPlay);

				if (typeToPlay == Card::Type::Diplomacy)
				{
					m_player->getHand()->removeCardFromHand(toPlay);
					Order* o = toPlay->play(nullptr, nullptr, m_player, playerTarget, 0);
					delete toPlay;
					return o;
				}
				else
				{
					m_player->getHand()->removeCardFromHand(toPlay);
					unsigned int armiesToSend = src->getAvailableArmies();
					src->m_availableArmies -= armiesToSend;
					Order* o = toPlay->play(src, strongest, m_player, nullptr, armiesToSend);
					delete toPlay;
					return o;
				}

			}
			else
			{
				//do other order
				int i = 0;
				bool targetFound = false;
				defend.erase(defend.begin());
				Territory* target = NULL;
				for (Territory* atk : attack) {
					for (Territory* adj : strongest->getAdjList()) {
						if (adj == atk) {
							target = adj;
							targetFound = true;
							break;
						}
					}
					if (targetFound) {
						break;
					}
					i++;
				}
				if (target == NULL) {
					return NULL;
				}
				attack.erase(attack.begin() + i);

				int armies = strongest->getAvailableArmies();
				strongest->m_availableArmies = 0;
				return new AdvanceOrder(m_player, strongest, target, armies);
			}
		} 
		//defend
		Territory* strongest = defend.at(0);
		Territory* source = NULL;
		const Map* map = GameManager::getMap();

		std::vector<Territory*> accessList = map->getAccessList(strongest);
		for(Territory* t:accessList){
			if(t->getOwner() == m_player && t->getAvailableArmies() > 0){
				source = t;
				break;
			}
		}
		if(source==NULL){
			return NULL;
		}
		
		int armies = source->getAvailableArmies();
		source->m_availableArmies = 0;
		return new AdvanceOrder(m_player, source, strongest, armies);
	}

	void AggressivePlayerStrategy::generateTerritoryLists() {
		//remove everything from the lists
		m_toDef.clear();
		m_toAtk.clear();
		Territory* strongest = NULL;
		for(Territory* t: m_player->getTerritories()){
			if (strongest == NULL){
				strongest = t;
			}
			else if (t->getArmies() > strongest->getArmies()){
				strongest = t;
			}
		
			//use a macro to make code easier to change and more readable
			if(t->getArmies()>= STRONGEST_THRESHOLD){
				m_toDef.push_back(t);
			}
		}

		if(strongest->getArmies() < STRONGEST_THRESHOLD){
			m_toDef.push_back(strongest);
		}
		
		quickSortTerritories(m_toDef, 0, m_toDef.size());


		for (Territory* t: m_toDef){
			for (Territory* adj : t->getAdjList())
			{
				if (adj->getOwner() != m_player) {
					m_toAtk.push_back(adj);
				}
			}
		}
	}

	PlayerStrategy* AggressivePlayerStrategy::copy() const
	{
		return new AggressivePlayerStrategy(*this);
	}

	void AggressivePlayerStrategy::quickSortTerritories(vector<Territory*>& list, int start, int end) {
		if (end - start <= 1)
			return;
		else if (end - start == 2) {
			if (list[start]->getArmies() < list[end - 1]->getArmies()) {
				swapTerritories(list, start, end);
			}
			return;
		}
		int pivotIndex = (end - start) / 2 + start;
		Territory* pivot = list[pivotIndex];
		int wall = start;
		for (int i = start; i < end; i++) {
			if (list[i]->getArmies() > pivot->getArmies()) {
				swapTerritories(list, i, wall);
				wall++;
			}
		}
		quickSortTerritories(list, start, wall);
		if (wall == start)
			wall++;
		quickSortTerritories(list, wall, end);
	}

	void AggressivePlayerStrategy::swapTerritories(vector<Territory*>& list, int i, int j) {
		if (i == j)
		{
			return;
		}

		Territory* temp = list[i];
		list[i] = list[j];
		list[j] = temp;
	}

	bool AggressivePlayerStrategy::ShouldBlockade(Territory* target)
	{
		if (m_player->getTerritories().size() == 1)
		{
			return false;
		}

		std::vector<Territory*> accessList = GameManager::getMap()->getAccessList(target);

		if (accessList.size() == 0)
		{
			return false;
		}

		int heuristic = 0;

		for (Territory* t : accessList)
		{
			if (t->getOwner() == GameManager::getNeutralPlayer())
			{
				continue;
			}
			else if (t->getOwner() == target->getOwner())
			{
				heuristic -= t->getArmies();
			}
			else
			{
				heuristic += t->getArmies();
			}
		}

		return heuristic >= BLOCKADE_THRESHOLD;
	}

	Player* AggressivePlayerStrategy::GetNegotiatingTarget(Territory* t) const
	{
		std::vector<Territory*> accessList = GameManager::getMap()->getAccessList(t);

		if (accessList.size() == 0)
		{
			return nullptr;
		}

		const std::vector<Player*> activePlayers = GameManager::getActivePlayers();
		std::pair<int, Player*>* heuristicArr = new std::pair<int, Player*>[activePlayers.size() - 1];
		int index = 0;

		//calculate heuristic of each player
		for (Territory* curr : accessList)
		{
			if (curr->getOwner() == GameManager::getNeutralPlayer())
			{
				continue;
			}
			else if (curr->getOwner() != t->getOwner())
			{
				for (size_t i = 0; i < activePlayers.size(); i++)
				{
					if (curr->getOwner() == activePlayers[i])
					{
						bool foundPlayer = false;
						for (int j = 0; j < index; j++)
						{
							if (heuristicArr[j].second == curr->getOwner())
							{
								heuristicArr[j].first += curr->getArmies();
								foundPlayer = true;
								break;
							}
						}

						if (!foundPlayer)
						{
							heuristicArr[index] = std::pair<int, Player*>(curr->getArmies(), curr->getOwner());
							index++;
						}
					}
				}
			}
		}

		int targetIndex = 0;
		for (int i = 1; i < index; i++)
		{
			if (heuristicArr[targetIndex].first < heuristicArr[i].first)
			{
				targetIndex = i;
			}
		}

		Player* target = heuristicArr[targetIndex].second;
		delete[] heuristicArr;
		return target;
	}

	Territory* AggressivePlayerStrategy::GetSourceTerritory(Territory* target) const
	{
		for (Territory* t : m_player->getTerritories())
		{
			if (t->m_availableArmies > 0)
			{
				if (target == nullptr)
				{
					return t;
				}

				for (Territory* adj : t->getAdjList())
				{
					if (adj == target)
					{
						return t;
					}
				}
			}
		}

		return nullptr;
	}


	std::ostream& operator<<(std::ostream& stream, const AggressivePlayerStrategy&)
	{
		stream << "Aggressive Player Strategy";
		return stream;
	}


	// BenevolentPlayerStrategy ///////////////////////////////////////////////////////////////

	BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* player) : PlayerStrategy(player) { }

	BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other) : PlayerStrategy(other) { }

	BenevolentPlayerStrategy::~BenevolentPlayerStrategy() { }

	BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& other)
	{
		PlayerStrategy::operator=(other);
		return *this;
	}

	Order* BenevolentPlayerStrategy::issueOrder() {
		if(m_reinforcements > 0 )
			return issueDeployOrder();
		
		return defensivePlay();
		
	}

	
	
	Order* BenevolentPlayerStrategy::defensivePlay(){
		vector<Territory*>& toDfd=toDefend();
		if(toDfd.empty())
			return NULL;
		
		if(m_player->hasCardType(Card::Type::Reinforcement) && Random::GetFloat()<=0.6f){
			Hand* hand = m_player->getHand();
			Card* crd = nullptr;
			for(Card* card :*hand){
				if(card->getType()==Card::Type::Reinforcement){
					crd=card;
					break;
				}

			}
			hand->removeCardFromHand(crd);
			crd->play(NULL,NULL,m_player,NULL,0);
			return issueOrder();
		}
		else
		{
			return Advance();
		}
		
	}
	
	DeployOrder* BenevolentPlayerStrategy::issueDeployOrder()
	{
		std::vector<Territory*>& toDef = toDefend();
		Territory* target = toDef[0];

		toDef.erase(toDef.begin());
		int armies = m_reinforcements;
		m_reinforcements = 0;
		return new DeployOrder(m_player, target, armies);
	}

	AdvanceOrder* BenevolentPlayerStrategy::Advance(){
		vector<Territory*>& toDfd=toDefend();
		Territory* target = toDfd[0];
		Territory* source= getSourceTerritory(target);
		if(source==NULL)
			return NULL;
		
		int armies=source->getAvailableArmies()/2;
		if(armies<1)
			armies=1;

		source->m_availableArmies -= armies;
		return new AdvanceOrder(m_player,source,target,armies);
	}

	Territory* BenevolentPlayerStrategy::getSourceTerritory(Territory* target){
		Territory* source=nullptr;
		vector<Territory*> accesslist=GameManager::getMap()->getAccessList(target);
		for(Territory* t : accesslist){
			if (t->getOwner()==m_player && t->getAvailableArmies()!= 0){
				if(source==NULL)
					source=t;
				else if(source->getAvailableArmies() < t->getAvailableArmies())
				{
					source=t;
					
				}
			}
		}
		return source;

	}
	
	void BenevolentPlayerStrategy::generateTerritoryLists(){
		m_toDef.clear();
		vector<Territory*> owned=m_player->getTerritories();
		quickSortTerritories(owned,0,owned.size());
		m_toDef=owned;

	}

	void BenevolentPlayerStrategy::quickSortTerritories(vector<Territory*>& list,int start,int end){
		if(end-start <=1)
			return;
		else if(end-start ==2){
			if(list[start]->getArmies() > list[end-1]->getArmies()){
				swapTerritories(list,start,end);	
			}
			return;
		}
		int pivotIndex=(end - start)/2 + start;
		Territory* pivot=list[pivotIndex];
		int wall=start;
		for(int i=start ; i<end ; i++){
			if(list[i]->getArmies()< pivot->getArmies()){
				swapTerritories(list,i,wall);
				wall++;
			}
		}
		quickSortTerritories(list,start,wall);
		if(wall==start)
			wall++;
		quickSortTerritories(list,wall,end);


	}

	void BenevolentPlayerStrategy::swapTerritories(vector<Territory*>& list,int i,int j){
		if (i == j)
		{
			return;
		}

		Territory* temp=list[i];
		list[i]=list[j];
		list[j]=temp;
	}

	PlayerStrategy* BenevolentPlayerStrategy::copy() const
	{
		return new BenevolentPlayerStrategy(*this);
	}
	
	std::ostream& operator<<(std::ostream& stream, const BenevolentPlayerStrategy&)
	{
		stream << "Benevolant Player Strategy";
		return stream;
	}

}

