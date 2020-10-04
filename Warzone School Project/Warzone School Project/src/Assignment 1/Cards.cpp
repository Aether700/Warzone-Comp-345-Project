
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <time.h>
#include "Cards.h"


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
        srand(time(NULL));
        int randCard;
        randCard = rand() % (deck.size());
        Card* c = deck.at(randCard);
        deck.erase(deck.begin() + randCard);

        return c;
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

    Order* play(Territory* start,Territory* dest,Player* p){
         switch(type)
         {
             case bomb:
             Order* O= new BombOrder(p,dest);
             return O;
             break;
        
         
             case reinforcement:
             Order* O= new BombOrder(p,dest);
             return O;
             break;
         
            case blockade:
            Order* O= new BlockadeOrder(p,dest);
             return O;
             break;


            case airlift:
             Order* O= new AirliftOrder(p, start,dest, 5);
             return O;
             break;


            case diplomacy:
            //  Order* O= new BombOrder(p,dest);
            //  return O;
             break;
         
         
         
         
         }


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
}
   




