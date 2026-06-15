#include "card.h"

Card::Card(int s, int v) : suit(s), value(v) {}

std::string Card::getCardName() const {
    std::string sName[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
    std::string vName;

    if (value <= 10) vName = std::to_string(value);
    else if (value == 11) vName = "J";
    else if (value == 12) vName = "Q";
    else if (value == 13) vName = "K";
    else if (value == 14) vName = "A";

    return sName[suit] + "_" + vName;
}