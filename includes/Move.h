//
// Created by cblah on 3/4/2022.
//

#pragma once

#include <cstdint>

namespace Othello {

/**
 * Represents a single move in Othello
 */
class Move {
public:
    Move();
    Move(char col, int16_t row);
    
    // Getters
    char getCol() const { return col; }
    int16_t getRow() const { return row; }
    
    // Setters
    void setCol(char c) { col = c; }
    void setRow(int16_t r) { row = r; }
    
    // Comparison operators
    bool operator==(const Move& other) const {
        return col == other.col && row == other.row;
    }
    
    bool operator!=(const Move& other) const {
        return !(*this == other);
    }

private:
    char col;
    int16_t row;
};

} // namespace Othello