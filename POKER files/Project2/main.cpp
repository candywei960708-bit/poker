#include <iostream>
#include "Game.h"

using namespace std;

int main() {
    cout << "Welcome to Texas Holdem Console Game" << endl;

    cout << "Enter number of NPC opponents (1-5): ";
    int botCount;
    cin >> botCount;

    if (botCount < 1) botCount = 1;

    Game game;
    game.initializeGame(botCount);

    while (!game.isGameOver()) {
        game.startNewRound();

        game.playerActionPhase();

        game.dealFlop();
        game.playerActionPhase();

        game.dealTurn();
        game.playerActionPhase();

        game.dealRiver();
        game.playerActionPhase();

        game.evaluateWinner();
        game.handleBankruptcies();

        if (game.isGameOver()) break;

        cout << "\nPlay next round? (1: Yes, 0: No): ";
        int keepPlaying;
        cin >> keepPlaying;
        if (keepPlaying == 0) break;
    }

    cout << "Game Over. Thanks for playing" << endl;
    return 0;
}