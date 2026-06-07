#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Card.h"
#include <vector>
#include <string>

class Evaluator {
public:
    static int getScore(const std::vector<Card>& hand, const std::vector<Card>& community);
    static std::string getHandName(int score) {
        if (score >= 900) return "Straight Flush";
        if (score >= 800) return "Four of a Kind";
        if (score >= 700) return "Full House";
        if (score >= 600) return "Flush";
        if (score >= 500) return "Straight";
        if (score >= 400) return "Three of a Kind";
        if (score >= 300) return "Two Pair";
        if (score >= 200) return "One Pair";
        return "High Card";
    }
};
#endif