#include "Evaluator.h"
#include <algorithm>
#include <map>

// 輔助排序比較函式
bool compareCards(const Card& a, const Card& b) {
    return a.value > b.value; // 由大到小排序
}

int Evaluator::scoreHand(const std::vector<Card>& fullHand) {
    if (fullHand.size() < 5) return 0;

    // 先複製一份並由大到小排序
    std::vector<Card> cards = fullHand;
    std::sort(cards.begin(), cards.end(), compareCards);

    // 1. 統計各花色與各點數的出現次數
    int suitCounts[4] = { 0 };
    std::map<int, int> valueCounts;
    for (const auto& c : cards) {
        suitCounts[c.suit]++;
        valueCounts[c.value]++;
    }

    // 2. 檢查是否有同花 (Flush)
    bool hasFlush = false;
    int flushSuit = -1;
    for (int i = 0; i < 4; ++i) {
        if (suitCounts[i] >= 5) {
            hasFlush = true;
            flushSuit = i;
            break;
        }
    }

    // 3. 提取同花色的所有牌（供同花順檢查使用）
    std::vector<Card> flushCards;
    if (hasFlush) {
        for (const auto& c : cards) {
            if (c.suit == flushSuit) flushCards.push_back(c);
        }
    }

    // 4. 檢查是否有順子 (Straight)
    bool hasStraight = false;
    int straightHigh = -1;

    // 去除重複點數以利檢查順子
    std::vector<int> uniqueValues;
    for (const auto& c : cards) {
        if (uniqueValues.empty() || uniqueValues.back() != c.value) {
            uniqueValues.push_back(c.value);
        }
    }

    // 特殊處理 A,2,3,4,5 的最小順子 (A當作1)
    bool hasAce = (std::find(uniqueValues.begin(), uniqueValues.end(), 14) != uniqueValues.end());

    for (size_t i = 0; i + 4 < uniqueValues.size(); ++i) {
        if (uniqueValues[i] - uniqueValues[i + 4] == 4) {
            hasStraight = true;
            straightHigh = uniqueValues[i];
            break;
        }
    }
    if (!hasStraight && hasAce) {
        if (std::find(uniqueValues.begin(), uniqueValues.end(), 2) != uniqueValues.end() &&
            std::find(uniqueValues.begin(), uniqueValues.end(), 3) != uniqueValues.end() &&
            std::find(uniqueValues.begin(), uniqueValues.end(), 4) != uniqueValues.end() &&
            std::find(uniqueValues.begin(), uniqueValues.end(), 5) != uniqueValues.end()) {
            hasStraight = true;
            straightHigh = 5; // A,2,3,4,5 順子最高是 5
        }
    }

    // 5. 檢查同花順與同花大順 (Straight Flush / Royal Flush)
    if (hasFlush) {
        std::vector<int> fValues;
        for (const auto& c : flushCards) {
            if (fValues.empty() || fValues.back() != c.value) fValues.push_back(c.value);
        }
        for (size_t i = 0; i + 4 < fValues.size(); ++i) {
            if (fValues[i] - fValues[i + 4] == 4) {
                if (fValues[i] == 14) return 90000000; // 同花大順 (Royal Flush)
                return 80000000 + fValues[i] * 10000;  // 同花順 (Straight Flush)
            }
        }
        // 同花順的 A,2,3,4,5 特殊情況
        if (std::find(fValues.begin(), fValues.end(), 14) != fValues.end() &&
            std::find(fValues.begin(), fValues.end(), 2) != fValues.end() &&
            std::find(fValues.begin(), fValues.end(), 3) != fValues.end() &&
            std::find(fValues.begin(), fValues.end(), 4) != fValues.end() &&
            std::find(fValues.begin(), fValues.end(), 5) != fValues.end()) {
            return 80000000 + 5 * 10000;
        }
    }

    // 6. 找出四條、三條、對子的點數
    int fourOfAKindVal = -1;
    int threeOfAKindVal = -1;
    std::vector<int> pairs;

    for (auto const& [val, count] : valueCounts) {
        if (count == 4) fourOfAKindVal = val;
        else if (count == 3) {
            if (val > threeOfAKindVal) threeOfAKindVal = val; // 拿最大的三條
        }
        else if (count == 2) pairs.push_back(val);
    }
    std::sort(pairs.begin(), pairs.end(), std::greater<int>()); // 對子由大到小排

    // 7. 開始進行各大牌型評分 (由大到小檢查)

    // 鐵支 (Four of a Kind)
    if (fourOfAKindVal != -1) {
        int kicker = -1;
        for (const auto& c : cards) {
            if (c.value != fourOfAKindVal) { kicker = c.value; break; }
        }
        return 70000000 + fourOfAKindVal * 10000 + kicker;
    }

    // 葫蘆 (Full House)
    if (threeOfAKindVal != -1 && !pairs.empty()) {
        return 60000000 + threeOfAKindVal * 10000 + pairs[0];
    }
    if (threeOfAKindVal != -1 && std::count_if(valueCounts.begin(), valueCounts.end(), [](auto p) {return p.second == 3;}) > 1) {
        // 如果有兩個三條，其中一個會當作二條組成葫蘆
        int secondThree = -1;
        for (auto const& [val, count] : valueCounts) {
            if (count == 3 && val != threeOfAKindVal) secondThree = val;
        }
        return 60000000 + threeOfAKindVal * 10000 + secondThree;
    }

    // 同花 (Flush)
    if (hasFlush) {
        // 分數由前五張同花色的牌決定
        return 50000000 + flushCards[0].value * 10000 + flushCards[1].value * 100 + flushCards[2].value;
    }

    // 順子 (Straight)
    if (hasStraight) {
        return 40000000 + straightHigh * 10000;
    }

    // 三條 (Three of a Kind)
    if (threeOfAKindVal != -1) {
        int k1 = -1, k2 = -1;
        for (const auto& c : cards) {
            if (c.value != threeOfAKindVal) {
                if (k1 == -1) k1 = c.value;
                else if (k2 == -1) { k2 = c.value; break; }
            }
        }
        return 30000000 + threeOfAKindVal * 10000 + k1 * 100 + k2;
    }

    // 兩對 (Two Pair)
    if (pairs.size() >= 2) {
        int kicker = -1;
        for (const auto& c : cards) {
            if (c.value != pairs[0] && c.value != pairs[1]) { kicker = c.value; break; }
        }
        return 20000000 + pairs[0] * 10000 + pairs[1] * 100 + kicker;
    }

    // 一對 (One Pair)
    if (pairs.size() == 1) {
        int k1 = -1, k2 = -1, k3 = -1;
        for (const auto& c : cards) {
            if (c.value != pairs[0]) {
                if (k1 == -1) k1 = c.value;
                else if (k2 == -1) k2 = c.value;
                else if (k3 == -1) { k3 = c.value; break; }
            }
        }
        return 10000000 + pairs[0] * 10000 + k1 * 100 + k2;
    }

    // 高牌 (High Card)
    return cards[0].value * 100000 + cards[1].value * 1000 + cards[2].value * 10 + cards[3].value;
}