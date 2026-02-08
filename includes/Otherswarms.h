#pragma once

#include "MonteCarloAgent.h"

namespace Othello {

/**
 * Firefly Algorithm variant
 * Uses light intensity (node attractiveness) for move selection
 * 
 * Algorithm:
 * - Nodes emit "light" based on their win rate
 * - Light intensity decreases with distance (number of simulations)
 * - Fireflies are attracted to brighter lights (better positions)
 * - This balances exploration (dim lights) vs exploitation (bright lights)
 */
class FireflySwarm : public MonteCarloAgent {
public:
    explicit FireflySwarm(std::shared_ptr<GameTree> gameTree);
    
    void simulate(std::shared_ptr<GameTreeNode> node) override;
    std::string getName() const override { return "Firefly"; }

private:
    double attractiveness;   // Base attractiveness coefficient
    double lightAbsorption;  // Light absorption coefficient
};

/**
 * Cuckoo Search variant  
 * Uses Lévy flights and brood parasitism concepts
 * 
 * Algorithm:
 * - Uses Lévy flights for move selection (long jumps occasionally)
 * - Abandons poor nests (bad positions) with probability pa
 * - Balances local search (short steps) with global exploration (Lévy flights)
 * - Mimics cuckoo bird behavior of laying eggs in other birds' nests
 */
class CuckooBirdSwarm : public MonteCarloAgent {
public:
    explicit CuckooBirdSwarm(std::shared_ptr<GameTree> gameTree);
    
    void simulate(std::shared_ptr<GameTreeNode> node) override;
    std::string getName() const override { return "Cuckoo Bird"; }

private:
    double levyAlpha;        // Lévy flight exponent (1 < alpha <= 2)
    double abandonmentRate;  // Probability of abandoning a nest (pa)
    
    /**
     * Generate a Lévy flight step size
     * Uses Mantegna's algorithm
     */
    double generateLevyFlight();
};

} // namespace Othello