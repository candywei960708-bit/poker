#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

int main() {
    std::cout << "Enter number of NPC players (1-5): ";
    int npcCount;
    std::cin >> npcCount;
    if (npcCount < 1) npcCount = 1;
    if (npcCount > 5) npcCount = 5;

    Game pokerGame;
    pokerGame.addPlayer("You");
    for (int i = 1; i <= npcCount; ++i) {
        pokerGame.addPlayer("NPC_" + std::to_string(i));
    }

    sf::RenderWindow window(sf::VideoMode(1280, 720), "CYCU CSIE Texas Hold'em Poker");
    window.setFramerateLimit(60);

    sf::Texture tableTex, deckTex, backTex;
    if (!tableTex.loadFromFile("assets/table.jpg") ||
        !deckTex.loadFromFile("assets/cards.png") ||
        !backTex.loadFromFile("assets/card_back.png")) {
        std::cerr << "Error loading textures! Ensure paths are correct.\n";
        return -1;
    }

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font!\n";
        return -1;
    }


    if (pokerGame.getPlayerChips() <= 0) {
        std::cout << "\n【GAME OVER】You have 0 chips! You are bankrupt.\n";
        return 0;
    }

    pokerGame.startNewRound(window, tableTex, deckTex, backTex, font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (pokerGame.isRoundOver()) {

              
                    if (pokerGame.getPlayerChips() <= 0) {
                        std::cout << "\n【GAME OVER】You have 0 chips! You are bankrupt.\n";
                        window.close(); // 關閉圖形視窗
                        return 0;       // 終止主程式
                    }

                    pokerGame.startNewRound(window, tableTex, deckTex, backTex, font);
                }
                else {
                    pokerGame.proceedToNextStage(window, tableTex, deckTex, backTex, font);
                }
            }
        }

        window.clear();
        sf::Sprite table(tableTex);
        table.setScale(1280.0f / tableTex.getSize().x, 720.0f / tableTex.getSize().y);
        window.draw(table);

        pokerGame.drawGameElements(window, deckTex, backTex, font);
        window.display();
    }

    return 0;
}