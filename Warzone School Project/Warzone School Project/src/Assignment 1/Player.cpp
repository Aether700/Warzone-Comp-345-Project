#include "Player.h"

Player::Player() {
	hand = new Hand();
}

Player::Player(string n, vector<Territory*> t) {
	playerName = n;
	hand = new Hand();
	territories = t;
}

string Player::getName() {
	return playerName;
}

void Player::setName(string n) {
	playerName = n;
}

vector<Territory*> Player::getTerritorries() {
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

Hand * Player::getHand()
{
	return hand;
}

void Player::setHand(){
    Hand h;
    hand = &h;
}

void Player::addTerritory(Territory* newTerritory) {
	territorries.push_back(newTerritory);
}

void Player::removeTerritory(Territory* oldTerritory) {
	for (int i = 0; i < territorries.size(); i++) {
		if (territorries[i]->getTerritoryName() == oldTerritory->getTerritoryName()) {
			territorries.erase(territorries.begin()+i);
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
