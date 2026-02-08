//
// Created by cblah on 3/4/2022.
//

##pragma once

#include "Player.h"
#include "GameTree.h"
#include "SwarmAgent.h"
#include <memory>
#include <vector>
#include <chrono>
#include <string>

namespace Othello {

/**
 * AI Agent that uses swarm intelligence algorithms
 */
class Agent : public Player {
public:
    Agent(std::shared_ptr<OthelloGame> game, 
          std::shared_ptr<GameTree> gameTree);
    
    /**
     * Override makeMove to use AI decision making
     */
    void makeMove() override;
    
    /**
     * Main AI decision-making function
     */
    void decide();
    
    /**
     * Print game tree to file for analysis
     * @param filename Output filename
     */
    void printTreeToFile(const std::string& filename);
    
    /**
     * Print average computation time
     */
    void printAverageTime();

private:
    std::shared_ptr<GameTree> gameTree;
    std::vector<std::unique_ptr<SwarmAgent>> swarms;
    std::vector<double> diveTimes;
    std::shared_ptr<GameTreeNode> bookmark;
    
    /**
     * Initialize swarm agents
     */
    void initializeSwarms();
    
    /**
     * Move bookmark to opponent's last move
     */
    void moveBookmark();
    
    /**
     * Analyze children and select best move
     * @return Best child node
     */
    std::shared_ptr<GameTreeNode> analyze();
    
    /**
     * Run parallel Monte Carlo simulations
     * @param node Node to simulate from
     */
    void monteCarloParallel(std::shared_ptr<GameTreeNode> node);
    
    /**
     * Helper for tree printing
     */
    void printTreeHelper(std::shared_ptr<GameTreeNode> node,
                        int level,
                        int parentCount,
                        std::ofstream& outfile);
    
    int treeCounter;
};

} // namespace Othello