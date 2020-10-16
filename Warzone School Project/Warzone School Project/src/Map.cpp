#include "Map.h"
#include "Player.h"
#include "GameEngine.h"

#include <queue>
#include <sstream>
#include <unordered_map>
#include <assert.h>


#define ARMIES_SPACES 7

namespace WZ
{
	static std::string FitInTable(std::string_view str, unsigned int totalSpaces)
	{
		assert(str.size() <= totalSpaces);

		int spacesLeft = totalSpaces - str.size();

		int spacesBefore = spacesLeft / 2;
		int spacesAfter = spacesLeft - spacesBefore;
		std::stringstream ss;

		for (int i = 0; i < spacesBefore; i++)
		{
			ss << " ";
		}
		
		ss << str;

		for (int i = 0; i < spacesAfter; i++)
		{
			ss << " ";
		}

		return ss.str();
	}

	static std::string SetTableEntry(Territory* t, int* entrySpaces)
	{
		std::stringstream ss;
		std::stringstream adjStream;

		//"|  Territory\t|  Continent\t|  Owner\t|  Armies\t| Adjacent Territories\t|\n"
		ss << "|" << FitInTable(t->getName(), entrySpaces[0]) << "|" 
			<< FitInTable(t->getContinent()->getName(), entrySpaces[1]) << "|"
			<< FitInTable(t->getOwner()->getPlayerName(), entrySpaces[2]) << "|"
			<< FitInTable(std::to_string(t->getArmies()), ARMIES_SPACES) << "|";

		
		for (Territory* adj : t->getAdjList())
		{
			adjStream << adj->getName() << ", ";
		}

		std::string str = adjStream.str();
		if (t->getAdjList().size() != 0)
		{
			str.erase(str.size() - 2, 2);
		}

		ss << FitInTable(adjStream.str(), entrySpaces[3])<<"|\n";

		return ss.str();
	}

	// Territory //////////////////////////////////////////////

	Territory::Territory(const std::string& name, unsigned int id, Continent* continent, unsigned int armies)
		: m_name(name), m_id(id), m_armies(armies), m_continent(continent), 
		m_owner(GameManager::getNeutralPlayer()), m_visited(false) { }

	Territory::Territory(const Territory& other)
		: m_name(other.m_name), m_id(other.m_id), m_armies(other.m_armies),
		m_continent(other.m_continent), m_owner(other.m_owner), m_adjList(other.m_adjList), m_visited(false) { }

	const std::string& Territory::getName() const { return m_name; }
	
	unsigned int Territory::getID() const { return m_id; }
	
	Continent* Territory::getContinent() { return m_continent; }

	const Continent* Territory::getContinent() const { return m_continent; }

	const Player* Territory::getOwner() const { return m_owner; }

	Player* Territory::getOwner() { return m_owner; }

	unsigned int Territory::getArmies() const { return m_armies; }

	void Territory::setOwner(Player* p) { m_owner = p; }
	
	void Territory::setArmies(unsigned int amount) { m_armies = amount; }

	void Territory::clearAdjList() { m_adjList.clear(); }

	void Territory::addAdjTerritory(Territory* adj) { m_adjList.push_back(adj); }
	
	void Territory::removeAdjTerritory(Territory* adj) 
	{
		for (size_t i = 0; i < m_adjList.size(); i++)
		{
			if (*m_adjList[i] == *adj)
			{
				m_adjList.erase(m_adjList.begin() + i);
				break;
			}
		}
	}
	
	const std::vector<Territory*>& Territory::getAdjList() const { return m_adjList; }

	Territory& Territory::operator=(const Territory& other)
	{
		m_name = other.m_name;
		m_id = other.m_id;
		m_armies = other.m_armies;
		m_continent = other.m_continent;
		m_owner = other.m_owner;
		m_adjList = m_adjList;

		return *this;
	}

	bool Territory::operator==(const Territory& other) const 
	{
		return m_name == other.m_name && m_id == other.m_id;
	}

	bool Territory::operator!=(const Territory& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const Territory& t)
	{
		std::stringstream ss;
		ss << "\"" << t.getName() << "\"" << " Armies: " << t.getArmies() << " Owner: " << *t.getOwner();
		
		stream << ss.str();
		return stream;
	}

	void Territory::setContinent(Continent* continent) 
	{
		assert(continent != nullptr);
		m_continent = continent; 
	}


	// Continent //////////////////////////////////////////////
	
	Continent::Continent(const std::string& name, unsigned int bonus) 
		: m_name(name), m_bonus(bonus), m_visited(false) { }

	Continent::Continent(const Continent& other) 
		: m_name(other.m_name), m_bonus(other.m_bonus), 
		m_territories(other.m_territories), m_visited(false) { }

	Continent::~Continent()
	{
		for (Territory* t : m_territories)
		{
			delete t;
		}
	}

	const std::string& Continent::getName() const { return m_name; }
	
	unsigned int Continent::getBonus() const { return m_bonus; }
	
	void Continent::clearTerritories() 
	{ 
		for (Territory* t : m_territories)
		{
			delete t;
		}

		m_territories.clear(); 
	}

	Territory* Continent::getTerritory(size_t index) { return m_territories[index]; }

	const Territory* Continent::getTerritory(size_t index) const { return m_territories[index]; }
	
	size_t Continent::getCount() const { return m_territories.size(); }
	
	bool Continent::contains(Territory* t) const
	{
		for (Territory* currTerritory : m_territories)
		{
			if (*t == *currTerritory)
			{
				return true;
			}
		}
		return false;
	}


	bool Continent::validate() const 
	{
		if (m_territories.empty())
		{
			return false;
		}

		ResetVisitedTerritories();

		std::queue<Territory*> q;

		size_t firstTerrIndex = -1;

		//make sure first territory is not a trap territory
		for (size_t i = 0; i < m_territories.size(); i++)
		{
			if (m_territories[i]->getAdjList().size() != 0)
			{
				firstTerrIndex = i;
				break;
			}
		}

		//make sure that the continent is not made of territories without any neighbors
		if (firstTerrIndex == -1)
		{
			return false;
		}

		//enqueue the first territory selected
		q.push(m_territories[firstTerrIndex]);

		//visit all Territories linked to this territory
		while (!q.empty())
		{
			Territory* currTerr = q.front(); //take next territory
			q.pop(); //remove current territory
			currTerr->m_visited = true;

			for (Territory* t : currTerr->getAdjList())
			{
				if (!t->m_visited)
				{
					q.push(t);
				}
			}
		}

		//make sure that all territories have been visited. if they have not the graph is disconnected
		for(Territory* t : m_territories)
		{
			if (!t->m_visited)
			{
				return false;
			}
		}

		return true;
	}

	bool Continent::noSharedTerritories(const Continent* other) const
	{
		for (Territory* currTerritory : m_territories)
		{
			for(Territory* t : other->m_territories)
			{
				if (*currTerritory == *t)
				{
					return false;
				}
			}
		}
		return true;
	}

	void Continent::addTerritory(Territory* t)
	{
		m_territories.push_back(t);
	}
	
	void Continent::removeTerritory(Territory* t)
	{
		for (size_t i = 0; i < m_territories.size(); i++)
		{
			if (*m_territories[i] == *t)
			{
				m_territories.erase(m_territories.begin() + i);
				break;
			}
		}
	}

	Continent& Continent::operator=(const Continent& other)
	{
		m_name = other.m_name;
		m_bonus = other.m_bonus;
		m_territories = other.m_territories;

		return *this;
	}

	bool Continent::operator==(const Continent& other) const
	{
		return m_name == other.m_name && m_bonus == other.m_bonus;
	}

	bool Continent::operator!=(const Continent& other) const
	{
		return !(*this == other);
	}

	void Continent::ResetVisitedTerritories() const
	{
		for (Territory* t : m_territories)
		{
			t->m_visited = false;
		}
	}

	std::ostream& operator<<(std::ostream& stream, const Continent& c)
	{
		std::stringstream ss;

		ss << "\"" << c.getName() << "\"" << " Bonus: " << c.getBonus() << " Territories:\n";
		for (size_t i = 0; i < c.getCount(); i++)
		{
			ss << "\t[" << *c.getTerritory(i) << "],\n";
		}

		std::string str = ss.str();
		
		if (c.getCount() != 0)
		{
			str.erase(str.length() - 2, 2);
		}

		stream << str;
		return stream;
	}
	
	// Map ////////////////////////////////////////////////////

	Map::Map() { }

	Map::Map(std::vector<Continent*> continents) : m_continents(continents) 
	{
		SetTerritoryHash();
	}

	Map::Map(const Map& other)
	{
		m_continents.reserve(other.m_continents.size());
		CopyMap(other);
	}

	Map::~Map()
	{
		for (Continent* c : m_continents)
		{
			delete c;
		}
	}

	bool Map::validate() const
	{
		if (m_continents.size() == 0)
		{
			return false;
		}

		//make sure continents are valid connected subgraphs
		for (Continent* c : m_continents)
		{
			if (!c->validate())
			{
				return false;
			}
		}

		//make sure that no continent is isolated
		ResetVisitedContinents();

		std::queue<Continent*> q;
		q.push(m_continents[0]);
		
		//visit every continent that the first continent has access to
		while(!q.empty())
		{
			Continent* currContinent = q.front();
			q.pop();

			currContinent->m_visited = true;

			//check every territory in the continent
			for (size_t i = 0; i < currContinent->getCount(); i++)
			{
				//for every of those territories inside the continent check their adjList
				Territory* currTerritory = currContinent->getTerritory(i);

				for (Territory* t : currTerritory->getAdjList())
				{
					//for every territory in the adjList check if the continent 
					//contains that territory. if it doesn't then we enqueue 
					//the continent of that territory in the queue
					if (!currContinent->contains(t))
					{
						Continent* c = t->getContinent();
						if (!c->m_visited)
						{
							q.push(c);
						}
					}
				}
			}
		}

		//make sure every continent has been visited
		for (Continent* c : m_continents)
		{
			if (!c->m_visited)
			{
				return false;
			}
		}

		//make sure every territory is owned by one and only one continent

		for (size_t i = 0; i < m_continents.size(); i++)
		{
			for (size_t j = m_continents.size() - 1; j > i; j--)
			{
				if (!m_continents[i]->noSharedTerritories(m_continents[j]))
				{
					return false;
				}
			}
		}


		return true;
	}

	Continent* Map::getContinent(size_t index) { return m_continents[index]; }
	
	const Continent* Map::getContinent(size_t index) const { return m_continents[index]; }

	size_t Map::getCount() const { return m_continents.size(); }
	
	void Map::addContinent(Continent* c) 
	{ 
		m_continents.push_back(c); 
		for (size_t i = 0; i < c->getCount(); i++)
		{
			Territory* curr = c->getTerritory(i);
			m_territoryHash[curr->getID()] = curr;
		}
	}
	
	void Map::removeContinent(Continent* c)
	{
		for (size_t i = 0; i < m_continents.size(); i++)
		{
			if (*m_continents[i] == *c)
			{
				for (auto it = m_territoryHash.begin(); it != m_territoryHash.end(); it++)
				{
					if ((*it).second->getContinent() == m_continents[i])
					{
						m_territoryHash.erase(it);
					}
				}
				m_continents.erase(m_continents.begin() + i);
				break;
			}
		}
	}

	std::unordered_map<unsigned int, Territory*>::iterator Map::begin() { return m_territoryHash.begin(); }

	std::unordered_map<unsigned int, Territory*>::iterator Map::end() { return m_territoryHash.end(); }

	std::unordered_map<unsigned int, Territory*>::const_iterator Map::begin() const { return m_territoryHash.cbegin(); }

	std::unordered_map<unsigned int, Territory*>::const_iterator Map::end() const { return m_territoryHash.cend(); }

	Map& Map::operator=(const Map& other)
	{
		for (Continent* c : m_continents)
		{
			delete c;
		}

		CopyMap(other);

		return *this;
	}

	void Map::CopyMap(const Map& other)
	{
		std::unordered_map<unsigned int, Territory*> copiedTerritories;

		for (Continent* c : other.m_continents)
		{
			Continent* copy = new Continent(*c);

			for (size_t i = 0; i < copy->getCount(); i++)
			{
				unsigned int id = copy->getTerritory(i)->getID();
				copiedTerritories[id] = new Territory(*copy->getTerritory(i));
				copiedTerritories[id]->setContinent(copy);
			}

			copy->clearTerritories();
			m_continents.push_back(copy);
		}

		for (std::pair<const unsigned int, Territory*> pair : copiedTerritories)
		{
			Territory* currTerritory = pair.second;
			currTerritory->clearAdjList();

			for (Territory* adj : currTerritory->getAdjList())
			{
				currTerritory->addAdjTerritory(adj);
			}

			currTerritory->getContinent()->addTerritory(currTerritory);
		}

		m_territoryHash.clear();
		SetTerritoryHash();
	}

	void Map::ResetVisitedContinents() const
	{
		for (Continent* c : m_continents)
		{
			c->m_visited = false;
		}
	}

	void Map::SetTerritoryHash()
	{
		for (Continent* c : m_continents)
		{
			for (size_t i = 0; i < c->getCount(); i++)
			{
				Territory* currTerritory = c->getTerritory(i);
				m_territoryHash[currTerritory->getID()] = currTerritory;
			}
		}
	}
	
	std::ostream& operator<<(std::ostream& stream, const Map& m)
	{
		std::stringstream ss;

		int entrySpaces[4] = { 9, 9, 5, 20 };

		//check max entry length for each column
		for (std::pair<unsigned int, Territory*> pair : m)
		{
			Territory* curr = pair.second;
			if (entrySpaces[0] < curr->getName().size())
			{
				entrySpaces[0] = curr->getName().size();
			}

			if (entrySpaces[1] < curr->getContinent()->getName().size())
			{
				entrySpaces[1] = curr->getContinent()->getName().size();
			}

			if (entrySpaces[2] < curr->getOwner()->getPlayerName().size())
			{
				entrySpaces[2] = curr->getOwner()->getPlayerName().size();
			}

			int adjSize = 0;
			for (Territory* t : curr->getAdjList())
			{
				adjSize += t->getName().size() + 2;
			}

			if (curr->getAdjList().size() != 0)
			{
				adjSize -= 2;
			}

			if (entrySpaces[3] < adjSize)
			{
				entrySpaces[3] = adjSize;
			}
		}

		int totalSpace = 0;

		for (int i = 0; i < 4; i++)
		{
			entrySpaces[i] += 2;
			totalSpace += entrySpaces[i];
		}

		totalSpace += 5 + ARMIES_SPACES;

		//header of table
		ss << "|" << FitInTable("Territory", entrySpaces[0]) << "|" << FitInTable("Continent", entrySpaces[1]) 
			<< "|" << FitInTable("Owner", entrySpaces[2]) 
			<< "|" << FitInTable("Armies", ARMIES_SPACES) 
			<< "|" << FitInTable("Adjacent Territories", entrySpaces[3]) << "|\n";

		for (int i = 0; i < totalSpace; i++)
		{
			ss << "-";
		}
		ss << "\n";
		
		//territory entries
		for (std::pair<unsigned int, Territory*> pair : m)
		{
			Territory* curr = pair.second;
			ss << SetTableEntry(curr, entrySpaces);
		}

		//end of table
		for (int i = 0; i < totalSpace; i++)
		{
			ss << "-";
		}
		ss << "\n";

		stream << ss.str();
		return stream;
	}


}