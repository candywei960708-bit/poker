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
    void initDeck();
    void showPlayerChips();

public:
    Game();
    ~Game();
    void addPlayer(std::string name);
    void startRound();
    void determineWinner();
};
#endif