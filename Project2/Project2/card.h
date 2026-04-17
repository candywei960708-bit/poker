#ifndef CARD_H
#define CARD_H
#include <string>

class Card {
private:
    int rank;
    int suit;
public:
    Card(int r, int s);
    int getRank() const;
    int getSuit() const;
    std::string toString() const;
};
#endif