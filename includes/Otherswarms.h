#pragma once

#include "MonteCarloAgent.h"

namespace Othello {

/**
 * Firefly Algorithm variant
 * Uses light intensity (node attractiveness) for move selection
 */
class FireflySwarm : public MonteCarloAgent {
public:
    explicit FireflySwarm(std::shared_ptr<GameTree> gameTree);
    std::string getName() const override { return "Firefly"; }
};

/**
 * Cuckoo Search variant  
 * Uses Lévy flights and brood parasitism concepts
 */
class CuckooBirdSwarm : public MonteCarloAgent {
public:
    explicit CuckooBirdSwarm(std::shared_ptr<GameTree> gameTree);
    std::string getName() const override { return "Cuckoo Bird"; }
};

} // namespace Othello