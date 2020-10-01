#include "Player.h"
#include "Map.h"

namespace WZ
{
	Player::Player() {
		hand = new Hand();
	}

	Player::Player(string n, vector<Territory*> t) {
		playerName = n;
		territories = t;
		hand = new Hand();
	}

	vector<Territory*> Player::getTerritorries() const{
		return territorries;
	}

	Territory* Player::getTerritory(string n) {
		for (int i = 0; i < territorries.size(); i++)
		{
			if (territorries[i]->getTerritoryName() == n)
			{
				return territorries[i];
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

	void Player::setPlayerName(string n) {
		playerName = n;
	}

	void Player::setHand() {
		Hand h;
		hand = &h;
	}

	void Player::setTerritorries() {
		Territory t;
		territories = &t;
	}

	void Player::addTerritory(Territory* newTerritory) {
		territorries.push_back(newTerritory);
	}

	void Player::removeTerritory(Territory* oldTerritory) {
		for (int i = 0; i < territorries.size(); i++) {
			if (territorries[i]->getTerritoryName() == oldTerritory->getTerritoryName()) {
				territorries.erase(territorries.begin() + i);
			}
		}
	}

	void Player::toDefend(Player* p, Territory* t)
	{

	}

	void Player::toAttack()(Player* p, Territory* t)
	{

	}

	void Player::issueOrder()
	{

	}
}