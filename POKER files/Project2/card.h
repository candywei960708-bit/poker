#ifndef CARD_H
#define CARD_H
#include <string>
#include <SFML/Graphics.hpp>

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
    void render(sf::RenderWindow& window, sf::Texture& deckTex, sf::Texture& backTex, float posX, float posY);
};
#endif