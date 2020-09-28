

#ifndef CARDS_H_
#define CARDS_H_

#include <vector>

using namespace std;


    class Deck
    {
    public:
        Deck();
        ~Deck();
		void initDeck(Deck* deck);
        void addCardToDeck(Card* card);
        void removeCardFromDeck(Card* card);
        Card* draw();
        Card* play();
        
    private:
		std::vector<Card*> deck;
        
    };

    class Card
    {
    public:
        Card();
        ~Card();
        void setType(int random);
        char getType( ) const;
	private:
		char type;
    };



    class Hand{

    public:
        Hand();
        ~Hand();
        void addCardToHand(Card *card1);
		void removeCardFromHand(Card *card1);

    private:
        std::vector<Card*> hand;
    };


#endif /* CARDS_H_ */
