//
// Created by cblah on 3/4/2022.
//

#include "../includes/GameTree.h"
#include "../includes/Utils.h"
#include <iostream>

namespace Othello {

GameTree::GameTree(std::shared_ptr<OthelloBitBoard> board,
                   std::shared_ptr<MoveSet> moves)
    : moves(moves) {
    root = std::make_shared<GameTreeNode>(board);
}

void GameTree::scanNode(std::shared_ptr<GameTreeNode> node) {
    if (!node) return;
    
    // Switch player color for next move
    Player switchedColor = (node->getColor() == Player::BLACK) ? 
                           Player::WHITE : Player::BLACK;
    
    // Find legal moves for the next player
    node->getBoard()->findLegals(switchedColor);
    uint64_t legalBoard = node->getBoard()->getLegals();
    
    // Check if this is a terminal node (no moves or game over)
    if (legalBoard == BitMask::EMPTY_BOARD) {
        if (!node->getBoard()->gameOver(node->getColor())) {
            // Pass move
            auto passNode = std::make_shared<GameTreeNode>(node->getBoard());
            passNode->setColor(switchedColor);
            node->setChild(passNode, *moves->getMovePtr(Position::PASS));
        }
        node->setState(ScanState::SCANNED);
        return;
    }
    
    // ========================================================================
    // OPTIMIZED: Use bit manipulation instead of nested loops
    // ========================================================================
    
    std::shared_ptr<GameTreeNode> tmpNode = node;
    bool isFirstChild = true;
    
    // Iterate through set bits (legal moves) only
    while (legalBoard != 0) {
        // Find position of lowest set bit (next legal move)
        int bitPosition = __builtin_ctzll(legalBoard);
        
        // Convert to corrected location (flip the board)
        int correctedLocation = 63 - bitPosition;
        
        // Create new node for this move
        auto newNode = std::make_shared<GameTreeNode>(node->getBoard());
        newNode->setColor(switchedColor);
        
        // Add as child or sibling
        if (isFirstChild) {
            node->setChild(newNode, *moves->getMovePtr(correctedLocation));
            tmpNode = node->getChild();
            isFirstChild = false;
        } else {
            tmpNode->setSibling(newNode, *moves->getMovePtr(correctedLocation));
            tmpNode = tmpNode->getSibling();
        }
        
        // Clear the lowest set bit
        legalBoard &= legalBoard - 1;
    }
    
    node->setState(ScanState::SCANNED);
}

void GameTree::printTree(std::shared_ptr<GameTreeNode> node) const {
    printTreeHelper("", node, false);
}

void GameTree::printTreeHelper(const std::string& prefix,
                               std::shared_ptr<GameTreeNode> node,
                               bool isLeft) const {
    if (!node) return;
    
    std::cout << prefix;
    std::cout << (isLeft ? "|--" : "L__");
    
    // Print the move location
    std::cout << node->getLocation().getCol() 
              << node->getLocation().getRow() << std::endl;
    
    // Recursively print children
    printTreeHelper(prefix + (isLeft ? "|  " : "   "), 
                    node->getSibling(), true);
    printTreeHelper(prefix + (isLeft ? "|  " : "   "), 
                    node->getChild(), false);
}

} // namespace Othello