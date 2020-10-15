

#ifndef CARDS_H_
#define CARDS_H_

#include <vector>
#include <iostream>

namespace WZ
{
    
    class Territory;
    class Player;
    class Order;
    
    //Card class
    class Card
    {
    public:
        //default constructor of the Card class
        Card(); 
        //default destructor of the Order class
        ~Card();
        //Copy constructor of Card class which takes the other card we want to init
        Card(const Card& other);
        //method to set the type of the card which takes an integer between 0 to 4
        void setType(int random);
        //to return the type of a required Card
        const char* getType() const;
        //play function which allows to play a card and returns the specified card order
        Order* play(Territory* start,Territory* dest,Player* p,Player* r,int amount);
        /* The assignement operator which Assigns the values of the provided Card object
        and returns the modified object */
        Card& operator=(const Card& other);
    private:
        const char* type;
    };
    //insertion operator for every Card object
    std::ostream& operator<<(std::ostream& stream, const Card& c);

    //Deck class
    class Deck
    {
    public:
        //default constructor of the Card class
        Deck();
        //default destructor of the Order class
        ~Deck();
        //Copy constructor of Deck class which takes the other deck we want to init
        Deck(const Deck& other);
        //Initialize our deck with 5 cards one of each type
        void initDeck();
        //get the size of our deck
        size_t getCount () const;
        //to add a card to the deck
        void addCardToDeck(Card* card);
        //to remove a card from the deck
        void removeCardFromDeck(Card* card);
        //draw a random card from the deck and returns a pointer to that card
        Card* draw();
        /* The assignement operator which Assigns the values of the provided Deck object
        and returns the modified object */
        Deck& operator=(const Deck& other);
        /* default begin function to allow use of ranged for loops
		returns: an iterator pointing to the beginning of the list
		*/
        std::vector<Card*>::iterator begin();
        /* default end function to allow use of ranged for loops
		returns: an iterator pointing to the end of the list
		*/
        std::vector<Card*>::iterator end();
        /* default begin function to allow use of ranged for loops in a const context
		returns: an const_iterator pointing to the beginning of the list */
        std::vector<Card*>::const_iterator begin() const;
        /* default end function to allow use of ranged for loops in a const context
		returns: an const_iterator pointing to the end of the list */
        std::vector<Card*>::const_iterator end() const;

    private:
        std::vector<Card*> deck;

    };
    //insertion operator for every Deck object
    std::ostream& operator<<(std::ostream& stream, const Deck& d);

    class Hand {

    public:
        //default constructor of the Hand class
        Hand();
        //default destructor of the Hand class
        ~Hand();
        //Copy constructor of Hand class which takes the other hand we want to init
        Hand(const Hand& other);
        //get the hand's size
        size_t getCount () const;
        //to add a card to the hand list
        void addCardToHand(Card* card1);
        //to remove a card from the hand list
        void removeCardFromHand(Card* card1);
        /* The assignement operator which Assigns the values of the provided Hand object
        and returns the modified object */
        Hand& operator=(const Hand& other);
        /* default begin function to allow use of ranged for loops
		returns: an iterator pointing to the beginning of the list
		*/
        std::vector<Card*>::iterator begin();
        /* default end function to allow use of ranged for loops
		returns: an iterator pointing to the end of the list
		*/
        std::vector<Card*>::iterator end();
        /* default begin function to allow use of ranged for loops in a const context
		returns: an const_iterator pointing to the beginning of the list
		*/
        std::vector<Card*>::const_iterator begin() const;
        /* default end function to allow use of ranged for loops in a const context
		returns: an const_iterator pointing to the end of the list */
        std::vector<Card*>::const_iterator end() const;
        

    private:
        std::vector<Card*> hand;
    };
    //insertion operator for every Hand object
    std::ostream& operator<<(std::ostream& stream, const Hand& h);
}

#endif /* CARDS_H_ */
