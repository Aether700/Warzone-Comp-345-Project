#include "Orders.h"
#include "Utils.h"
#include "Map.h"
#include "Player.h"
#include "GameEngine.h"

#include <sstream>
#include <assert.h>

#define DEF_WIN_RATE 0.7f
#define ATK_WIN_RATE 0.6f

namespace WZ
{
	static bool HasTerritoryAdj(const Player* p, const Territory* t)
	{
		for (Territory* playerTerritory : *p)
		{
			for(Territory* adjacentTerritory : playerTerritory->getAdjList())
			{
				if (*adjacentTerritory == *t)
				{
					return true;
				}
			}
		}
		return false;
	}
	
	static bool Attack(Territory* source, Territory* target, unsigned int amount)
	{
		assert(amount != 0);
		assert(source != nullptr);
		assert(target != nullptr);

		unsigned int initialAmount = amount;

		unsigned int atkCasualties = 0;
		unsigned int defCasualties = 0;

		//atk casualties (defenders killing attackers)
		for (size_t i = 0; i < target->getArmies() && amount > atkCasualties; i++)
		{
			if (Random::GetFloat() >= DEF_WIN_RATE)
			{
				atkCasualties++;
			}
		}


		//def casualties (attackers killing defenders)
		for (size_t i = 0; i < amount && target->getArmies() > defCasualties; i++)
		{
			if (Random::GetFloat() >= ATK_WIN_RATE)
			{
				defCasualties++;
			}
		}

		//apply casualties to defenders
		target->setArmies(target->getArmies() - defCasualties);
		amount -= atkCasualties;


		if (target->getArmies() == 0)
		{
			target->setOwner(source->getOwner());
			target->setArmies(amount);
			source->setArmies(source->getArmies() - initialAmount);

			GameManager::drawCard(source->getOwner());
			return true;
		}

		source->setArmies(source->getArmies() - initialAmount + amount);

		return false;
	}

	static void Move(Territory* source, Territory* target, unsigned int amount)
	{
		target->setArmies(target->getArmies() + amount);
		source->setArmies(source->getArmies() - amount);
	}

	
	// Order /////////////////////////////////////////////////////////////////////////

	Order::Order(Player* p) : m_player(p), m_isExecuted(false) 
	{
		assert(p != nullptr);
	}

	Order::Order(const Order& other) 
		: m_player(other.m_player), m_isExecuted(other.m_isExecuted) { }

	Order::~Order() { }

	const Player* Order::getPlayer() const { return m_player; }
	
	bool Order::isExecuted() const { return m_isExecuted; }

	Order& Order::operator=(const Order& other) 
	{ 
		this->m_player = other.m_player; 
		return *this;
	}

	bool Order::operator==(const Order& other) const
	{
		return *m_player == *other.m_player && m_isExecuted == other.m_isExecuted;
	}

	bool Order::operator!=(const Order& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const Order& o)
	{
		stream << o.toString();
		return stream;
	}

	// DeployOrder /////////////////////////////////////////////////////////////////////////
	
	DeployOrder::DeployOrder(Player* p, Territory* dest, unsigned int amount) 
		: Order(p), m_destination(dest), m_amount(amount)
	{
		assert(dest != nullptr);
	}

	DeployOrder::DeployOrder(const DeployOrder& other) 
		: Order(other), m_destination(other.m_destination), m_amount(other.m_amount)
	{

	}

	bool DeployOrder::validate() const
	{
		std::cout << "Validating DeployOrder\n";
		return getPlayer()->ownsTerritory(m_destination);
	}
	
	void DeployOrder::execute()
	{
		if (validate())
		{
			m_destination->setArmies(m_destination->getArmies() + m_amount);
			m_isExecuted = true;
		}
	}

	std::string DeployOrder::toString() const
	{
		std::stringstream ss;
		ss << *getPlayer() << ": ";

		if (isExecuted())
		{
			ss << "has deployed " << m_amount << " troups to " << *m_destination 
				<< " (current troups: " << m_destination->getArmies() << ")";
		}
		else
		{
			ss << "deploy " << m_amount << " troups to " << *m_destination;
		}
		return ss.str();
	}

	DeployOrder& DeployOrder::operator=(const DeployOrder& other)
	{
		Order::operator=(other);
		m_destination = other.m_destination;
		m_amount = other.m_amount;

		return *this;
	}

	bool DeployOrder::operator==(const Order& other) const
	{
		if (typeid(*this) != typeid(other))
		{
			return false;
		}

		DeployOrder& deploy = (DeployOrder&) other;

		return Order::operator==(other) && m_amount == deploy.m_amount && *m_destination == *deploy.m_destination;
	}


	// AdvanceOrder /////////////////////////////////////////////////////////////////////////

	AdvanceOrder::AdvanceOrder(Player* p, Territory* source, Territory* target, unsigned int amount) 
		: Order(p), m_source(source), m_target(target), m_amount(amount), m_outcome(Outcome::NotExecuted)
	{
		assert(source != nullptr);
		assert(target != nullptr);
	}

	AdvanceOrder::AdvanceOrder(const AdvanceOrder& other)
		: Order(other), m_source(other.m_source), m_target(other.m_target), 
		m_amount(other.m_amount), m_outcome(other.m_outcome)
	{

	}

	bool AdvanceOrder::validate() const
	{
		std::cout << "Validating AdvanceOrder\n";
		if (!getPlayer()->ownsTerritory(m_source) || GameManager::isNegotiating(getPlayer(), (const Player*) m_target->getOwner()))
		{
			return false;
		}

		std::vector<Territory*> adjacent = m_source->getAdjList();

		for (Territory* t : adjacent)
		{
			if (*t == *m_target)
			{
				return m_source->getArmies() >= m_amount;
			}
		}
		return false;
	}

	void AdvanceOrder::execute()
	{
		if (validate())
		{
			if (getPlayer()->ownsTerritory(m_target))
			{
				Move(m_source, m_target, m_amount);
				m_outcome = Outcome::Move;
			}
			else
			{
				if (Attack(m_source, m_target, m_amount))
				{
					m_outcome = Outcome::AttackWin;
				}
				else
				{
					m_outcome = Outcome::AttackLost;
				}
			}

			m_isExecuted = true;
		}
	}

	std::string AdvanceOrder::toString() const
	{
		std::stringstream ss;
		ss << getPlayer()->getPlayerName() << ": ";

		switch (m_outcome)
		{
		case Outcome::NotExecuted:
			ss << "Advance " << m_amount << " troups from " << m_source->getName() << " to " << m_target->getName();
			break;

		case Outcome::Move:
			ss << "Moved " << m_amount << " troups from " << m_source->getName() << " to " << m_target->getName();
			break;

		case Outcome::AttackLost:
			ss << m_source->getName() << " failed to conquer " << m_target->getName() << " with " << m_amount << " troups";
			break;

		case Outcome::AttackWin:
			ss << m_source->getName() << " conquered " << m_target->getName() << " with " << m_amount << " troups";
			break;
		}

		return ss.str();
	}

	AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& other)
	{
		Order::operator=(other);

		m_source = other.m_source;
		m_target = other.m_target;
		m_amount = other.m_amount;
		m_outcome = other.m_outcome;

		return *this;
	}

	bool AdvanceOrder::operator==(const Order& other) const
	{
		if (typeid(*this) != typeid(other))
		{
			return false;
		}

		AdvanceOrder& advance = (AdvanceOrder&)other;

		return Order::operator==(other) && m_amount == advance.m_amount && 
			*m_target == *advance.m_target;
	}

	// BombOrder /////////////////////////////////////////////////////////////////////////

	BombOrder::BombOrder(Player* p, Territory* target) : Order(p), m_target(target)	
	{
		assert(target != nullptr);
	}

	BombOrder::BombOrder(const BombOrder& other) : Order(other), m_target(other.m_target) { }

	bool BombOrder::validate() const
	{
		std::cout << "Validating BombOrder\n";
		if (getPlayer()->ownsTerritory(m_target))
		{
			return false;
		}

		return HasTerritoryAdj(getPlayer(), m_target);
	}

	void BombOrder::execute()
	{
		if (validate())
		{
			m_target->setArmies(m_target->getArmies() / 2);
			m_isExecuted = true;
		}
	}

	std::string BombOrder::toString() const
	{
		std::stringstream ss;

		ss << *getPlayer() << ": ";

		if (isExecuted())
		{
			ss << "has bombed ";
		}
		else
		{
			ss << "bomb ";
		}
		ss << *m_target;

		return ss.str();
	}

	BombOrder& BombOrder::operator=(const BombOrder& other)
	{
		Order::operator=(other);
		m_target = other.m_target;

		return *this;
	}

	bool BombOrder::operator==(const Order& other) const
	{
		if (typeid(*this) != typeid(other))
		{
			return false;
		}

		BombOrder& bomb = (BombOrder&)other;

		return Order::operator==(other) && *m_target == *bomb.m_target;
	}

	// BlockadeOrder /////////////////////////////////////////////////////////////////////////

	BlockadeOrder::BlockadeOrder(Player* p, Territory* target) 
		: Order(p), m_target(target) 
	{
		assert(target != nullptr);
	}
	
	BlockadeOrder::BlockadeOrder(const BlockadeOrder& other) 
		: Order(other), m_target(other.m_target) { }

	bool BlockadeOrder::validate() const
	{	
		std::cout << "Validating BlockadeOrder\n";
		return getPlayer()->ownsTerritory(m_target);
	}

	void BlockadeOrder::execute()
	{
		if (validate())
		{
			m_target->setArmies(m_target->getArmies() * 2);
			m_target->setOwner(GameManager::getNeutralPlayer());
			m_isExecuted = true;
		}
	}

	std::string BlockadeOrder::toString() const
	{
		std::stringstream ss;

		ss << *getPlayer() << ": ";

		if (isExecuted())
		{
			ss << "has blockaded ";
		}
		else
		{
			ss << "blockade ";
		}

		ss << *m_target;

		return ss.str();
	}

	BlockadeOrder& BlockadeOrder::operator=(const BlockadeOrder& other)
	{
		Order::operator=(other);
		m_target = other.m_target;

		return *this;
	}

	bool BlockadeOrder::operator==(const Order& other) const
	{
		if (typeid(*this) != typeid(other))
		{
			return false;
		}

		BlockadeOrder& block = (BlockadeOrder&)other;

		return Order::operator==(other) && *m_target == *block.m_target;
	}


	// AirliftOrder /////////////////////////////////////////////////////////////////////////

	AirliftOrder::AirliftOrder(Player* p, Territory* source, Territory* destination, unsigned int amount) 
		: Order(p), m_source(source), m_destination(destination), m_amount(amount), m_outcome(Outcome::NotExecuted)
	{
		assert(source != nullptr);
		assert(destination != nullptr);
	}

	AirliftOrder::AirliftOrder(const AirliftOrder& other) 
		: Order(other), m_source(other.m_source), m_destination(other.m_destination),
		m_amount(other.m_amount), m_outcome(other.m_outcome) { }

	bool AirliftOrder::validate() const
	{
		std::cout << "Validating AirliftOrder\n";
		return getPlayer()->ownsTerritory(m_source) && m_source->getArmies() >= m_amount 
			&& !GameManager::isNegotiating(getPlayer(), m_destination->getOwner());
	}

	void AirliftOrder::execute()
	{
		if (validate())
		{
			if (getPlayer()->ownsTerritory(m_destination))
			{
				Move(m_source, m_destination, m_amount);
				m_outcome = Outcome::Move;
			}
			else
			{
				if (Attack(m_source, m_destination, m_amount))
				{
					m_outcome = Outcome::AttackWin;
					return;
				}
				m_outcome = Outcome::AttackLost;
			}
		}
	}

	std::string AirliftOrder::toString() const
	{
		std::stringstream ss;
		ss << getPlayer()->getPlayerName() << ": ";

		switch (m_outcome)
		{
		case Outcome::NotExecuted:
			ss << "Airlift " << m_amount << " troups from " << m_source->getName() << " to " << m_destination->getName();
			break;

		case Outcome::Move:
			ss << "has Airlifted " << m_amount << " troups from " << m_source->getName() << " to " << m_destination->getName();
			break;

		case Outcome::AttackLost:
			ss << *m_source << " failed to conquer " << m_destination->getName() << " with " << m_amount << " troups";
			break;

		case Outcome::AttackWin:
			ss << m_source->getName() << " conquered " << m_destination->getName() << " with " << m_amount << " troups";
			break;
		}

		return ss.str();
	}

	AirliftOrder& AirliftOrder::operator=(const AirliftOrder& other)
	{
		Order::operator=(other);
		m_source = other.m_source;
		m_destination = other.m_destination;
		m_amount = other.m_amount;
		m_outcome = other.m_outcome;

		return *this;
	}

	bool AirliftOrder::operator==(const Order& other) const
	{
		if (typeid(*this) != typeid(other))
		{
			return false;
		}

		AirliftOrder& airlift = (AirliftOrder&)other;

		return Order::operator==(other) && m_amount == airlift.m_amount &&
			*m_destination == *airlift.m_destination;
	}

	// NegotiateOrder /////////////////////////////////////////////////////////////////////////

	NegotiateOrder::NegotiateOrder(Player* p, Player* otherPlayer) : Order(p), m_otherPlayer(otherPlayer) 
	{
		assert(otherPlayer != nullptr);
	}

	NegotiateOrder::NegotiateOrder(const NegotiateOrder& other) 
		: Order(other), m_otherPlayer(other.m_otherPlayer) { }

	const Player* NegotiateOrder::getOtherPlayer() const { return m_otherPlayer; }

	bool NegotiateOrder::validate() const
	{
		std::cout << "Validating NegotiateOrder\n";
		return *getPlayer() != *const_cast<const Player*>(m_otherPlayer);
	}

	void NegotiateOrder::execute()
	{
		if (validate())
		{
			GameManager::addNegotiatingPlayers(getPlayer(), m_otherPlayer);
			m_isExecuted = true;
		}
	}

	std::string NegotiateOrder::toString() const
	{
		std::stringstream ss;
		ss << getPlayer()->getPlayerName() << ": ";

		if (isExecuted())
		{
			ss << "is negotiating with ";
		}
		else
		{
			ss << "negotiate with ";
		}

		ss << m_otherPlayer->getPlayerName();

		return ss.str();
	}

	NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& other)
	{
		Order::operator=(other);
		m_otherPlayer = other.m_otherPlayer;

		return *this;
	}

	bool NegotiateOrder::operator==(const Order& other) const
	{
		if (typeid(*this) != typeid(other))
		{
			return false;
		}

		NegotiateOrder& negotiate = (NegotiateOrder&)other;

		return Order::operator==(other) && *m_otherPlayer == *negotiate.m_otherPlayer;
	}

	// OrderList /////////////////////////////////////////////////////////////////////////

	OrderList::OrderList(size_t defaultSize) 
	{
		m_orders.reserve(defaultSize);
	}

	OrderList::OrderList(const std::initializer_list<Order*>& list) : m_orders(list) { }
	OrderList::OrderList(const OrderList& other) : m_orders(other.m_orders) { }

	size_t OrderList::getCount() const { return m_orders.size(); }

	void OrderList::addOrder(Order* order) { m_orders.push_back(order); }

	void OrderList::deleteOrder(size_t index) { m_orders.erase(m_orders.begin() + index); }
	
	void OrderList::deleteOrder(const Order* order) 
	{
		for (std::vector<Order*>::iterator it = m_orders.begin(); it != m_orders.end(); it++)
		{
			if (*(*it) == *order) 
			{
				m_orders.erase(it);
				return;
			}
		}
	}

	void OrderList::move(size_t orderMoved, size_t dest)
	{
		Order* toMove = m_orders[orderMoved];
		deleteOrder(orderMoved);
		m_orders.insert(m_orders.cbegin() + dest, toMove);
	}

	std::vector<Order*>::iterator OrderList::begin() { return m_orders.begin(); }
	std::vector<Order*>::iterator OrderList::end() { return m_orders.end(); }

	std::vector<Order*>::const_iterator OrderList::begin() const { return m_orders.cbegin(); }
	std::vector<Order*>::const_iterator OrderList::end() const { return m_orders.cend(); }

	Order* OrderList::operator[](size_t index) { return m_orders[index]; }
	const Order* OrderList::operator[](size_t index) const { return m_orders[index]; }

	OrderList& OrderList::operator=(const OrderList& other)
	{
		m_orders = other.m_orders;
		return *this;
	}

	bool OrderList::operator==(const OrderList& other) const
	{
		if (m_orders.size() != other.m_orders.size())
		{
			return false;
		}

		for (size_t i = 0; i < other.m_orders.size(); i++)
		{
			if (*m_orders[i] != *other.m_orders[i])
			{
				return false;
			}
		}

		return true;
	}

	bool OrderList::operator!=(const OrderList& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const OrderList& o)
	{
		std::stringstream ss;

		ss << "OrderList:";

		for (Order* order : o)
		{
			ss << "\t[" << *order << "],\n";
		}

		std::string str = ss.str();

		if(o.getCount() != 0)
		{
			str.erase(str.length() - 2, 2);
		}

		stream << str;
		return stream;
	}

}
