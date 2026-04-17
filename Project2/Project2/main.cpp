#include "Game.h"
#include <iostream>

int main() {
    int npcCount;
    std::cout << "Texas Hold'em Poker Project\n";
    std::cout << "Enter number of NPCs (1-5): ";
    std::cin >> npcCount;

    Game pokerGame;
    pokerGame.addPlayer("You");
    for (int i = 0; i < npcCount; ++i) pokerGame.addPlayer("NPC_" + std::to_string(i + 1));

    char cont = 'y';
    while (cont == 'y' || cont == 'Y') {
        pokerGame.startRound();
        std::cout << "\nPlay again? (y/n): ";
        std::cin >> cont;
    }
    return 0;
}