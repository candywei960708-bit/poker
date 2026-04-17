#include "Card.h"

Card::Card(int r, int s) : rank(r), suit(s) {}

int Card::getRank() const { return rank; }
int Card::getSuit() const { return suit; }

std::string Card::toString() const {
    std::string ranks[] = { "", "", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    std::string suits[] = { "[Clubs]", "[Diamonds]", "[Hearts]", "[Spades]" };
    return suits[suit] + ranks[rank];
}