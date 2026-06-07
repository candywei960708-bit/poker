#include "Player.h"

Player::Player(std::string n, int c) : name(n), chips(c), folded(false), roundBet(0), allIn(false) {}

void Player::addCard(Card c) {
    hand.push_back(c);
}

void Player::clearHand() {
    hand.clear();
    folded = false;
    roundBet = 0;
    allIn = false;
}

void Player::fold() {
    folded = true;
}

bool Player::hasFolded() const {
    return folded;
}

int Player::getChips() const {
    return chips;
}

std::string Player::getName() const {
    return name;
}

std::vector<Card> Player::getHand() const {
    return hand;
}

void Player::addChips(int amount) {
    chips += amount;
}

void Player::clearCurrentBet() {
    roundBet = 0;
}

int Player::getCurrentBet() const {
    return roundBet;
}

bool Player::isAllIn() const {
    return allIn;
}

void Player::bet(int amount, int& pot) {
    if (amount >= chips) {
        amount = chips;
        allIn = true;
    }
    chips -= amount;
    pot += amount;
    roundBet += amount;
}