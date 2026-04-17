#include "Evaluator.h"
#include <algorithm>
#include <map>

int Evaluator::getScore(const std::vector<Card>& hand, const std::vector<Card>& community) {
    std::vector<Card> all = hand;
    all.insert(all.end(), community.begin(), community.end());

    std::vector<int> r;
    for (auto& c : all) r.push_back(c.getRank());
    std::sort(r.rbegin(), r.rend());

    std::map<int, int> counts;
    for (int x : r) counts[x]++;

    int score = r[0];
    for (auto const& [rank, count] : counts) {
        if (count == 2) score += 100;
        if (count == 3) score += 400;
    }
    return score;
}