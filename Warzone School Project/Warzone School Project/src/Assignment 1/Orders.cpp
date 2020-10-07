#include "Orders.h"
#include "Map.h"
#include "Player.h"

#include <sstream>
#include <assert.h>

namespace WZ
{
	static bool isNegotiating(const Player* p1, const Player* p2)
	{
		for (Player* p : p1->getNegotiatingPlayers())
		{
			if (*p == *p2)
			{
				return true;
			}
		}

		return false;
	}

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
		return getPlayer()->ownsTerritory(m_destination);
	}
	
	void DeployOrder::execute()
	{
		if (validate())
		{
			std::cout << "Deploy order executed\n";
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
		: Order(p), m_source(source), m_target(target), m_amount(amount)
	{
		assert(source != nullptr);
		assert(target != nullptr);
	}

	AdvanceOrder::AdvanceOrder(const AdvanceOrder& other)
		: Order(other), m_source(other.m_source), m_target(other.m_target), 
		m_amount(other.m_amount)
	{

	}

	bool AdvanceOrder::validate() const
	{
		if (!getPlayer()->ownsTerritory(m_source) || isNegotiating(getPlayer(), (const Player*) m_target->getOwner()))
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
			std::cout << "Advance order executed\n";
			m_isExecuted = true;
		}
	}

	std::string AdvanceOrder::toString() const
	{
		std::stringstream ss;
		ss << *getPlayer() << ": ";

		if (isExecuted())
		{
			ss << "has advanced ";
		}
		else
		{
			ss << "advance ";
		}

		ss << m_amount << " troups from " << *m_source << " to " << *m_target;

		return ss.str();
	}

	AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& other)
	{
		Order::operator=(other);

		m_source = other.m_source;
		m_target = other.m_target;
		m_amount = other.m_amount;

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
		if (getPlayer()->ownsTerritory(m_target))
		{
			return false;
		}

		return HasTerritoryAdj(getPlayer(), m_target) && !isNegotiating(getPlayer(), (const Player*) m_target->getOwner());
	}

	void BombOrder::execute()
	{
		if (validate())
		{
			std::cout << "Bomb order executed\n";
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
		return getPlayer()->ownsTerritory(m_target);
	}

	void BlockadeOrder::execute()
	{
		if (validate())
		{
			std::cout << "Blockade order executed\n";
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
		: Order(p), m_source(source), m_destination(destination), m_amount(amount) 
	{
		assert(source != nullptr);
		assert(destination != nullptr);
	}

	AirliftOrder::AirliftOrder(const AirliftOrder& other) 
		: Order(other), m_source(other.m_source), m_destination(other.m_destination),
		m_amount(other.m_amount) { }

	bool AirliftOrder::validate() const
	{
		return getPlayer()->ownsTerritory(m_source) && m_source->getArmies() >= m_amount;
	}

	void AirliftOrder::execute()
	{
		if (validate())
		{
			std::cout << "Airlift order executed\n";
			m_isExecuted = true;
		}
	}

	std::string AirliftOrder::toString() const
	{
		std::stringstream ss;
		ss << *getPlayer() << ": ";

		if(isExecuted())
		{
			ss << "has Airlifted ";
		}
		else
		{
			ss << "Airlift ";
		}

		ss << m_amount << " troups from " << *m_source << " to " << *m_destination;

		return ss.str();
	}

	AirliftOrder& AirliftOrder::operator=(const AirliftOrder& other)
	{
		Order::operator=(other);
		m_source = other.m_source;
		m_destination = other.m_destination;
		m_amount = other.m_amount;

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
		return *getPlayer() != *const_cast<const Player*>(m_otherPlayer);
	}

	void NegotiateOrder::execute()
	{
		if (validate())
		{
			std::cout << "Negotiation order executed\n";
			m_isExecuted = true;
		}
	}

	std::string NegotiateOrder::toString() const
	{
		std::stringstream ss;
		ss << *getPlayer() << ": ";

		if (isExecuted())
		{
			ss << "is negotiating with ";
		}
		else
		{
			ss << "negotiate with ";
		}

		ss << *m_otherPlayer;

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

		for (Order* order : o)
		{
			ss << "[" << *order << "], ";
		}

		std::string str = ss.str();

		str.erase(str.length() - 2, 2);

		stream << str;
		return stream;
	}

}
