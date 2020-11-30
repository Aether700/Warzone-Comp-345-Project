#include <assert.h>
#include <array>
#include <sstream>
#include <random>
#include <chrono>

#include "PlayerStrategies.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include "Utils.h"

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
		if(m_reinforcements > 0 && defend.size() > 0){
			Territory* territory = toDefend().at(0);


		}
	}

	std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
	
	}

	std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine default_random_engine(seed);
		std::vector<Territory*> territories = m_player->getTerritories();
		std::stable_sort(territories.begin(), territories.end());
		return territories;
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
			Card* crd;
			for(Card* card :*hand){
				if(card->getType()==Card::Type::Reinforcement){
					crd=card;
					break;
				}

			}
			hand->removeCardFromHand(crd);
			crd->play(NULL,NULL,NULL,NULL,0);
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
		m_reinforcements = 0;
		return new DeployOrder(m_player, target, m_reinforcements);
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