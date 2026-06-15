#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "card.h"

class Player {
public:
    std::string name;
    std::vector<Card> hand;
    int chips;
    int currentBet;
    bool isFolded;
    bool isAllIn;
    bool isBankrupt;

    Player(std::string n = "", int initialChips = 1000);
    void resetForNewRound();
};

#endif