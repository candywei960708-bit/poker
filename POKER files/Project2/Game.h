#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Player.h"
#include "card.h"

class Game {
private:
    std::vector<Card> deck;
    std::vector<Card> communityCards;
    std::vector<Player> players;
    int pot;
    int currentMaxBet;

    void createDeck();
    void shuffleDeck();
    void dealInitialCards();

public:
    Game();
    void initializeGame(int botCount);
    void startNewRound();
    void printCurrentState() const;
    void playerActionPhase();
    void dealFlop();
    void dealTurn();
    void dealRiver();
    void evaluateWinner();
    void handleBankruptcies();
    bool isGameOver() const;
};

#endif