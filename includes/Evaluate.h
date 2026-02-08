//
// Created by cblah on 3/4/2022.
//

#pragma once

#include "GameTreeNode.h"
#include "Constants.h"
#include <cstdint>

namespace Othello {
namespace Evaluate {

/**
 * Evaluate a node using UCB1 (Upper Confidence Bound) formula
 * Used for selecting best move in Monte Carlo Tree Search
 * @param node The node to evaluate
 */
void exploit(GameTreeNode& node);

/**
 * Mutate genetic algorithm arrays (for evolutionary swarms)
 * @param one First GA array
 * @param two Second GA array
 */
void mutate(int16_t* one, int16_t* two);

/**
 * Calculate board score for a player using position heuristic
 * @param board The board to evaluate
 * @param player The player to evaluate for
 * @return Score for the player
 */
int calculateScore(const OthelloBitBoard& board, Player player);

/**
 * Calculate UCB1 value for a node
 * @param node The node to calculate for
 * @param parentSimulations Total simulations of parent node
 * @param explorationConstant Constant for exploration vs exploitation balance
 * @return UCB1 value
 */
double calculateUCB1(const GameTreeNode& node, 
                     int parentSimulations,
                     double explorationConstant = 1.41);

} // namespace Evaluate
} // namespace Othello