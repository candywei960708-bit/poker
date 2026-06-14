#ifndef CARD_H
#define CARD_H
#include <string>

class Card {
private:
    int rank;
    int suit;
    bool isFaceUp;
public:
    Card(int r, int s);
    int getRank() const;
    int getSuit() const;
    std::string toString() const;

    void setFaceUp(bool faceUp);
    bool getFaceUp() const;
};
#endif
