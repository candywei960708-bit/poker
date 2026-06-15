#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <vector>
#include "card.h"

class Evaluator {
public:
    static int scoreHand(const std::vector<Card>& fullHand);
};

#endif