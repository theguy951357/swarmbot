//
// Created by cblah on 3/4/2022.
//

#pragma once

#include "GameTreeNode.h"
#include "OthelloBitBoard.h"
#include "MoveSet.h"
#include <memory>
#include <string>

namespace Othello {

/**
 * Game tree for Othello
 * Manages the tree of possible game states
 */
class GameTree {
public:
    GameTree(std::shared_ptr<OthelloBitBoard> board, 
             std::shared_ptr<MoveSet> moves);
    
    /**
     * Scan a node and generate all its children (legal moves)
     * Optimized to use bit manipulation instead of nested loops
     * @param node The node to scan
     */
    void scanNode(std::shared_ptr<GameTreeNode> node);
    
    /**
     * Get the root node of the tree
     */
    std::shared_ptr<GameTreeNode> getRoot() const { return root; }
    
    /**
     * Print the game tree (for debugging)
     * @param node Starting node
     */
    void printTree(std::shared_ptr<GameTreeNode> node) const;

private:
    std::shared_ptr<GameTreeNode> root;
    std::shared_ptr<MoveSet> moves;
    
    /**
     * Helper function for tree printing
     */
    void printTreeHelper(const std::string& prefix, 
                        std::shared_ptr<GameTreeNode> node, 
                        bool isLeft) const;
};

} // namespace Othello