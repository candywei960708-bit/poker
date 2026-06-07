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
    int roundBet;
    bool allIn;

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
    void addChips(int amount);

    void clearCurrentBet();
    int getCurrentBet() const;
    bool isAllIn() const;
};

#endif