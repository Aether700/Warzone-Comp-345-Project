#include "Cards.h"
#include <iostream>
#include "Player.h"
#include "Map.h"

void CardsDriver() {
    
    WZ::Deck* d1 = new WZ::Deck();
    WZ::Hand* h1 = new WZ::Hand();
    WZ::Player* p = new WZ::Player();
    WZ::Territory* t = new WZ::Territory("territory",1);

    
   
    //d1->initDeck();
    std::cout<<"Deck before drawing "<< *d1<<std::endl<<std::endl;
    std::cout<<"Hand before drawing "<< *h1 << std::endl<<std::endl;
    std::cout<<"Drawing cards and storing them to hand "<<std::endl;
    WZ::Card* c = d1->draw();
    while(c!=nullptr){
        std::cout<<"This is the card type "<<c->getType()<< std::endl;
        h1->addCardToHand(c);
        c = d1->draw();
    }
    std::cout<<"\nDeck after drawing "<< *d1<<std::endl<<std::endl;
    std::cout<<"Hand after drawing "<< *h1 << std::endl<<std::endl;
    std::cout<<"Playing cards from hand:"<< std::endl<<std::endl;
    for(WZ::Card* currentCard: *h1){
        
        WZ::Order* temp=currentCard->play(t,t,p,p,2);
        delete temp;
    }
    delete d1;
    delete h1;
    delete t;
    delete p;











    


 }