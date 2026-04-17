#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Card.h"
#include <vector>

class Evaluator {
public:
    // 只保留函式原型
    static int getScore(const std::vector<Card>& hand, const std::vector<Card>& community);
};
#endif