//
// Created by cblah on 3/4/2022.
//


#include "../../includes/Evaluate.h"
#include <cmath>
#include <random>
#include <algorithm>

namespace Othello {
namespace Evaluate {

// Thread-local random number generator
thread_local std::mt19937 rng(std::random_device{}());

void exploit(GameTreeNode& node) {
    int simulations = node.getSimulations();
    
    if (simulations == 0) {
        node.setRank(0.0);
        return;
    }
    
    int wins = node.getWins();
    double winRate = static_cast<double>(wins) / simulations;
    
    // Get parent simulations for UCB1 calculation
    auto parent = node.getParent();
    int parentSimulations = parent ? parent->getSimulations() : 1;
    
    // Calculate UCB1 value
    double rank = calculateUCB1(node, parentSimulations);
    node.setRank(rank);
}

void mutate(int16_t* one, int16_t* two) {
    if (!one || !two) return;
    
    std::uniform_int_distribution<int> dist(0, 31);
    int swapPoint = dist(rng);
    
    // Swap genetic material at the crossover point
    for (int i = swapPoint; i < 32; ++i) {
        std::swap(one[i], two[i]);
    }
}

int calculateScore(const OthelloBitBoard& board, Player player) {
    int score = 0;
    
    uint64_t playerBoard = (player == Player::BLACK) ? 
                           board.getBoardBlackPlayer() : 
                           board.getBoardWhitePlayer();
    
    // Use bit manipulation to iterate through pieces
    while (playerBoard != 0) {
        int pos = __builtin_ctzll(playerBoard);
        score += Heuristic::BOARD_VALUES[pos];
        playerBoard &= playerBoard - 1;  // Clear lowest bit
    }
    
    return score;
}

double calculateUCB1(const GameTreeNode& node,
                     int parentSimulations,
                     double explorationConstant) {
    int simulations = node.getSimulations();
    
    if (simulations == 0) {
        return std::numeric_limits<double>::infinity();
    }
    
    int wins = node.getWins();
    double winRate = static_cast<double>(wins) / simulations;
    
    // UCB1 formula: winRate + c * sqrt(ln(parentSims) / sims)
    double exploration = explorationConstant * 
                        std::sqrt(std::log(parentSimulations) / simulations);
    
    return winRate + exploration;
}

} // namespace Evaluate
} // namespace Othello