#include "Player.h"

Player::Player(std::string n, int initialChips) {
    name = n;
    chips = initialChips;
    currentBet = 0;
    isFolded = false;
    isAllIn = false;
    isBankrupt = false;
}

void Player::resetForNewRound() {
    hand.clear();
    currentBet = 0;
    isFolded = false;
    isAllIn = false;
    if (chips <= 0) {
        isBankrupt = true;
    }
}