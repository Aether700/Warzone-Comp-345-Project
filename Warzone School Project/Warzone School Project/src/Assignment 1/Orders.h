#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

namespace WZ
{
	class Territory;
	class Player;

	/*Base order class. An abstract class that serves as the base of 
	  every order in the game by providing the api used by the orders
	*/
	class Order
	{
	public:
		/*Constructor of the base Order class, takes in a pointer to the player who gave the order.

		  p: a pointer to the player who gave the order 
		*/
		Order(Player* p);

		/* copy constructor of the Order class
		   
		   other: the other order which will be used to initialize this order
		*/
		Order(const Order& other);

		/* default destructor of the Order class

			while the destructor does not do anything, it is necessary to create 
			a virtual destructor to allow subclasses to be deleted efficiently. 
			This virtual destructor allows them to do so.
		*/
		virtual ~Order();

		/* getter function which 
		   returns a const pointer to the player who gave the order

		   returns: a const pointer to the player who gave the order
		*/
		const Player* getPlayer() const;

		/* getter function which returns a boolian 
		   representing whether or not the order has already been executed

		   returns: true if the order has been executed false otherwise
		*/
		bool isExecuted() const;

		/* validates whether or not the order is a "valid" order
		   the actual implementation is left to the sub classes

		   returns: true if the order is valid false otherwise
		*/
		virtual bool validate() const = 0;

		/* checks if the order is valid and executes it if it is
		   the actual implementation is left to the sub classes
		*/
		virtual void execute() = 0;

		/* returns a string representation of the order Object
		   the actual implementation is left to the sub classes
		   
		   returns: a string representation of the Order object
		*/
		virtual std::string toString() const = 0;

		/* assignement operator. Assigns the values of the provided Order object and returns the modified object 
		   (as is customary)

		   other: the Order object to assign
		   returns: the modified Order object
		*/
		Order& operator=(const Order& other);

		/* equals operator. returns true if the two orders point to the same player 
		   and if they are both executed/not executed. Since this is an abstract class, 
		   this function is used by subclasses to implement their own equality operator.

		   other: the other Order object being compared to this one

		   returns: true if the two orders point to the same player 
		   and if they are both executed/not executed, false otherwise
		*/
		virtual bool operator==(const Order& other) const;

		/*Standard != operator. 
		 Returns true if the == operator was false and vice versa

		 returns: true if the == operator was false and vice versa
		*/
		bool operator!=(const Order& other) const;

	protected:
		bool m_isExecuted;

	private:
		Player* m_player;
	};

	/* insertion operator for every Order object. Since the implementation uses 
	   the virtual function "toString" to get the representation of the object, 
	   this function can be used by any sub class of the Order object 
	   (provided they implement the toString function)

	   stream: the stream to insert the Order into
	   o: the order to insert into the stream
	   returns: the stream provided once the order has been inserted
	*/
	std::ostream& operator<<(std::ostream& stream, const Order& o);
	
	/*DeployOrder class, a sub class of the Order object.
	  represents a deploy order.
	*/
	class DeployOrder : public Order
	{
	public:
		/*constructor of the DeployOrder class

    	   p: the player who gave the order
		   dest: the territory on which to deploy the troups/armies
		   amount: the number of troups/armies to deploy
		*/
		DeployOrder(Player* p, Territory* dest, unsigned int amount);

		/* copy constructor of the DeployOrder class

		   other: the other DeployOrder which will be used to initialize this DeployOrder
		*/
		DeployOrder(const DeployOrder& other);

		/* override of the validate function from the Order class. 
		   Makes sure that the player who gave the order owns the 
		   territory on which the troups are to be deployed

		   returns: true if the order is valid i.e. the player owns the territory on which to deploy
		*/
		virtual bool validate() const override;

		/*override of the execute function from the Order class
		  if the order is valid will deploy the specified number of troups on the territory targeted

		  Note: the current version only prints that a deploy order has been executed (no actual implementation)
		*/
		virtual void execute() override;

		/*override of the toString function from the Order class
		  returns a string representation of the object

		  returns: a string representation of the object
		*/
		virtual std::string toString() const override;

		/* assignement operator. Assigns the values of the provided DeployOrder object and returns the modified object
		   (as is customary)

		   other: the DeployOrder object to assign
		   returns: the modified object
		*/
		DeployOrder& operator=(const DeployOrder& other);

		/* equals operator. returns true if both objects are of type DeployOrder 
		   and they share the same player, the same destination and the same amount.

		   other: the other Order object being compared to this one

		   returns: true if both objects are of type DeployOrder 
		   and they share the same player, the same destination and the same amount, false otherwise
		*/
		bool operator==(const Order& other) const override;

	private:
		Territory* m_destination;
		unsigned int m_amount;
	};

	/*AdvanceOrder class, a sub class of the Order object.
	  represents a advance order.
	*/
	class AdvanceOrder : public Order
	{
	public:
		/*constructor of the AdvanceOrder class

		   p: the player who gave the order
		   source: the territory from which the troups/armies start from
		   target: the territory the troups/armies are traveling to/attacking
		   amount: the number of troups/armies advancing
		*/
		AdvanceOrder(Player* p, Territory* source, Territory* target, unsigned int amount);
		
		/* copy constructor of the AdvanceOrder class

		   other: the other AdvanceOrder which will be used to initialize this AdvanceOrder
		*/
		AdvanceOrder(const AdvanceOrder& other);

		/* override of the validate function from the Order class.
		   Makes sure that the player who gave the order owns the source
		   territory, that enough troups are available on that territory 
		   and that the targeted territory is adjacent to the source territory 
		   (i.e. the source territory has access to the targeted one)

		   returns: true if the order is valid i.e. the player owns the 
		   source territory, if it has enough troups to complete the order and if the 
		   source territory has access to the target one
		*/
		virtual bool validate() const override;

		/*override of the execute function from the Order class
		  if the order is valid will advance the specified number of troups on the territory targeted. 
		  If the targeted territory is owned by the same player the troups will only mobilize there, 
		  otherwise they will attack the territory specified

		  Note: the current version only prints that a advance order has been executed (no actual implementation)
		*/
		virtual void execute() override;

		/*override of the toString function from the Order class
		  returns a string representation of the object

		  returns: a string representation of the object
		*/
		virtual std::string toString() const override;

		/* assignement operator. Assigns the values of the provided AdvanceOrder object and returns the modified object
		   (as is customary)

		   other: the AdvanceOrder object to assign
		   returns: the modified object
		*/
		AdvanceOrder& operator=(const AdvanceOrder& other);

		/* equals operator. returns true if both objects are of type AdvanceOrder
		   and they share the same player, the same source & destination and the same amount.

		   other: the other Order object being compared to this one

		   returns: true if both objects are of type AdvanceOrder
		   and they share the same player, the same source & destination and the same amount, false otherwise
		*/
		bool operator==(const Order& other) const override;

	private:


		Territory* m_source;
		Territory* m_target;
		unsigned int m_amount;
	};

	/*BombOrder class, a sub class of the Order object.
	  represents a bomb order.
	*/
	class BombOrder : public Order
	{
	public:
		/* constructor of the BombOrder class

		   p: the player who gave the order
		   target: the territory to be bombed
		*/
		BombOrder(Player* p, Territory* target);

		/* copy constructor of the BombOrder class

		   other: the other BombOrder which will be used to initialize this BombOrder
		*/
		BombOrder(const BombOrder& other);

		/* override of the validate function from the Order class.
		   Makes sure that the player has a territory adjacent to the targeted 
		   territory, that the territory to be bombed is not owned by the player
		   who gave the order and that both the player bombing and the player 
		   being bombed are not currently in negotiation
		   
		   returns: true if the order is valid i.e. the player does not owns the
		   targeted territory, that they own a territory adjacent to the target and 
		   that both the player bombing and the player 
		   being bombed are not currently in negotiation
		*/
		virtual bool validate() const override;

		/*override of the execute function from the Order class
		  if the order is valid cuts the troup quantity of the targeted territory in half

		  Note: the current version only prints that a bomb order has been executed (no actual implementation)
		*/
		virtual void execute() override;

		/*override of the toString function from the Order class
		  returns a string representation of the object

		  returns: a string representation of the object
		*/
		virtual std::string toString() const override;

		/* assignement operator. Assigns the values of the provided BombOrder object and returns the modified object
		   (as is customary)

		   other: the BombOrder object to assign
		   returns: the modified object
		*/
		BombOrder& operator=(const BombOrder& other);

		/* equals operator. returns true if both objects are of type BombOrder
		   and they share the same player & the same target territory

		   other: the other Order object being compared to this one

		   returns: true if both objects are of type BombOrder
		   and they share the same player& the same target territory, false otherwise
		*/
		bool operator==(const Order& other) const override;

	private:
		Territory* m_target;
	};

	/*BlockadeOrder class, a sub class of the Order object.
	  represents a blockade order.
	*/
	class BlockadeOrder : public Order
	{
	public:
		/* constructor of the BlockadeOrder class

		   p: the player who gave the order
		   target: the territory to be blockaded
		*/
		BlockadeOrder(Player* p, Territory* target);

		/* copy constructor of the BlockadeOrder class

		   other: the other BombOrder which will be used to initialize this BlockadeOrder
		*/
		BlockadeOrder(const BlockadeOrder& other);

		/* override of the validate function from the Order class.
		   Makes sure that the player who gave the order owns the territory 
		   to be blockaded

		   returns: true if the player who gave the order owns the 
		   territory to be blockaded, false otherwise
		*/
		virtual bool validate() const override;
		
		/*override of the execute function from the Order class
		  if the order is valid tripples the troup quantity of the targeted territory and makes it neutral

		  Note: the current version only prints that a blockade order has been executed (no actual implementation)
		*/
		virtual void execute() override;

		/*override of the toString function from the Order class
		  returns a string representation of the object

		  returns: a string representation of the object
		*/
		virtual std::string toString() const override;

		/* assignement operator. Assigns the values of the provided BlockadeOrder object and returns the modified object
		   (as is customary)

		   other: the BlockadeOrder object to assign
		   returns: the modified object
		*/
		BlockadeOrder& operator=(const BlockadeOrder& other);

		/* equals operator. returns true if both objects are of type BlockadeOrder
		   and they share the same player & the same target territory

		   other: the other Order object being compared to this one

		   returns: true if both objects are of type BlockadeOrder
		   and they share the same player& the same target territory, false otherwise
		*/
		bool operator==(const Order& other) const override;

	private:
		Territory* m_target;
	};

	/*AirliftOrder class, a sub class of the Order object.
	  represents a airlift order.
	*/
	class AirliftOrder : public Order
	{
	public:
		/*constructor of the AirliftOrder class

		   p: the player who gave the order
		   source: the territory from which the troups/armies start from
		   destination: the territory the troups/armies are traveling to/attacking
		   amount: the number of troups/armies advancing
		*/
		AirliftOrder(Player* p, Territory* source, Territory* destination, unsigned int amount);

		/* copy constructor of the AirliftOrder class

		   other: the other AirliftOrder which will be used to initialize this AirliftOrder
		*/
		AirliftOrder(const AirliftOrder& other);

		/* override of the validate function from the Order class.
		   Makes sure that the player who gave the order owns the source
		   territory and that enough troups are available on that territory
		   
		   returns: true if the order is valid i.e. the player owns the
		   source territory and if it has enough troups to complete the order
		*/
		virtual bool validate() const override;
		
		/*override of the execute function from the Order class
		  if the order is valid will advance the specified number of troups on the territory targeted.
		  If the targeted territory is owned by the same player the troups will only mobilize there,
		  otherwise they will attack the territory specified

		  Note: the current version only prints that an airlift order has been executed (no actual implementation)
		*/
		virtual void execute() override;

		/*override of the toString function from the Order class
		  returns a string representation of the object

		  returns: a string representation of the object
		*/
		virtual std::string toString() const override;

		/* assignement operator. Assigns the values of the provided AirliftOrder object and returns the modified object
		   (as is customary)

		   other: the AirliftOrder object to assign
		   returns: the modified object
		*/
		AirliftOrder& operator=(const AirliftOrder& other);

		/* equals operator. returns true if both objects are of type AirliftOrder
		   and they share the same player, the same source & destination and the same amount.

		   other: the other Order object being compared to this one

		   returns: true if both objects are of type AirliftOrder
		   and they share the same player, the same source & destination and the same amount, false otherwise
		*/
		bool operator==(const Order& other) const override;

	private:

		Territory* m_source;
		Territory* m_destination;
		unsigned int m_amount;
	};

	/*NegotiateOrder class, a sub class of the Order object.
	  represents a negotiate order.
	*/
	class NegotiateOrder : public Order
	{
	public:
		/*constructor of the NegotiationOrder class

		   p: the player who gave the order
		   otherPlayer: the other player involved in the negotiation
		*/
		NegotiateOrder(Player* p, Player* otherPlayer);
		
		/* copy constructor of the NegotiateOrder class

		   other: the other NegotiateOrder which will be used to initialize this NegotiateOrder
		*/
		NegotiateOrder(const NegotiateOrder& other);

		/*getter function, returns the other player involved in the negotiations

		  returns: the other player involved in the negotiations
		*/
		const Player* getOtherPlayer() const;

		/* override of the validate function from the Order class.
		   Makes sure that the player who is being negotiated with is not the player who gave the order

		   returns: true if the order is valid i.e. the player owns the
		   source territory and if it has enough troups to complete the order
		*/
		virtual bool validate() const override;
		
		/*override of the execute function from the Order class
		  if the order is valid will prevent any attacks from either players targeting the other this turn

		  Note: the current version only prints that an negotiate order has been executed (no actual implementation)
		*/
		virtual void execute() override;

		/*override of the toString function from the Order class
		  returns a string representation of the object

		  returns: a string representation of the object
		*/
		virtual std::string toString() const override;

		/* assignement operator. Assigns the values of the provided NegotiateOrder object and returns the modified object
		   (as is customary)

		   other: the NegotiateOrder object to assign
		   returns: the modified object
		*/
		NegotiateOrder& operator=(const NegotiateOrder& other);

		/* equals operator. returns true if both objects are of type NegotiateOrder
		   and the player who gave the order as well as the other player involved in 
		   the negotiation are the same respectively 

		   other: the other Order object being compared to this one

		   returns: true if both objects are of type NegotiateOrder
		   and the player who gave the order as well as the other player involved in
		   the negotiation are the same respectively
	    */
		bool operator==(const Order& other) const override;

	private:
		Player* m_otherPlayer;
	};

	/*the OrderList class, a list of Orders allowing different
	  operations on these orders such as moving them inside the 
	  list and removing/adding orders to the list
	*/
	class OrderList
	{
	public:
		
		/*default constructor of the OrderList class. 
		  takes in an optional default size for the underlying vector to be resized to.

		  defaultSize: optional argument which defaults to 5. indicates the value that 
		  the underlying vector should be resized to.
		*/
		OrderList(size_t defaultSize = 5);

		/*initializer list constructor of the OrderList class
		  allows the list to be initialized with an initialization list or Order*

		  list: the initialization list to store inside the OrderList object
		*/
		OrderList(const std::initializer_list<Order*>& list);
		
		/*The copy constructor of the OrderList. copies the list 
		  from the list of orders provided to this OrderList object

		  other: the other OrderList object to copy
		*/
		OrderList(const OrderList& other);

		/*returns the number of orders stored inside the list
		  returns: the number of orders stored inside the list
		*/
		size_t getCount() const;

		/*adds the provided Order* to the front of the list

		  order: the order to add to the list
		*/
		void addOrder(Order* order);

		/*deletes the order at the specified index

		  index: the index of the order to remove from the list
		*/
		void deleteOrder(size_t index);

		/*overload of the deleteOrder function. 
		  deletes the order provided if it can be found in the list

		  order: the order to remove from the list if it can be found
		*/
		void deleteOrder(const Order* order);

		/*moves the order at the index specified to the desired index

		  orderMoved: the index of the order to move
		  dest: the index to which the order should be moved
		*/
		void move(size_t orderMoved, size_t dest);

		/* default begin function to allow use of ranged for loops

		returns: an iterator pointing to the beginning of the list
		*/
		std::vector<Order*>::iterator begin();

		/* default end function to allow use of ranged for loops

		returns: an iterator pointing to the end of the list
		*/
		std::vector<Order*>::iterator end();

		/* default begin function to allow use of ranged for loops in a const context

		returns: an const_iterator pointing to the beginning of the list
		*/
		std::vector<Order*>::const_iterator begin() const;
		
		/* default end function to allow use of ranged for loops in a const context

		returns: an const_iterator pointing to the end of the list
		*/
		std::vector<Order*>::const_iterator end() const;

		/*bracket operator used to access elements inside the list at the index specified

		  index: the index of the order to retrieve
		  returns: the Order* at the specifed index
		*/
		Order* operator[](size_t index);
		
		/*const bracket operator used to access elements inside the list at the index specified in a const context

		  index: the index of the order to retrieve
		  returns: the const Order* at the specifed index
		*/
		const Order* operator[](size_t index) const;

		/* assignement operator. copies the values of the provided list to this one and returns the modified list
		   (as is customary)

		   other: the OrderList assign
		   returns: the modified OrderList
		*/
		OrderList& operator=(const OrderList& other);

		/* equals operator. returns true if both lists have the same number of 
		   Orders and they have the same orders at the same positions

		   other: the other OrderList object being compared to this one

		   returns: true if both lists have the same number of 
		   Orders and they have the same orders at the same positions
		*/
		bool operator==(const OrderList& other) const;

		/*Standard != operator.
		 Returns true if the == operator was false and vice versa

		 returns: true if the == operator was false and vice versa
		*/
		bool operator!=(const OrderList& other) const;
	private:
		std::vector<Order*> m_orders;
	};

	/*stream insertion operator of the OrderList class. 
	  inserts the list of orders, order by order

	   stream: the stream to insert the list into
	   o: the OrderList to insert into the stream
	   returns: the stream provided once the list has been inserted
	*/
	std::ostream& operator<<(std::ostream& stream, const OrderList& o);
	
}
