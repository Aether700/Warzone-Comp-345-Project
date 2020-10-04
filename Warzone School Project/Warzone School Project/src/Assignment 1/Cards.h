

#ifndef CARDS_H_
#define CARDS_H_

#include <vector>
#include "Orders.h"

namespace WZ
{
    
    class Territory;
    class Player;
    
    class Card
    {
    public:
        Card();
        ~Card();
        Card(const Card& other);
        void setType(int random);
        const char* getType() const;
        Order* play(Territory* start,Territory* dest,Player* p,Player* r);
        Card& operator=(const Card& other);
    private:
        const char* type;
    };
    std::ostream& operator<<(std::ostream& stream, const Card& c);

    class Deck
    {
    public:
        Deck();
        ~Deck();
        Deck(const Deck& other);
        void initDeck(Deck* deck);
        void addCardToDeck(Card* card);
        void removeCardFromDeck(Card* card);
        Card* draw();
        Deck& operator=(const Deck& other);
        std::vector<Card*>::iterator begin();
        std::vector<Card*>::iterator end();
        std::vector<Card*>::const_iterator begin() const;
        std::vector<Card*>::const_iterator end() const;

    private:
        std::vector<Card*> deck;

    };
    std::ostream& operator<<(std::ostream& stream, const Deck& d);

    class Hand {

    public:
        Hand();
        ~Hand();
        Hand(const Hand& other);
        void addCardToHand(Card* card1);
        void removeCardFromHand(Card* card1);
        Hand& operator=(const Hand& other);
        std::vector<Card*>::iterator begin();
        std::vector<Card*>::iterator end();
        std::vector<Card*>::const_iterator begin() const;
        std::vector<Card*>::const_iterator end() const;
        

    private:
        std::vector<Card*> hand;
    };
    std::ostream& operator<<(std::ostream& stream, const Hand& h);
}

#endif /* CARDS_H_ */
