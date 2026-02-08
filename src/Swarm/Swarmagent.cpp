#include "../includes/SwarmAgent.h"

namespace Othello {

SwarmAgent::SwarmAgent(std::shared_ptr<GameTree> gameTree)
    : gameTree(gameTree),
      currentNode(nullptr),
      winValue(Player::BLACK) {
}

void SwarmAgent::findWinValue() {
    if (!currentNode || !currentNode->getBoard()) {
        winValue = Player::BLACK;
        return;
    }
    
    // Use optimized popcount for piece counting
    int blackCount = currentNode->getBoard()->getPieceCount(Player::BLACK);
    int whiteCount = currentNode->getBoard()->getPieceCount(Player::WHITE);
    
    winValue = (blackCount > whiteCount) ? Player::BLACK : Player::WHITE;
}

void SwarmAgent::backPropagate(std::shared_ptr<GameTreeNode> root,
                                Player playerColor) {
    int wiAdd = (winValue == playerColor) ? 1 : 0;
    
    auto current = currentNode;
    while (current) {
        // Atomic increment (thread-safe)
        current->incrementSimulations();
        if (wiAdd) {
            current->incrementWins();
        }
        current = current->getParent();
    }
}

} // namespace Othello