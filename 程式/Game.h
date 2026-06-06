#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Card.h"
#include <vector>
#include <SFML/Graphics.hpp>

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

    void startNewRound(sf::RenderWindow& window, sf::Texture& tableTex, sf::Texture& deckTex, sf::Texture& backTex, sf::Font& font);
    void proceedToNextStage(sf::RenderWindow& window, sf::Texture& tableTex, sf::Texture& deckTex, sf::Texture& backTex, sf::Font& font);
    bool isRoundOver() const;

    int getPlayerChips() const; // 🎯 新增：供 main.cpp 檢查玩家死活
    void determineWinner();
    void drawGameElements(sf::RenderWindow& window, sf::Texture& deckTex, sf::Texture& backTex, sf::Font& font);
};

#endif