#include "Player.h"

Player::Player(std::string n, int c) : name(n), chips(c), folded(false) {}

void Player::addCard(Card c) { hand.push_back(c); }
void Player::clearHand() { hand.clear(); folded = false; }
void Player::fold() { folded = true; }
bool Player::hasFolded() const { return folded; }
int Player::getChips() const { return chips; }
std::string Player::getName() const { return name; }
std::vector<Card> Player::getHand() const { return hand; }

void Player::bet(int amount, int& pot) {
    if (amount > chips) amount = chips;
    chips -= amount;
    pot += amount;
}