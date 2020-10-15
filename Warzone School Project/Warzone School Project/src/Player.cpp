#include "Player.h"
#include "Orders.h"
#include "Map.h"
#include "Cards.h"

namespace WZ
{
	using namespace std;

	Player::Player() {
		hand = new Hand();
		listOrders = new OrderList();
	}

	Player::Player(const Player& p) {
		hand = new Hand(*p.hand);
		playerName = p.playerName;
		territories = territories;
		listOrders = new OrderList(*p.listOrders);
	}

	Player::~Player() {
		delete hand;
		delete listOrders;
	}

	Player::Player(string n, vector<Territory*> t) {
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

	Hand* Player::getHand()
	{
		return hand;
	}

	string Player::getPlayerName() const {
		return playerName;
	}

	//for not just returns an empty vector<Player*>
	std::vector<Player*> Player::getNegotiatingPlayers() const
	{
		std::vector<Player*> n;
		return n;
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
	}

	void Player::removeTerritory(Territory* oldTerritory) {
		for (size_t i = 0; i < territories.size(); i++) {
			if (territories[i]->getName() == oldTerritory->getName()) {
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

	void Player::issueOrder(Order* order)
	{
		listOrders->addOrder(order);
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
}