#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
    int suit;  // 0: Clubs, 1: Diamonds, 2: Hearts, 3: Spades
    int value; // 2-14 (11:J, 12:Q, 13:K, 14:A)

    Card(int s = 0, int v = 0);
    std::string getCardName() const;
};

#endif