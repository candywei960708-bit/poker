#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Card.h"
#include <vector>

class Game {
private:
    std::vector<Card> deck;
    std::vector<Player*> players;
    std::vector<Card> communityCards;
    int pot;
    int roundStage;

    void initDeck();
    void showPlayerChips();
    void bettingRound();

public:
    Game();
    ~Game();
    void addPlayer(std::string name);

    void startNewRound();
    void proceedToNextStage();
    bool isRoundOver() const;

    int getPlayerChips() const; 
    void determineWinner();
    void displayBoard(); // 顯示終端機版本的桌面狀態
};

#endif
