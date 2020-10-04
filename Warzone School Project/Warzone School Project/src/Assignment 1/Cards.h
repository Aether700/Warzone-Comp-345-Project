

#ifndef CARDS_H_
#define CARDS_H_

#include <vector>

namespace WZ
{
    class Order;
    class Territory;
    class Player;
    
    class Card
    {
    public:
        Card();
        ~Card();
        void setType(int random);
        const char* getType() const;
    private:
        const char* type;
        Order* play(Territory* start,Territory* dest,Player* p);
    };

    class Deck
    {
    public:
        Deck();
        ~Deck();
        void initDeck(Deck* deck);
        void addCardToDeck(Card* card);
        void removeCardFromDeck(Card* card);
        Card* draw();

    private:
        std::vector<Card*> deck;

    };




    class Hand {

    public:
        Hand();
        ~Hand();
        void addCardToHand(Card* card1);
        void removeCardFromHand(Card* card1);

    private:
        std::vector<Card*> hand;
    };
}

#endif /* CARDS_H_ */
