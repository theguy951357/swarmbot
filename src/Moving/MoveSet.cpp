//
// Created by cblah on 3/4/2022.
//

#include "../includes/MoveSet.h"

namespace Othello {

MoveSet::MoveSet() {
    initializeMoves();
}

void MoveSet::initializeMoves() {
    // Initialize all 64 possible moves on the board
    int index = 0;
    for (int row = 1; row <= 8; ++row) {
        for (char col = 'a'; col <= 'h'; ++col) {
            moves[index] = std::make_shared<Move>(col, row);
            ++index;
        }
    }
}

const Move& MoveSet::getMove(int index) const {
    if (index < 0 || index >= 64) {
        // Return a default move for invalid index
        static Move defaultMove('a', 1);
        return defaultMove;
    }
    return *moves[index];
}

std::shared_ptr<Move> MoveSet::getMovePtr(int index) const {
    if (index < 0 || index >= 64) {
        return nullptr;
    }
    return moves[index];
}

} // namespace Othello



