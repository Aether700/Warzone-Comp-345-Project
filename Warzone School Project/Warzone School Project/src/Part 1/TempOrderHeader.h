#pragma once
#include <vector>

namespace WZ
{
	class Player;

	//temp not real class
	class Territory
	{
		//need bool operator
		//need to be able to check owner (null if neutral?)
		//need to be able to set owner
		//need list of adjacent territories (getAdjacent() const)
		//need access to number of troups on territory (getTroups, setTroups)
	public:
		Territory(Player* owner = nullptr, unsigned int troups = 0) : m_owner(owner), m_troups(troups)
		{
		}

		Player* getOwner() { return m_owner; }
		const Player* getOwner() const { return m_owner; }

		void setOwner(Player* o) { m_owner = o; }

		unsigned int getTroups() const { return m_troups; }
		void setTroups(unsigned int amount) { m_troups = amount; }

		std::vector<Territory*> getAdjacent() const { return m_adjacent; }

		bool operator==(const Territory& other) const { return m_owner == other.m_owner; }
		
		//temp
		void setAdj(std::vector<Territory*> list) { m_adjacent = list; }
		std::string toString() const { return "Territory"; }

	private:
		Player* m_owner;
		std::vector<Territory*> m_adjacent;
		unsigned int m_troups;
	};

	//temp player not actual class
	class Player
	{
		//need bool operator player == player
		//hash map
		//ownsTerritory function
	public:
		Player() 
		{

		}

		std::string toString() const { return "Player"; }
		bool ownsTerritory(Territory* t) const
		{
			return t->getOwner() != nullptr && *t->getOwner() == *this;
		}

		std::vector<Territory*>::const_iterator begin() const { return m_owned.cbegin(); }
		std::vector<Territory*>::const_iterator end() const   { return m_owned.cend(); }

		std::vector<Player*> getNegotiatingPlayers() const
		{
			std::vector<Player*> n;
			return n;
		}

		//temp
		void setTerritories(std::vector<Territory*> list) { m_owned = list; }

		bool operator==(const Player& other) const { return this == &other; }
		bool operator!=(const Player& other) const { return !this->operator==(other); }

	private:
		std::vector<Territory*> m_owned;
	};
}