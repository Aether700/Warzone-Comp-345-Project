#include "Player.h"
#include "Orders.h"
#include "Map.h"
#include "Cards.h"
#include "GameEngine.h"

#define OFFENSIVE_PLAY 0.4f
#define CARD_PLAY 0.6f
#define BLOCKADE_THRESHOLD 15
#define DEF_THRESHOLD 10

namespace WZ
{
	using namespace std;

	Player::Player() : hasDrawnCard(false), m_reinforcements(0) {
		hand = new Hand();
		listOrders = new OrderList();
	}

	Player::Player(const Player& p) : hasDrawnCard(p.hasDrawnCard), m_reinforcements(0) {
		hand = new Hand(*p.hand);
		playerName = p.playerName;
		territories = territories;
		listOrders = new OrderList(*p.listOrders);
	}

	Player::~Player() {
		delete hand;
		delete listOrders;
	}

	Player::Player(string n, vector<Territory*> t) : hasDrawnCard(false), m_reinforcements(0) {
		playerName = n;
		territories = t;
		hand = new Hand();
		listOrders = new OrderList();
	}

	vector<Territory*> Player::getTerritories() const {
		return territories;
	}

	Territory* Player::getTerritory(string n) {
		for (size_t i = 0; i < territories.size(); i++)
		{
			if (territories[i]->getName() == n)
			{
				return territories[i];
			}
		}
		return NULL;
	}

	size_t Player::getNumOfTerritories() const { return territories.size(); }

	Hand* Player::getHand()
	{
		return hand;
	}

	string Player::getPlayerName() const {
		return playerName;
	}

	const OrderList* Player::getOrderList() const { return listOrders; }

	std::vector<Territory*>::iterator Player::begin() {
		return territories.begin();
	}

	std::vector<Territory*>::iterator Player::end() {
		return territories.end();
	}

	std::vector<Territory*>::const_iterator Player::begin() const {
		return territories.cbegin();
	}

	std::vector<Territory*>::const_iterator Player::end() const {
		return territories.cend();
	}

	bool Player::ownsTerritory(Territory* t) const
	{
		for (std::vector<Territory*>::const_iterator it = this->begin(); it != this->end(); it++)
		{
			if (*t == *(*it))
			{
				return true;
			}
		}
		return false;
	}

	void Player::setPlayerName(string n) {
		playerName = n;
	}

	void Player::setHand() {
		Hand h;
		hand = &h;
	}

	void Player::addTerritory(Territory* newTerritory) {
		territories.push_back(newTerritory);
		newTerritory->setOwner(this);
	}

	void Player::removeTerritory(Territory* oldTerritory) {
		for (size_t i = 0; i < territories.size(); i++) {
			if (territories[i]->getName() == oldTerritory->getName()) {
				territories[i]->setOwner(GameManager::getNeutralPlayer());
				territories.erase(territories.begin() + i);
			}
		}
	}

	vector<Territory*> Player::toDefend()
	{
		return territories;
	}

	vector<Territory*> Player::toAttack()
	{
		return territories;
	}

	//returns null if done playing for this turn
	Order* Player::issueOrder() {
		if (m_reinforcements != 0)
		{
			std::vector<Territory*> def = toDefend();
			Territory* destination = def[0];
			def.erase(def.begin());
			m_toDef.push_back(destination);
			unsigned int amount = m_reinforcements / def.size();

			if (amount == 0)
			{
				amount = 1;
			}

			m_reinforcements -= amount;

			DeployOrder* deploy = new DeployOrder(this, destination, amount);
			return deploy;
		}
		else
		{
			float randVal = OFFENSIVE_PLAY;

			if (!m_toDef.empty())
			{
				randVal = Random::GetFloat();
			}

			if (!m_toAtk.empty() && randVal <= OFFENSIVE_PLAY)
			{
				//do offensive play
				std::vector<Territory*> atk = toAttack();
				Territory* target = atk[0];
				atk.erase(atk.begin());

				Territory* src = nullptr;

				if (hasCardType(Card::Type::Airlift))
				{
					src = GetSourceTerritory(target);
				}

				if (src != nullptr || hasCardType(Card::Type::Bomb) && Random::GetFloat() <= CARD_PLAY)
				{
					Card::Type typeToPlay;

					if (src == nullptr)
					{
						typeToPlay = Card::Type::Bomb;
					}
					else if (!hasCardType(Card::Type::Bomb))
					{
						typeToPlay = Card::Type::Airlift;
					}
					else
					{
						//if we have both cards pick one card randomly
						if (Random::GetFloat() <= 0.5f)
						{
							typeToPlay = Card::Type::Bomb;
						}
						else
						{
							typeToPlay = Card::Type::Airlift;
						}
					}

					Card* toPlay = GetCardOfType(typeToPlay);

					if (typeToPlay == Card::Type::Bomb)
					{
						return toPlay->play(nullptr, target, this, nullptr, 0);
					}
					else
					{
						unsigned int armiesToSend = GetNumArmiesToSend(src);
						return toPlay->play(src, target, this, nullptr, armiesToSend);
					}
				}
				else
				{
					//do other order (here only advance)
					if (src == nullptr)
					{
						return nullptr;
					}

					unsigned int armiesToSend = GetNumArmiesToSend(src);

					return new AdvanceOrder(this, src, target, armiesToSend);
				}
			}
			else
			{
				if (m_toDef.empty())
				{
					return nullptr;
				}

				//do defensive play
				std::vector<Territory*> def = toDefend();
				Territory* target = def[0];
				def.erase(def.begin());

				Player* playerTarget = nullptr;
				if (hasCardType(Card::Type::Diplomacy))
				{
					playerTarget = GetNegotiatingTarget(target);
				}

				Territory* src = nullptr;

				if (hasCardType(Card::Type::Airlift))
				{
					src = GetSourceTerritory(target);
				}

				if (src != nullptr || playerTarget != nullptr && Random::GetFloat() <= CARD_PLAY)
				{
					//play card
					Card::Type typeToPlay;
					if (src == nullptr)
					{
						typeToPlay = Card::Type::Diplomacy;
					}
					else if (playerTarget)
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

					Card* toPlay = GetCardOfType(typeToPlay);

					if (typeToPlay == Card::Type::Diplomacy)
					{
						return toPlay->play(nullptr, nullptr, this, playerTarget, 0);
					}
					else
					{
						unsigned int armiesToSend = GetNumArmiesToSend(src);

						return toPlay->play(src, target, this, nullptr, armiesToSend);
					}

				}
				else
				{
					//do other order

					//check if we blockade
					if (ShouldBlockade(target))
					{
						return new BlockadeOrder(this, target);
					}

					//otherwise we try to advance
					if (src == nullptr)
					{
						return nullptr;
					}

					unsigned int armiesToSend = GetNumArmiesToSend(src);

					return new AdvanceOrder(this, src, target, armiesToSend);
				}
			}
		}
	}

	bool Player::hasCardType(Card::Type type) const
	{
		for (Card* c : *hand)
		{
			if (c->getType() == type)
			{
				return true;
			}
		}

		return false;
	}

	Player& Player::operator=(const Player& p) {
		delete hand;
		delete listOrders;
		hand = new Hand(*p.hand);
		playerName = p.playerName;
		territories = territories;
		listOrders = new OrderList(*p.listOrders);
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const Player& p) {
		stream << p.getPlayerName();
		return stream;
	}

	//might have to modify later
	bool Player::operator==(const Player& other) const
	{
		return playerName == other.playerName && listOrders == other.listOrders;
	}

	bool Player::operator!=(const Player& other) const
	{
		return !(*this == other);
	}

	Territory* Player::GetSourceTerritory(Territory* target) const
	{
		for (Territory* t : territories)
		{
			if (t->m_availableArmies > 0 && !IsToDefend(t))
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

	bool Player::IsToDefend(Territory* t) const
	{
		for (Territory* curr : m_toDef)
		{
			if (curr == t)
			{
				return true;
			}
		}
		return false;
	}

	unsigned int Player::GetNumArmiesToSend(Territory* t)
	{
		if (t->m_availableArmies == 0)
		{
			return 0;
		}

		unsigned int armiesToSend = (unsigned int)(Random::GetFloat() * t->m_availableArmies) + 1;
		armiesToSend = Clamp(1, t->m_availableArmies, armiesToSend);
		t->m_availableArmies -= armiesToSend;
		return armiesToSend;
	}

	Card* Player::GetCardOfType(Card::Type type)
	{
		for (Card* c : *hand)
		{
			if (c->getType() == type)
			{
				return c;
			}
		}

		return nullptr;
	}

	Player* Player::GetNegotiatingTarget(Territory* t) const
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
			if (curr->getOwner() != t->getOwner())
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

	bool Player::ShouldBlockade(Territory* target) const
	{
		std::vector<Territory*> accessList = GameManager::getMap()->getAccessList(target);

		if (accessList.size() == 0)
		{
			return false;
		}

		int heuristic = 0;

		for (Territory* t : accessList)
		{
			if (t->getOwner() == target->getOwner())
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

	void Player::GenerateToTerritoryLists()
	{
		GenerateToAtk();
		GenerateToDef();
	}

	void Player::GenerateToDef()
	{
		m_toDef.clear();

		for (Territory* owned : territories)
		{
			std::vector<Territory*> accessList = GameManager::getMap()->getAccessList(owned);

			int heuristic = 0;

			for (Territory* adj : accessList)
			{
				if (adj->getOwner() != this)
				{
					heuristic += adj->getArmies();
				}
			}

			if (heuristic >= DEF_THRESHOLD)
			{
				m_toDef.push_back(owned);
			}
		}
	}

	void Player::GenerateToAtk()
	{
		m_toAtk.clear();

		for (Territory* owned : territories)
		{
			std::vector<Territory*> accessList = GameManager::getMap()->getAccessList(owned);

			for (Territory* adj : accessList)
			{
				if (adj->getOwner() != this)
				{
					m_toAtk.push_back(adj);
				}
			}
		}
	}
}