//
// Created by cblah on 4/14/2022.
//

#include "../includes/AntColonySwarm.h"
#include <random>
#include <cmath>

namespace Othello {

thread_local std::mt19937 ac_rng(std::random_device{}());

AntColonySwarm::AntColonySwarm(std::shared_ptr<GameTree> gameTree)
    : MonteCarloAgent(gameTree) {
}

void AntColonySwarm::simulate(std::shared_ptr<GameTreeNode> node) {
    if (!node) return;
    
    std::unique_lock<std::mutex> lock(node->getMutex(), std::try_to_lock);
    if (!lock.owns_lock()) return;
    
    currentNode = node;
    
    // Scan if needed
    ScanState expected = ScanState::NOT_SCANNED;
    if (currentNode->getState() == expected) {
        if (currentNode->compareAndSwapState(expected, ScanState::SCANNING)) {
            gameTree->scanNode(currentNode);
        }
    }
    
    if (!currentNode->getChild()) return;
    
    // Ant Colony specific: prefer nodes with higher win rates (pheromone trails)
    currentNode = currentNode->getChild();
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double bestScore = 0.0;
    auto bestNode = currentNode;
    
    // Evaluate each sibling using pheromone-inspired scoring
    auto tmpNode = currentNode;
    while (tmpNode) {
        int sims = tmpNode->getSimulations();
        if (sims > 0) {
            double winRate = static_cast<double>(tmpNode->getWins()) / sims;
            double pheromone = std::pow(winRate, pheromoneInfluence);
            
            if (pheromone > bestScore && dist(ac_rng) > 0.3) {
                bestScore = pheromone;
                bestNode = tmpNode;
            }
        }
        tmpNode = tmpNode->getSibling();
    }
    
    currentNode = bestNode;
    
    lock.unlock();
    simulate(currentNode);
}

} // namespace Othello