#include "Player.h"
#include "Map.h"

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
		for (int i = 0; i < territories.size(); i++)
		{
			if (territories[i]->getTerritoryName() == n)
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
		for (Territory* territory : *this)
		{
			if (*t == *territory)
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
		for (int i = 0; i < territories.size(); i++) {
			if (territories[i]->getTerritoryName() == oldTerritory->getTerritoryName()) {
				territories.erase(territories.begin() + i);
			}
		}
	}

	vector<Territory*> Player::toDefend(Player* p, Territory* t)
	{
		return territories;
	}

	vector<Territory*> Player::toAttack(Player* p, Territory* t)
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