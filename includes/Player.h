//
// Created by cblah on 3/4/2022.
//

#pragma once

#include "OthelloGame.h"
#include "Move.h"
#include "MoveSet.h"
#include <memory>
#include <vector>
#include <stack>

namespace Othello {

/**
 * Base class for all players (human and AI)
 */
class Player {
public:
    explicit Player(std::shared_ptr<OthelloGame> game);
    virtual ~Player() = default;  // Virtual destructor for polymorphism
    
    /**
     * Make a move (virtual for Agent override)
     */
    virtual void makeMove();
    
    /**
     * Add current move to strategy history
     */
    void addToStrategy();
    
    /**
     * Get player color
     */
    Player getColor() const { return color; }
    
    /**
     * Print available legal moves
     */
    void printMoves() const;
    
    /**
     * Print move history
     */
    void printStrategy() const;

protected:
    /**
     * Find and store all legal moves for current player
     */
    void getLegalMoves();
    
    uint64_t legalBoard;
    Player color;
    
    std::shared_ptr<OthelloGame> game;
    std::shared_ptr<MoveSet> moves;
    std::vector<std::shared_ptr<Move>> legalMoves;
    std::stack<std::shared_ptr<Move>> strategy;
};

} // namespace Othello