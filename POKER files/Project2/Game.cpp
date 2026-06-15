#include "Game.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "Evaluator.h"

using namespace std;

Game::Game() : pot(0), currentMaxBet(0) {}

void Game::initializeGame(int botCount) {
    players.push_back(Player("Player_You", 1000));
    for (int i = 1; i <= botCount; ++i) {
        players.push_back(Player("Bot_" + std::to_string(i), 1000));
    }
}

void Game::createDeck() {
    deck.clear();
    for (int s = 0; s < 4; ++s) {
        for (int v = 2; v <= 14; ++v) {
            deck.push_back(Card(s, v));
        }
    }
}

void Game::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

void Game::dealInitialCards() {
    for (auto& p : players) {
        if (!p.isBankrupt) {
            p.hand.push_back(deck.back()); deck.pop_back();
            p.hand.push_back(deck.back()); deck.pop_back();
        }
    }
}

void Game::startNewRound() {
    pot = 0;
    currentMaxBet = 0;
    communityCards.clear();
    for (auto& p : players) p.resetForNewRound();

    createDeck();
    shuffleDeck();
    dealInitialCards();
    cout << "\nSTART NEW ROUND" << endl;
}

void Game::printCurrentState() const {
    cout << "\nCURRENT GAME STATE" << endl;
    cout << "Total Pot: " << pot << endl;
    cout << "Community Cards: ";
    if (communityCards.empty()) cout << "None";
    for (const auto& c : communityCards) cout << "[" << c.getCardName() << "] ";
    cout << "\nYour Hand: ";
    for (const auto& c : players[0].hand) cout << "[" << c.getCardName() << "] ";
    cout << " (Your Chips: " << players[0].chips << ")" << endl;
}

void Game::playerActionPhase() {
    // 每次下注圈開始前，重置大家在這一圈的表態狀態
    // 用一個迴圈確保當有人 Raise 時，其他人必須重新表態
    bool bettingComplete = false;

    while (!bettingComplete) {
        bettingComplete = true; // 先假設這輪大家都平齊了

        for (auto& p : players) {
            // 已破產、已蓋牌、已 All-In 的人直接跳過
            if (p.isBankrupt || p.isFolded || p.isAllIn) continue;

            int callAmt = currentMaxBet - p.currentBet;

            // 如果這個玩家目前的下注額不等於目前最高注額，或者大家都還沒下注（最高注為0且玩家也沒表態）
            // 我們就必須詢問他，不讓他偷偷溜過去
            if (callAmt > 0 || (currentMaxBet == 0 && p.currentBet == 0)) {

                if (p.name == "Player_You") {
                    printCurrentState();
                    cout << p.name << " Turn. Current max bet: " << currentMaxBet << " (You bet: " << p.currentBet << ")" << endl;
                    cout << "Select action (1: Call/Check, 2: Raise, 3: All-In, 4: Fold): ";
                    int choice;
                    cin >> choice;

                    if (choice == 2) {
                        cout << "Enter total raise amount: ";
                        int raiseAmt;
                        cin >> raiseAmt;
                        if (raiseAmt >= (p.chips + p.currentBet)) choice = 3; // 籌碼不夠自動變 All-In
                        else if (raiseAmt <= currentMaxBet) {
                            cout << "Invalid raise amount! Forcing a Call instead." << endl;
                            choice = 1; // 亂輸入太低的金額就強制變跟注
                        }
                        else {
                            int extraCost = raiseAmt - p.currentBet;
                            pot += extraCost;
                            p.chips -= extraCost;
                            p.currentBet = raiseAmt;
                            currentMaxBet = raiseAmt;
                            bettingComplete = false; // 有人加注了！其他人待會要再問一輪！
                        }
                    }
                    if (choice == 1) {
                        if (callAmt >= p.chips) choice = 3; // 籌碼不夠跟就強制變 All-In
                        else {
                            pot += callAmt;
                            p.chips -= callAmt;
                            p.currentBet = currentMaxBet;
                            cout << "You chose Call/Check" << endl;
                        }
                    }
                    if (choice == 3) {
                        pot += p.chips;
                        p.currentBet += p.chips;
                        p.chips = 0;
                        p.isAllIn = true;
                        if (p.currentBet > currentMaxBet) {
                            currentMaxBet = p.currentBet;
                            bettingComplete = false; // All-In 金額超越目前最高注，其他人也要重問
                        }
                        cout << "You went ALL-IN" << endl;
                    }
                    if (choice == 4) {
                        p.isFolded = true;
                        cout << "You folded" << endl;
                    }
                }
                else {
                    // ================== NPC AI 行動區 ==================
                    int aiDecision = rand() % 100;

                    // 如果有人加注 (callAmt > 0)，AI 有 20% 機率蓋牌
                    if (callAmt > 0 && aiDecision < 20) {
                        p.isFolded = true;
                        cout << p.name << " chose Fold." << endl;
                    }
                    // AI 有 20% 機率反夾、再加注 50 元
                    else if (aiDecision >= 20 && aiDecision < 40 && p.chips >(callAmt + 50)) {
                        int aiRaiseTotal = currentMaxBet + 50;
                        int extraCost = aiRaiseTotal - p.currentBet;
                        pot += extraCost;
                        p.chips -= extraCost;
                        p.currentBet = aiRaiseTotal;
                        currentMaxBet = aiRaiseTotal;
                        cout << p.name << " chose Raise to " << aiRaiseTotal << endl;
                        bettingComplete = false; // NPC 加注了！等一下會再問一次玩家跟不跟！
                    }
                    // 剩餘狀況一律跟注或 Check
                    else {
                        if (callAmt < p.chips) {
                            pot += callAmt;
                            p.chips -= callAmt;
                            p.currentBet = currentMaxBet;
                            if (callAmt == 0) {
                                cout << p.name << " chose Check." << endl;
                            }
                            else {
                                cout << p.name << " chose Call." << endl;
                            }
                        }
                        else {
                            pot += p.chips;
                            p.currentBet += p.chips;
                            p.chips = 0;
                            p.isAllIn = true;
                            if (p.currentBet > currentMaxBet) {
                                currentMaxBet = p.currentBet;
                                bettingComplete = false;
                            }
                            cout << p.name << " went ALL-IN" << endl;
                        }
                    }
                }
            }
        }

        // 安全防護：如果一整輪問下來，只剩一個活著的人沒蓋牌，就不用繼續賭了，直接跳出
        int activePlayers = 0;
        for (const auto& p : players) {
            if (!p.isFolded && !p.isBankrupt) activePlayers++;
        }
        if (activePlayers <= 1) bettingComplete = true;
    }

    // 下注圈真正結束，把所有人的 currentBet 重置為 0，以便下一圈（如發翻牌後）重新計算
    for (auto& p : players) {
        p.currentBet = 0;
    }
    currentMaxBet = 0;
}

void Game::dealFlop() {
    cout << "\nDealing Flop" << endl;
    for (int i = 0; i < 3; ++i) { communityCards.push_back(deck.back()); deck.pop_back(); }
}

void Game::dealTurn() {
    cout << "\nDealing Turn" << endl;
    communityCards.push_back(deck.back()); deck.pop_back();
}

void Game::dealRiver() {
    cout << "\nDealing River" << endl;
    communityCards.push_back(deck.back()); deck.pop_back();
}

void Game::evaluateWinner() {
    cout << "\nSHOWDOWN RESULT" << endl;
    Player* winner = nullptr;
    int maxScore = -1;

    for (auto& p : players) {
        if (p.isBankrupt || p.isFolded) continue;

        vector<Card> combined = communityCards;
        combined.insert(combined.end(), p.hand.begin(), p.hand.end());
        int score = Evaluator::scoreHand(combined);

        cout << p.name << " hand score: " << score << endl;

        if (score > maxScore) {
            maxScore = score;
            winner = &p;
        }
    }

    if (winner) {
        cout << "\nWinner: " << winner->name << " Wins Pot: " << pot << endl;
        winner->chips += pot;
        pot = 0;
    }
}

void Game::handleBankruptcies() {
    for (auto& p : players) {
        if (!p.isBankrupt && p.chips <= 0) {
            p.isBankrupt = true;
            cout << p.name << " is bankrupt and eliminated" << endl;
        }
    }
}

bool Game::isGameOver() const {
    int activePlayers = 0;
    for (const auto& p : players) {
        if (!p.isBankrupt) activePlayers++;
    }
    return activePlayers <= 1;
}