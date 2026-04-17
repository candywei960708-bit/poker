#ifndef PLAYER_H
#define PLAYER_H
#include "Card.h"
#include <vector>
#include <string>

class Player {
protected:
    std::string name;
    int chips;
    std::vector<Card> hand;
    bool folded;
public:
    Player(std::string n, int c = 1000);
    void addCard(Card c);
    void clearHand();
    void fold();
    bool hasFolded() const;
    int getChips() const;
    std::string getName() const;
    std::vector<Card> getHand() const;
    void bet(int amount, int& pot);
};
#endif