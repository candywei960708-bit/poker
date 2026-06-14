#include "Game.h"
#include "Evaluator.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

Game::Game() : pot(0), roundStage(0) {}
Game::~Game() { for (auto p : players) delete p; }

void Game::addPlayer(std::string name) {
    players.push_back(new Player(name));
}

void Game::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; ++s)
        for (int r = 2; r <= 14; ++r)
            deck.push_back(Card(r, s));

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

void Game::displayBoard() {
    std::cout << "\n==========================================\n";
    std::cout << "[Community Cards]: ";
    if (communityCards.empty()) {
        std::cout << "(None)";
    } else {
        for (const auto& c : communityCards) {
            if (c.getFaceUp()) std::cout << c.toString() << " ";
            else std::cout << "[Hidden] ";
        }
    }
    std::cout << "\n";

    std::cout << "[Your Hand]: ";
    if (!players.empty() && !players[0]->hasFolded()) {
        for (const auto& c : players[0]->getHand()) {
            std::cout << c.toString() << " ";
        }
    } else {
        std::cout << "(Folded)";
    }
    std::cout << "  |  [Chips]: " << getPlayerChips() << "  |  [Pot]: " << pot << "\n";
    std::cout << "==========================================\n";
}

void Game::bettingRound() {
    for (auto p : players) p->clearCurrentBet();

    bool actionComplete = false;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    std::vector<Player*> activeLoggedPlayers;

    while (!actionComplete) {
        actionComplete = true;

        int maxBet = 0;
        for (auto p : players) {
            if (!p->hasFolded() && p->getCurrentBet() > maxBet) {
                maxBet = p->getCurrentBet();
            }
        }

        for (size_t i = 0; i < players.size(); ++i) {
            Player* p = players[i];

            if (p->hasFolded() || p->isAllIn() || p->getChips() <= 0) continue;

            bool hasHeChecked = (std::find(activeLoggedPlayers.begin(), activeLoggedPlayers.end(), p) != activeLoggedPlayers.end());

            if (p->getCurrentBet() < maxBet || (maxBet == 0 && !hasHeChecked)) {
                actionComplete = false;

                if (!hasHeChecked) activeLoggedPlayers.push_back(p);

                int needed = maxBet - p->getCurrentBet();

                if (i == 0) {
                    std::cout << "\n[Your Turn] Chips: " << p->getChips() << " | Pot: " << pot << std::endl;
                    if (needed == 0) {
                        std::cout << "Action: 1.Raise(20) 2.Check(Stay) 3.Fold 4.All-in? ";
                    }
                    else {
                        std::cout << "Action: 1.Raise(20) 2.Call(" << needed << ") 3.Fold 4.All-in? ";
                    }

                    int choice; std::cin >> choice;
                    if (choice == 4) {
                        int allInAmount = p->getChips();
                        p->bet(allInAmount, pot);
                        std::cout << "You WENT ALL-IN with " << p->getCurrentBet() << " chips!\n";

                        if (p->getCurrentBet() > maxBet) {
                            maxBet = p->getCurrentBet();
                            activeLoggedPlayers.clear();
                        }
                        activeLoggedPlayers.push_back(p);
                    }
                    else if (choice == 1) {
                        p->bet(needed + 20, pot);
                        std::cout << "You Raised to " << p->getCurrentBet() << " chips!";
                        if (p->isAllIn()) std::cout << " (ALL-IN!)";
                        std::cout << "\n";

                        maxBet = p->getCurrentBet();
                        activeLoggedPlayers.clear();
                        activeLoggedPlayers.push_back(p);
                    }
                    else if (choice == 2) {
                        p->bet(needed, pot);
                        std::cout << "You Checked/Called.";
                        if (p->isAllIn()) std::cout << " (ALL-IN!)";
                        std::cout << "\n";
                    }
                    else {
                        p->fold();
                        std::cout << "You Folded!\n";
                    }
                }
                else {
                    int dice = dis(gen);

                    if (dice <= 5 && p->getChips() > 0) {
                        int npcAllIn = p->getChips();
                        p->bet(npcAllIn, pot);
                        std::cout << p->getName() << " WENT ALL-IN with " << p->getCurrentBet() << " chips!\n";
                        if (p->getCurrentBet() > maxBet) {
                            maxBet = p->getCurrentBet();
                            activeLoggedPlayers.clear();
                        }
                        activeLoggedPlayers.push_back(p);
                    }
                    else if (dice <= 15) {
                        p->fold();
                        std::cout << p->getName() << " Folds!\n";
                    }
                    else if (dice <= 38 && needed == 0) {
                        p->bet(20, pot);
                        std::cout << p->getName() << " Raises 20 chips!";
                        if (p->isAllIn()) std::cout << " (ALL-IN!)";
                        std::cout << "\n";

                        maxBet = p->getCurrentBet();
                        activeLoggedPlayers.clear();
                        activeLoggedPlayers.push_back(p);
                    }
                    else if (dice <= 28 && needed > 0) {
                        p->bet(needed + 20, pot);
                        std::cout << p->getName() << " Re-raises 20 more chips!";
                        if (p->isAllIn()) std::cout << " (ALL-IN!)";
                        std::cout << "\n";

                        maxBet = p->getCurrentBet();
                        activeLoggedPlayers.clear();
                        activeLoggedPlayers.push_back(p);
                    }
                    else {
                        p->bet(needed, pot);
                        if (needed == 0) std::cout << p->getName() << " Checks.\n";
                        else {
                            std::cout << p->getName() << " Calls " << needed << " chips.";
                            if (p->isAllIn()) std::cout << " (ALL-IN!)";
                            std::cout << "\n";
                        }
                    }
                }
            }
        }

        int activeChasingPlayers = 0;
        for (auto p : players) {
            if (!p->hasFolded() && !p->isAllIn() && p->getChips() > 0) activeChasingPlayers++;
        }
        if (activeChasingPlayers <= 1) break;
    }
    std::cout << "--- Betting Round complete! Pot is now: " << pot << " ---" << std::endl;
}

void Game::startNewRound() {
    initDeck();
    communityCards.clear();
    pot = 0;
    roundStage = 0;

    std::cout << "\n==========================================" << std::endl;
    std::cout << "--- New Round Start! 10 chips Ante deducted ---" << std::endl;

    int deckIdx = 0;
    for (auto p : players) {
        p->clearHand();

        if (p->getChips() <= 0) {
            p->fold();
            continue;
        }

        p->bet(10, pot);

        Card c1 = deck[deckIdx++];
        Card c2 = deck[deckIdx++];
        if (p == players[0]) {
            c1.setFaceUp(true);
            c2.setFaceUp(true);
        }
        p->addCard(c1);
        p->addCard(c2);
    }

    displayBoard();

    if (players[0]->getChips() > 0 && !players[0]->hasFolded()) {
        bettingRound();
    }
}

void Game::proceedToNextStage() {
    if (roundStage >= 4) return;

    roundStage++;
    int deckIdx = (int)players.size() * 2 + (int)communityCards.size();

    if (roundStage == 1) {
        std::cout << "\n--- Stage 1: Flop (Dealing 3 Community Cards) ---" << std::endl;
        for (int i = 0; i < 3; ++i) {
            Card cc = deck[deckIdx++];
            cc.setFaceUp(true);
            communityCards.push_back(cc);
        }
    }
    else if (roundStage == 2) {
        std::cout << "\n--- Stage 2: Turn (Dealing 4th Community Card) ---" << std::endl;
        Card cc = deck[deckIdx++];
        cc.setFaceUp(true);
        communityCards.push_back(cc);
    }
    else if (roundStage == 3) {
        std::cout << "\n--- Stage 3: River (Dealing 5th Community Card) ---" << std::endl;
        Card cc = deck[deckIdx++];
        cc.setFaceUp(true);
        communityCards.push_back(cc);
    }
    else if (roundStage == 4) {
        std::cout << "\n--- Stage 4: Showdown (Revealing Hands) ---" << std::endl;
        determineWinner();
        showPlayerChips();
        return;
    }

    displayBoard();

    int activeNPCs = 0;
    for (size_t i = 1; i < players.size(); ++i) {
        if (!players[i]->hasFolded() && !players[i]->isAllIn() && players[i]->getChips() > 0) activeNPCs++;
    }

    if (!players[0]->hasFolded() && !players[0]->isAllIn() && players[0]->getChips() > 0 && activeNPCs > 0) {
        bettingRound();
    }
    else {
        std::cout << "--- No more betting actions available (Players All-in or Out) ---" << std::endl;
    }
}

int Game::getPlayerChips() const {
    if (players.empty()) return 0;
    return players[0]->getChips();
}

bool Game::isRoundOver() const {
    return roundStage == 4;
}

void Game::determineWinner() {
    int bestScore = -1;
    std::vector<Player*> winners;

    for (auto p : players) {
        if (p->hasFolded()) continue;

        std::vector<Card> currentHand = p->getHand();
        p->clearHand();
        for (auto& c : currentHand) {
            c.setFaceUp(true);
            p->addCard(c);
        }

        int s = Evaluator::getScore(p->getHand(), communityCards);
        std::cout << p->getName() << " [" << Evaluator::getHandName(s) << "] score: " << s << std::endl;

        if (s > bestScore) {
            bestScore = s;
            winners.clear();
            winners.push_back(p);
        }
        else if (s == bestScore) {
            winners.push_back(p);
        }
    }

    if (!winners.empty()) {
        int share = pot / (int)winners.size();
        for (auto w : winners) {
            std::cout << ">>> WINNER: " << w->getName() << " wins " << share << " chips! <<<" << std::endl;
            w->addChips(share);
        }
        pot = 0;
    }
}

void Game::showPlayerChips() {
    std::cout << "\n--- Current Status ---" << std::endl;
    for (auto p : players) std::cout << p->getName() << ": " << p->getChips() << std::endl;
}
