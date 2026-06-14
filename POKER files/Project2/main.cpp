#include <iostream>
#include <string>
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

    if (pokerGame.getPlayerChips() <= 0) {
        std::cout << "\n【GAME OVER】You have 0 chips! You are bankrupt.\n";
        return 0;
    }

    pokerGame.startNewRound();

    while (true) {
        std::cout << "\n>>> Enter 1 to proceed to the next stage, or 0 to exit: ";
        int choice;
        std::cin >> choice;

        if (choice == 0) break;

        if (pokerGame.isRoundOver()) {
            if (pokerGame.getPlayerChips() <= 0) {
                std::cout << "\n【GAME OVER】You have 0 chips! You are bankrupt.\n";
                break;
            }
            pokerGame.startNewRound();
        }
        else {
            pokerGame.proceedToNextStage();
        }
    }

    return 0;
}
