#include "Game.h"
#include "Evaluator.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>


Game::Game() : pot(0) {}
Game::~Game() { for (auto p : players) delete p; }

void Game::addPlayer(std::string name) {
    players.push_back(new Player(name));
}

void Game::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; ++s)
        for (int r = 2; r <= 14; ++r)
            deck.push_back(Card(r, s));
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(static_cast<unsigned int>(time(0))));
}

void Game::startRound() {
    initDeck();
    communityCards.clear();
    pot = 0;
    int deckIdx = 0;

    std::cout << "\n--- Round Start! 10 chips deducted as Ante ---" << std::endl;
    for (auto p : players) {
        p->clearHand();
        p->bet(10, pot);
        p->addCard(deck[deckIdx++]);
        p->addCard(deck[deckIdx++]);
    }

    std::cout << "Your Hand: " << players[0]->getHand()[0].toString() << " " << players[0]->getHand()[1].toString() << std::endl;

    std::cout << "Action: 1.Raise(20) 2.Check(Stay) 3.Fold? ";
    int choice; std::cin >> choice;
    int currentRaise = 0;

    if (choice == 1) { players[0]->bet(20, pot); currentRaise = 20; }
    else if (choice == 3) { players[0]->fold(); }

    for (size_t i = 1; i < players.size(); ++i) {
        if (currentRaise > 0) players[i]->bet(20, pot);
    }

    std::cout << "\n--- Community Cards ---" << std::endl;
    for (int i = 0; i < 5; ++i) communityCards.push_back(deck[deckIdx++]);
    for (auto& c : communityCards) std::cout << c.toString() << " ";
    std::cout << "\nPot total: " << pot << std::endl;

    determineWinner();
    showPlayerChips();
}

void Game::determineWinner() {
    int bestScore = -1;
    Player* winner = nullptr;
    std::cout << "\n--- Showdown ---" << std::endl;
    for (auto p : players) {
        if (p->hasFolded()) continue;
        int s = Evaluator::getScore(p->getHand(), communityCards);
        std::cout << p->getName() << " score: " << s << std::endl;
        if (s > bestScore) { bestScore = s; winner = p; }
    }
    if (winner) {
        std::cout << winner->getName() << " wins the pot: " << pot << "!" << std::endl;
        winner->bet(-pot, pot);
    }
}

void Game::showPlayerChips() {
    std::cout << "\n--- Current Status ---" << std::endl;
    for (auto p : players) std::cout << p->getName() << ": " << p->getChips() << std::endl;
}