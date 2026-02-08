//
// Created by cblah on 3/4/2022.
//

#pragma once

#include "Move.h"
#include <memory>
#include <array>

namespace Othello {

/**
 * MoveSet maintains all possible moves on an Othello board
 * Uses a lookup table for fast access
 */
class MoveSet {
public:
    MoveSet();
    
    // Get move by index (0-63)
    const Move& getMove(int index) const;
    std::shared_ptr<Move> getMovePtr(int index) const;
    
    // Get total number of moves
    static constexpr int getTotalMoves() { return 64; }

private:
    void initializeMoves();
    
    std::array<std::shared_ptr<Move>, 64> moves;
};

} // namespace Othello