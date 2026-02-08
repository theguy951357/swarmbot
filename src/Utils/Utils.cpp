//
// Created by cblah on 3/4/2022.
//

#include "../../includes/Utils.h"
#include <cctype>

namespace Othello {
namespace Utils {

int16_t convertToGrid(char col, int16_t row) {
    // Convert to lowercase for consistency
    col = std::tolower(col);
    
    // Validate input
    if (col < 'a' || col > 'h' || row < 1 || row > 8) {
        return -1;  // Invalid input
    }
    
    // Convert to grid position
    // 'a' = 0, 'b' = 1, etc.
    // row 1 = positions 0-7, row 2 = 8-15, etc.
    int16_t colIndex = col - 'a';
    int16_t rowIndex = row - 1;
    
    return rowIndex * 8 + colIndex;
}

void printGridLocation(int16_t location) {
    if (location < 0 || location > 63) {
        std::cout << "Invalid";
        return;
    }
    
    char col = gridToColumn(location);
    int16_t row = gridToRow(location);
    
    std::cout << col << row;
}

void printPlayerColor(Player player) {
    if (player == Player::BLACK) {
        std::cout << "Black";
    } else {
        std::cout << "White";
    }
}

char gridToColumn(int16_t location) {
    if (location < 0 || location > 63) {
        return '?';
    }
    return 'a' + (location % 8);
}

int16_t gridToRow(int16_t location) {
    if (location < 0 || location > 63) {
        return -1;
    }
    return (location / 8) + 1;
}

} // namespace Utils
} // namespace Othello