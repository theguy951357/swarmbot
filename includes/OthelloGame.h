//
// Created by cblah on 3/4/2022.
//

#pragma once

#include "OthelloBitBoard.h"
#include "Constants.h"
#include <memory>
#include <string>

namespace Othello {

/**
 * Game controller for Othello
 * Manages game flow and player turns
 */
class OthelloGame {
public:
    explicit OthelloGame(std::shared_ptr<OthelloBitBoard> board);
    
    /**
     * Initialize game by asking for player colors
     * Separated from constructor for testability
     */
    void initialize();
    
    /**
     * Check if there's a next move available
     * @return true if game should continue, false if game over
     */
    bool nextMove();
    
    /**
     * Switch to the other player
     */
    void switchPlayer();
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    
    Player getCurrentPlayer() const { return currentPlayer; }
    bool isAgentBlack() const { return agentBlack; }
    bool isMoveMade() const { return moveMade; }
    int16_t getLastMove() const { return lastMovePlayed; }
    std::shared_ptr<OthelloBitBoard> getBoard() const { return board; }
    
    // ========================================================================
    // SETTERS
    // ========================================================================
    
    void setLastMove(int16_t move) { lastMovePlayed = move; }
    void setMoveMade(bool made) { moveMade = made; }
    
    /**
     * Interpret user input and make a move
     * Format: "B c 4" or "W d 3"
     * @param input The input string
     * @return true if move was made, false otherwise
     */
    bool interpretInput(const std::string& input);

private:
    Player currentPlayer;
    std::shared_ptr<OthelloBitBoard> board;
    bool agentBlack;
    bool moveMade;
    int16_t lastMovePlayed;
};

} // namespace Othello