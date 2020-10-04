
#include <iostream>
#include <cstdlib>
#include <random>
#include <time.h>
#include "Cards.h"
#include <sstream>


namespace WZ
{
    using namespace std;

    Deck::Deck() {
        cout << "Creating deck" << endl;
    }

    Deck::~Deck()
    {
        cout << "Deleting deck" << endl;

        //delete all cards to avoid memory leak
        for (Card* c : deck)
        {
            delete c;
        }
    }
    Deck::Deck(const Deck& other)  {
        deck.reserve(other.deck.size());
        for (Card* c:other.deck)
        {
            deck.push_back(new Card(*c));
        }
        
     }

    void Deck::initDeck(Deck* deck)
    {
        cout << "Creating Deck" << endl;

        for (int j = 0; j < 5; j++) { //Creating 5 cards for the 5 types we have
            Card* c = new Card();
            c->setType(j); // each card with a unique type
            deck->addCardToDeck(c); 
        }
    }


    void Deck::addCardToDeck(Card* card) {
        cout << "Adding card to deck" << endl;
        deck.push_back(card); //add it to the deck vector

    }

    void Deck::removeCardFromDeck(Card* card)
    {
        vector<Card*>::iterator it = find(deck.begin(), deck.end(), card);
        if (it != deck.end())
        {
            deck.erase(it);
        }
    }
    Card* Deck::draw()
    {
        if(deck.empty())
        return nullptr;
        int randCard;
        randCard = rand() % (deck.size());
        Card* c = deck.at(randCard);
        deck.erase(deck.begin() + randCard);

        return c;
    }
    std::vector<Card*>::iterator Deck::begin(){return deck.begin();}
    std::vector<Card*>::iterator Deck::end(){return deck.end();}
    std::vector<Card*>::const_iterator Deck::begin() const{return deck.cbegin();}
    std::vector<Card*>::const_iterator Deck::end() const{return deck.cend();}
    std::ostream& operator<<(std::ostream& stream, const Deck& d){
        std::stringstream ss;
        ss<<"Deck:";
		for (Card* card : d)
		{
			ss << "[" << *card << "], ";
		}

		std::string str = ss.str();

		str.erase(str.length() - 2, 2);

		stream << str;
		return stream;

    }

    // Card class
    Card::Card()
    {
        cout << "Creating Card" << endl;
    }

    Card::~Card()
    {
        cout << "Removing Card" << endl;
    }

    Card::Card(const Card& other): type(other.type){ }
    void Card::setType(int random)
    {

        if (random == 0)
        {
            cout << "Bomb Card" << endl;
            type = "bomb";
            return;
        }
        if (random == 1)
        {
            cout << "Reinforcement Card" << endl;
            type = "reinforcement";
            return;
        }if (random == 2)
        {
            cout << "Blockade Card" << endl;
            type = "blockade";
            return;
        }
        if (random == 3)
        {
            cout << "Airlift Card" << endl;
            type = "airlift";
            return;
        }
        if (random == 4)
        {
            cout << "Diplomacy Card" << endl;
            type = "diplomacy";
            return;
        }
    }

    const char* Card::getType() const
    {
        return type;
    }

    Order* Card::play(Territory* start,Territory* dest,Player* p,Player* r,int amount){
         
         if(type =="bomb"){
             
             Order* O= new BombOrder(p,dest);
             return O;
         }
        
         else if (type=="reinforcement"){
             
             Order* O= new DeployOrder(p,dest,amount);
             return O; }
            
         
        else if  (type=="blockade"){
            Order* O= new BlockadeOrder(p,dest);
             return O; }
             


           else if (type=="airlift"){
            Order* O= new AirliftOrder(p, start,dest, amount);
             return O; } 
            


           else if(type=="diplomacy"){
            Order* O= new NegotiateOrder(p,r);
             return O; }
               
     }
     Card& Card::operator=(const Card& other){
         type=other.type;
         return *this;
     }
     
     std::ostream& operator<<(std::ostream& stream, const Card& c){
         stream<<c.getType();
         return stream;
     }

    //Hand class
    Hand::Hand()
    {}

    Hand::~Hand()
    {
        for (vector<Card*>::iterator it = hand.begin(); it != hand.end(); it++)
        {
            delete* it;
            *it = NULL;
        }
        hand.clear();
    }
    Hand::Hand(const Hand& other)  {
        hand.reserve(other.hand.size());
        for (Card* c:other.hand)
        {
            hand.push_back(new Card(*c));
        }
        
     }

    void Hand::addCardToHand(Card* card) {
        cout << "Adding card to deck" << endl;
        hand.push_back(card);
    }

    void Hand::removeCardFromHand(Card* card)
    {
        vector<Card*>::iterator it = find(hand.begin(), hand.end(), card);
        if (it != hand.end())
        {
            hand.erase(it);
        }
    }
    std::vector<Card*>::iterator Hand::begin(){return hand.begin();}
    std::vector<Card*>::iterator Hand::end(){return hand.end();}
    std::vector<Card*>::const_iterator Hand::begin() const{return hand.cbegin();}
    std::vector<Card*>::const_iterator Hand::end() const{return hand.cend();}
    std::ostream& operator<<(std::ostream& stream, const Hand& h){
        std::stringstream ss;
        ss<<"Hand:";
		for (Card* card : h)
		{
			ss << "[" << *card << "], ";
		}

		std::string str = ss.str();

		str.erase(str.length() - 2, 2);

		stream << str;
		return stream;

    }
}
   




