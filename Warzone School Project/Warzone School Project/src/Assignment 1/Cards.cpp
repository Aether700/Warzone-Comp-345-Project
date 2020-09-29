
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <time.h>
#include "Cards.h"

using namespace std;


    Deck::Deck(){
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

	void Deck::initDeck(Deck *deck)
	{
		cout << "Creating Deck" << endl;

            for(int j = 0; j < 6; j++){ //Creating 6 cards for the 7 types we have
                Card *c = new Card(); 
                c->setType(j); // each card with a unique type
                deck->addCardToDeck(c); //add it to the deck vector
            }
		}
	

    void Deck::addCardToDeck(Card *card){
		cout << "Adding card to deck" << endl;
        vector<Card*>::iterator it = find(deck.begin(), deck.end(), card);
        if (it != deck.end())
        {
            deck.push_back(card); 
        }
    }

     void Deck::removeCardFromDeck(Card *card)
    {
        vector<Card*>::iterator it = find(deck.begin(), deck.end(), card);
        if (it != deck.end())
        {
            deck.erase(it); 
        }
    }
    Card* Deck::draw()
	{
        srand ( time(NULL) ); 
        int randCard;
        randCard = rand() % (deck.size());
		Card* c = deck.at(randCard);
        //need to access the vector in deck and remove card from it
        //then add it to the vector in hand class
        
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
        if(random == 0)
        {
            cout << "Spy Card" << endl;
            type = 'spy';
            return;
        }

        if(random == 1)
        {
			cout << "Bomb Card" << endl;
            type = 'bomb';
            return;
        }
        if(random == 2)
        {
			cout << "Reinforcement Card" << endl;
			type = 'reinforcement';
            return;
        }if(random == 3)
        {
			cout << "Blockade Card" << endl;
			type = 'blockade';
            return;
        }
        if(random == 4)
        {
            cout << "Airlift Card" << endl;
            type = 'airlift';
            return;
        }
        if(random == 5)
        {
            cout << "Diplomacy Card" << endl;
            type = 'diplomacy';
            return;
        }
    }
     
    char Card::getType( ) const
    {
        return type;
    }
//Hand class
Hand::Hand()
    {}
            
Hand::~Hand()
    {
        for (vector<Card*>::iterator it = hand.begin(); it != hand.end(); it++)
        {
            delete *it;
            *it = NULL;
        }
        hand.clear();
    }
            
    
        
   




