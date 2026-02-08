//
// Created by cblah on 4/14/2022.
//

#pragma once

#include "MonteCarloAgent.h"

namespace Othello {

/**
 * Ant Colony Optimization variant of Monte Carlo agent
 * Uses pheromone-like trail reinforcement
 */
class AntColonySwarm : public MonteCarloAgent {
public:
    explicit AntColonySwarm(std::shared_ptr<GameTree> gameTree);
    
    void simulate(std::shared_ptr<GameTreeNode> node) override;
    std::string getName() const override { return "Ant Colony"; }

private:
    double pheromoneDecay = 0.9;
    double pheromoneInfluence = 1.0;
};

} // namespace Othello