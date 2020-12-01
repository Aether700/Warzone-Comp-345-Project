
#include <iostream>
#include <cstdlib>
#include <random>
#include "Cards.h"
#include <sstream>

#include "Orders.h"
#include "Utils.h"
#include "Player.h"

#define DECK_NUM_CARDS 20

#define CARD_PROB_BOMB 0.2f
#define CARD_PROB_REINFORCEMENT 0.2f
#define CARD_PROB_BLOCKADE 0.2f
#define CARD_PROB_AIRLIFT 0.2f
#define CARD_PROB_DIPLOMACY 0.2f

#define CARD_REINFORCEMENT_VAL 5;

namespace WZ
{
    using namespace std;

    Deck::Deck() 
    {
        initDeck();
    }

    Deck::~Deck()
    {
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

    void Deck::initDeck()
    {
        //add random cards depending on the probability of having those cards
        //to make sure every card type has an equal chance of being choosen we might overload the deck in this phase
        //but leave space for one of each type
        while (deck.size() < DECK_NUM_CARDS - 5)
        {
            if (Random::GetFloat() <= CARD_PROB_AIRLIFT)
            {
                deck.push_back(new Card(Card::Type::Airlift));
            }

            if (Random::GetFloat() <= CARD_PROB_BLOCKADE)
            {
                deck.push_back(new Card(Card::Type::Blockade));
            }

            if (Random::GetFloat() <= CARD_PROB_BOMB)
            {
                deck.push_back(new Card(Card::Type::Bomb));
            }

            if (Random::GetFloat() <= CARD_PROB_DIPLOMACY)
            {
                deck.push_back(new Card(Card::Type::Diplomacy));
            }

            if (Random::GetFloat() <= CARD_PROB_REINFORCEMENT)
            {
                deck.push_back(new Card(Card::Type::Airlift));
            }
        }

        //removes random cards as long as the deck has too many cards
        while (deck.size() > DECK_NUM_CARDS - 5)
        {
            deck.erase(deck.begin() + (Random::GetInt() % deck.size()));
        }

        //add one of each card type at the end to make sure there are one of each type
        deck.push_back(new Card(Card::Type::Airlift));
        deck.push_back(new Card(Card::Type::Blockade));
        deck.push_back(new Card(Card::Type::Bomb));
        deck.push_back(new Card(Card::Type::Diplomacy));
        deck.push_back(new Card(Card::Type::Reinforcement));

        //shrink deck so it fits the num of cards to save memory
        deck.shrink_to_fit();
    }


    size_t Deck::getCount() const{
         return deck.size();
    }

    void Deck::addCardToDeck(Card* card) {
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
    
    Deck& Deck::operator=(const Deck& other){
        for (Card* c : deck)
        {
            delete c;
        }
        for (Card* c:other.deck)
        {
            deck.push_back(new Card(*c));
        }
        return *this;
    }

    Card* Hand::operator[](size_t index) { return hand[index]; }
    
    std::vector<Card*>::iterator Deck::begin(){return deck.begin();}
    std::vector<Card*>::iterator Deck::end(){return deck.end();}
    std::vector<Card*>::const_iterator Deck::begin() const{return deck.cbegin();}
    std::vector<Card*>::const_iterator Deck::end() const{return deck.cend();}
    
    std::ostream& operator<<(std::ostream& stream, const Deck& d){
        std::stringstream ss;
        ss<<"Deck:\n";
		for (Card* card : d)
		{
			ss << "\t[" << *card << "],\n";
		}

		std::string str = ss.str();
        if(d.getCount()!=0){
		    str.erase(str.length() - 2, 2);
        }
		stream << str;
		return stream;
    }

    // Card class
    Card::Card(Card::Type t) : type(t) { }

    Card::~Card() { }

    Card::Card(const Card& other): type(other.type) { }

    Card::Type Card::getType() const
    {
        return type;
    }

    Order* Card::play(Territory* start,Territory* dest,Player* p,Player* r,int amount){
         
         if(type == Card::Type::Bomb){
             std::cout<<"Playing Bomb card"<<std::endl;
             Order* O= new BombOrder(p,dest);
             return O;
         }
         else if (type == Card::Type::Reinforcement){
             std::cout<<"Playing reinforcement card"<<std::endl;
             p->m_strategy->m_reinforcements += CARD_REINFORCEMENT_VAL;
             return nullptr; 
        }
        else if (type == Card::Type::Airlift){
            std::cout<<"Playing airlift card"<<std::endl;
            Order* O= new AirliftOrder(p, start,dest, amount);
             return O; 
        } 
        else if(type == Card::Type::Diplomacy){
            std::cout<<"Playing diplomacy card"<<std::endl;
            Order* O= new NegotiateOrder(p,r);
             return O; 
        }
        else
        {
             std::cout << "Playing blockade card" << std::endl;
             return new BlockadeOrder(p, dest);
        }
    }

     Card& Card::operator=(const Card& other){
         type=other.type;
         return *this;
     }
     
     std::ostream& operator<<(std::ostream& stream, const Card& c){
         switch(c.getType())
         {
            case Card::Type::Airlift:
                stream << "Airlift";
                break;
            
            case Card::Type::Blockade:
                stream << "Blockade";
                break;

            case Card::Type::Bomb:
                stream << "Bomb";
                break;

            case Card::Type::Diplomacy:
                stream << "Diplomacy";
                break;

            case Card::Type::Reinforcement:
                stream << "Reinforcement";
                break;
         }
         return stream;
     }

    //Hand class
    Hand::Hand() { }

    Hand::~Hand()
    {
        for (Card* c : hand)
        {
            delete c;    
        }
    }

    Hand::Hand(const Hand& other)  {
        hand.reserve(other.hand.size());
        for (Card* c:other.hand)
        {
            hand.push_back(new Card(*c));
        }
     }
     
     size_t Hand::getCount() const{
         return hand.size();
     }

    void Hand::addCardToHand(Card* card) {
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

    Hand& Hand::operator=(const Hand& other){
        for (vector<Card*>::iterator it = hand.begin(); it != hand.end(); it++)
        {
            delete* it;    
        }
        for (Card* c:other.hand)
        {
            hand.push_back(new Card(*c));
        }
        return *this;
    }

    std::vector<Card*>::iterator Hand::begin(){return hand.begin();}

    std::vector<Card*>::iterator Hand::end(){return hand.end();}

    std::vector<Card*>::const_iterator Hand::begin() const{return hand.cbegin();}

    std::vector<Card*>::const_iterator Hand::end() const{return hand.cend();}

    std::ostream& operator<<(std::ostream& stream, const Hand& h){
        std::stringstream ss;
        ss<<"Hand:\n";
		for (Card* card : h)
		{
			ss << "\t[" << *card << "],\n";
		}

		std::string str = ss.str();
        if(h.getCount()!= 0){
		    str.erase(str.length() - 2, 2);
        }
		stream << str;
		return stream;
    }
}
   




