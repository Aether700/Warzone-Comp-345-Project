#include "Player.h"
#include "Orders.h"
#include "Map.h"
#include "GameEngine.h"
#include <assert.h>

namespace WZ
{
	using namespace std;

	Player::Player() : hasDrawnCard(false), m_strategy(nullptr) {
		hand = new Hand();
		listOrders = new OrderList();
	}

	Player::Player(const Player& p) : hasDrawnCard(p.hasDrawnCard), m_strategy(p.m_strategy->copy()) {
		hand = new Hand(*p.hand);
		playerName = p.playerName;
		territories = territories;
		listOrders = new OrderList(*p.listOrders);
	}

	Player::~Player() {
		delete hand;
		delete listOrders;
		delete m_strategy;
	}

	Player::Player(string n, PlayerStrategy* strategy, vector<Territory*> t) : hasDrawnCard(false), m_strategy(strategy) {
		playerName = n;
		territories = t;
		hand = new Hand();
		listOrders = new OrderList();

		if (m_strategy == nullptr)
		{
			m_strategy = GetRandomStrategy();
		}
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

	void Player::SetStrategy(PlayerStrategy* strategy)
	{
		assert(strategy != nullptr);

		delete m_strategy;
		m_strategy = strategy;
		m_strategy->SetPlayer(this);
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

	void Player::generateTerritoryLists()
	{
		m_strategy->generateTerritoryLists();
	}

	std::vector<Territory*> Player::toDefend() { return m_strategy->toDefend(); }

	std::vector<Territory*> Player::toAttack() { return m_strategy->toAttack(); }

	//returns null if done playing for this turn
	Order* Player::issueOrder()
	{
		return m_strategy->issueOrder();
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

	Order* Player::executeTopOrder() {
		if (listOrders->getCount() == 0)
			return nullptr;
		Order* o = listOrders->dequeueOrder();

		o->execute();
		return o;
	}

	PlayerStrategy* Player::GetRandomStrategy()
	{
		if (Random::GetFloat() <= 0.5f)
		{
			return new BenevolentPlayerStrategy(this);
		}
		return new AggressivePlayerStrategy(this);
	}
}