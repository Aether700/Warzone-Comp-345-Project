#include "Player.h"
#include "Orders.h"
#include "Map.h"
#include "Cards.h"
#include "GameEngine.h"

namespace WZ
{
	using namespace std;

	Player::Player() : hasDrawnCard(false) {
		hand = new Hand();
		listOrders = new OrderList();
	}

	Player::Player(const Player& p) : hasDrawnCard(p.hasDrawnCard) {
		hand = new Hand(*p.hand);
		playerName = p.playerName;
		territories = territories;
		listOrders = new OrderList(*p.listOrders);
	}

	Player::~Player() {
		delete hand;
		delete listOrders;
	}

	Player::Player(string n, vector<Territory*> t) : hasDrawnCard(false) {
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

	void Player::issueOrder(int& reinforcements) {
		int playerChoice = 0, amount;						//	menu variable to enable the player to choose between options
		vector<Territory*> t = this->getTerritories();		//	territories the player holds
		while (reinforcements > 0) {						//	deploy reinforcements
			cout << reinforcements << " reinforcements available. Choose a location to deploy: ";
			for (size_t i = 0; i < this->toDefend().size(); i++) {			//	looping through the player's own territories
				cout << i << "\t" << t[i]->getName();
			}
			cin >> playerChoice;							//	player choose territory
			cout << "Number of troops to deploy: ";
			cin >> amount;									//	quantity to deploy
			DeployOrder* ord = new DeployOrder(this, t[playerChoice], amount);	//	create an order to deploy
			listOrders->addOrder(ord);						//	add the order
			delete ord;
		}
		cout << "What are your orders?\n";
		cout << "1\tPlay Card\n2\tAttack\n3\tDefend\n4\tBlockade" << endl;		//	player can choose the type of action to add to the queue orders
		cin >> playerChoice;
		while (playerChoice < 1 || playerChoice > 4) {
			cout << "Wrong input. Try again:" << endl;
			cin >> playerChoice;
		}

		switch (playerChoice) {
		case 1:
			Hand * playersHand = this->getHand();
			if (playersHand->getCount() < 1)
				cout << "No cards to play.";
			else {
				for (size_t i = 0; i < playersHand->getCount(); i++)
					cout << i << "\t" << playersHand[i];
				cout << "\nChoose the index of your card: ";
				cin >> playerChoice;
				while (playerChoice < 0 || playerChoice > playersHand->getCount()) {
					cout << "Invalid entry. Try again: " << endl;
					cin >> playerChoice;
				}
				if (*playersHand[playerChoice] == BombOrder)
			}
			break;

		case 2:
			cout << "Enter the territory index where the deploy should occure: " << endl;
			for (size_t i = 0; i < t->size(); i++)
				cout << i << "\t" << *(t + i).getName() << endl;
			cin >> playerChoice;
			currentPlayer.issueOrder();
			break;
		default:
			break;
		}
		currentPlayer.setOrderList(DeployOrder(&currentPlayer, ));
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