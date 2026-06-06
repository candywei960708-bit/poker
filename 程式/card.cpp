#include "Card.h"

Card::Card(int r, int s) : rank(r), suit(s), isFaceUp(false) {}

int Card::getRank() const { return rank; }
int Card::getSuit() const { return suit; }

std::string Card::toString() const {
    std::string ranks[] = { "", "", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    std::string suits[] = { "[Clubs]", "[Diamonds]", "[Hearts]", "[Spades]" };
    return suits[suit] + ranks[rank];
}

void Card::setFaceUp(bool faceUp) { isFaceUp = faceUp; }
bool Card::getFaceUp() const { return isFaceUp; }

void Card::render(sf::RenderWindow& window, sf::Texture& deckTex, sf::Texture& backTex, float posX, float posY) {
    sf::Sprite sprite;

    if (isFaceUp) {
        sprite.setTexture(deckTex);
        int cardWidth = deckTex.getSize().x / 13;
        int cardHeight = deckTex.getSize().y / 4;

        int colIndex = (rank == 14) ? 0 : (rank - 1);

        int rowIndex = 0;
        if (suit == 3) rowIndex = 0; // Spades
        if (suit == 0) rowIndex = 1; // Clubs
        if (suit == 1) rowIndex = 2; // Diamonds
        if (suit == 2) rowIndex = 3; // Hearts

        sprite.setTextureRect(sf::IntRect(colIndex * cardWidth, rowIndex * cardHeight, cardWidth, cardHeight));
        sprite.setScale(0.8f, 0.8f);
    }
    else {
        sprite.setTexture(backTex);
        sprite.setTextureRect(sf::IntRect(0, 0, backTex.getSize().x, backTex.getSize().y));
        float scaleX = (deckTex.getSize().x / 13.0f * 0.8f) / backTex.getSize().x;
        float scaleY = (deckTex.getSize().y / 4.0f * 0.8f) / backTex.getSize().y;
        sprite.setScale(scaleX, scaleY);
    }

    sprite.setPosition(posX, posY);
    window.draw(sprite);
}