//
// Created by cblah on 3/4/2022.
//

#include "../includes/MonteCarloAgent.h"
#include <random>
#include <algorithm>

namespace Othello {

// Thread-local random number generator for better performance
thread_local std::mt19937 rng(std::random_device{}());

MonteCarloAgent::MonteCarloAgent(std::shared_ptr<GameTree> gameTree)
    : SwarmAgent(gameTree) {
    geneticAlgorithm.fill(0);
}

void MonteCarloAgent::simulate(std::shared_ptr<GameTreeNode> node) {
    if (!node) return;
    
    // Try to acquire lock on the node
    std::unique_lock<std::mutex> lock(node->getMutex(), std::try_to_lock);
    if (!lock.owns_lock()) {
        // Node is busy, skip it
        return;
    }
    
    currentNode = node;
    
    // Atomically check and update state
    ScanState expected = ScanState::NOT_SCANNED;
    if (currentNode->getState() == expected) {
        if (currentNode->compareAndSwapState(expected, ScanState::SCANNING)) {
            gameTree->scanNode(currentNode);
        } else {
            // Another thread is scanning, move to parent
            currentNode = currentNode->getParent();
            if (!currentNode) return;
        }
    }
    
    // Check if we're at a leaf node
    if (!currentNode->getChild()) {
        return;
    }
    
    // Random move selection
    currentNode = currentNode->getChild();
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    // Randomly traverse siblings
    while (currentNode->getSibling() && dist(rng) < 0.5) {
        currentNode = currentNode->getSibling();
    }
    
    // Recursively simulate from selected child
    // Release lock before recursion
    lock.unlock();
    simulate(currentNode);
}

void MonteCarloAgent::setGeneticAlgorithm(int16_t* ga) {
    if (!ga) return;
    std::copy(ga, ga + 32, geneticAlgorithm.begin());
}

void MonteCarloAgent::makeNewGA() {
    std::uniform_int_distribution<int> dist(0, 64);
    
    for (int i = 0; i < 31; ++i) {
        geneticAlgorithm[i] = dist(rng);
    }
    geneticAlgorithm[31] = 0;
}

} // namespace Othello