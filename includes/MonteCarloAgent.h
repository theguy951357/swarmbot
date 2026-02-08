//
// Created by cblah on 3/4/2022.
//

#pragma once

#include "SwarmAgent.h"
#include <array>
#include <cstdint>

namespace Othello {

/**
 * Monte Carlo Tree Search agent
 * Base algorithm for all swarm variants
 */
class MonteCarloAgent : public SwarmAgent {
public:
    explicit MonteCarloAgent(std::shared_ptr<GameTree> gameTree);
    
    void simulate(std::shared_ptr<GameTreeNode> node) override;
    std::string getName() const override { return "Monte Carlo"; }
    
    int16_t* getGeneticAlgorithm() override { return geneticAlgorithm.data(); }
    void setGeneticAlgorithm(int16_t* ga) override;
    void makeNewGA() override;

protected:
    std::array<int16_t, 32> geneticAlgorithm;
};

} // namespace Othello