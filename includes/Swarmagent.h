#pragma once

#include "GameTree.h"
#include "GameTreeNode.h"
#include <memory>
#include <string>

namespace Othello {

/**
 * Abstract base class for all swarm intelligence agents
 * Uses polymorphism to allow different swarm algorithms
 */
class SwarmAgent {
public:
    explicit SwarmAgent(std::shared_ptr<GameTree> gameTree);
    virtual ~SwarmAgent() = default;
    
    /**
     * Simulate a game from the given node
     * Each swarm type implements this differently
     * @param node Starting node for simulation
     */
    virtual void simulate(std::shared_ptr<GameTreeNode> node) = 0;
    
    /**
     * Find the winner from the current simulation
     * Stores result in winValue
     */
    virtual void findWinValue();
    
    /**
     * Back-propagate simulation results up the tree
     * @param root Root node of the tree
     * @param playerColor The player color for this agent
     */
    virtual void backPropagate(std::shared_ptr<GameTreeNode> root, 
                               Player playerColor);
    
    /**
     * Get the name of this swarm algorithm
     * @return Algorithm name
     */
    virtual std::string getName() const = 0;
    
    /**
     * Get genetic algorithm array (for evolutionary swarms)
     * @return Pointer to GA array, or nullptr if not applicable
     */
    virtual int16_t* getGeneticAlgorithm() { return nullptr; }
    
    /**
     * Set genetic algorithm array
     * @param ga The GA array to set
     */
    virtual void setGeneticAlgorithm(int16_t* ga) {}
    
    /**
     * Initialize a new genetic algorithm
     */
    virtual void makeNewGA() {}

protected:
    std::shared_ptr<GameTree> gameTree;
    std::shared_ptr<GameTreeNode> currentNode;
    Player winValue;
};

} // namespace Othello