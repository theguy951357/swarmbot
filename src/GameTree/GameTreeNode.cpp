//
// Created by cblah on 5/5/2022.
//

#include "../../includes/GameTreeNode.h"

namespace Othello {

GameTreeNode::GameTreeNode(std::shared_ptr<OthelloBitBoard> board)
    : location(),
      state(ScanState::NOT_SCANNED),
      color(Player::BLACK),
      rank(0.0),
      wins(0),
      simulations(0),
      locked(false),
      board(board),
      parent(),
      child(nullptr),
      sibling(nullptr) {
}

void GameTreeNode::setChild(std::shared_ptr<GameTreeNode> childNode, const Move& move) {
    if (!childNode) return;
    
    // Create a copy of the current board for the child
    auto childBoard = std::make_shared<OthelloBitBoard>(*this->board);
    childNode->board = childBoard;
    childNode->setLocation(move);
    childNode->parent = shared_from_this();
    
    // Play the move on the child's board
    childBoard->playPiece(childNode->getColor(), move.getCol(), move.getRow(), true);
    
    this->child = childNode;
}

void GameTreeNode::setSibling(std::shared_ptr<GameTreeNode> siblingNode, const Move& move) {
    if (!siblingNode) return;
    
    // Siblings share the same parent, so copy parent's board
    auto parentNode = this->parent.lock();
    if (!parentNode) return;
    
    auto siblingBoard = std::make_shared<OthelloBitBoard>(*parentNode->getBoard());
    siblingNode->board = siblingBoard;
    siblingNode->setLocation(move);
    siblingNode->parent = this->parent;
    
    // Play the move on the sibling's board
    siblingBoard->playPiece(siblingNode->getColor(), move.getCol(), move.getRow(), true);
    
    this->sibling = siblingNode;
}

} // namespace Othello