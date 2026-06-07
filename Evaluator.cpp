#include "Evaluator.h"
#include <algorithm>
#include <map>
#include <set>

int Evaluator::getScore(const std::vector<Card>& hand, const std::vector<Card>& community) {
    std::vector<Card> all = hand;
    all.insert(all.end(), community.begin(), community.end());

    std::vector<int> ranks;
    std::map<int, int> suitCounts;
    std::map<int, int> rankCounts;
    std::set<int> uniqueRanks;

    for (const auto& c : all) {
        ranks.push_back(c.getRank());
        rankCounts[c.getRank()]++;
        suitCounts[c.getSuit()]++;
        uniqueRanks.insert(c.getRank());
    }
    std::sort(ranks.rbegin(), ranks.rend());

    bool isFlush = false;
    for (auto const& [suit, count] : suitCounts) {
        if (count >= 5) isFlush = true;
    }

    bool isStraight = false;
    std::vector<int> sortedUnique(uniqueRanks.begin(), uniqueRanks.end());
    if (sortedUnique.size() >= 5) {
        for (size_t i = 0; i <= sortedUnique.size() - 5; ++i) {
            if (sortedUnique[i + 4] - sortedUnique[i] == 4) isStraight = true;
        }
        if (uniqueRanks.count(14) && uniqueRanks.count(2) && uniqueRanks.count(3) &&
            uniqueRanks.count(4) && uniqueRanks.count(5)) isStraight = true;
    }

    int pairs = 0, triples = 0, quads = 0;
    for (auto const& [rank, count] : rankCounts) {
        if (count == 2) pairs++;
        if (count == 3) triples++;
        if (count == 4) quads++;
    }

    int baseScore = ranks[0];

    if (isFlush && isStraight) return 900 + baseScore;
    if (quads > 0)             return 800 + baseScore;
    if (triples > 0 && pairs > 0) return 700 + baseScore;
    if (isFlush)               return 600 + baseScore;
    if (isStraight)            return 500 + baseScore;
    if (triples > 0)           return 400 + baseScore;
    if (pairs >= 2)            return 300 + baseScore;
    if (pairs == 1)            return 200 + baseScore;

    return baseScore;
}