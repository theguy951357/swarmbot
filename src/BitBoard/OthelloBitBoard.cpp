//
// Created by cblah on 3/4/2022.
//

#include "../includes/OthelloBitBoard.h"
#include "../includes/Utils.h"
#include <iostream>

namespace Othello {

OthelloBitBoard::OthelloBitBoard() 
    : boardBlackPlayer(BitMask::INITIAL_BLACK),
      boardWhitePlayer(BitMask::INITIAL_WHITE),
      legals(BitMask::EMPTY_BOARD) {
}

OthelloBitBoard::OthelloBitBoard(const OthelloBitBoard& other)
    : boardBlackPlayer(other.boardBlackPlayer),
      boardWhitePlayer(other.boardWhitePlayer),
      legals(other.legals) {
}

bool OthelloBitBoard::playPiece(Player player, char y, int16_t x, bool simulation) {
    int16_t shift = Utils::convertToGrid(y, x);
    if (shift < 0) return false;
    
    uint64_t piecePlayed = 1ULL << shift;
    
    if (!isLegalMove(piecePlayed)) {
        return false;
    }
    
    uint64_t playerBoard;
    uint64_t opponentBoard;
    
    if (player == Player::BLACK) {
        playerBoard = this->boardBlackPlayer;
        opponentBoard = this->boardWhitePlayer;
    } else {
        playerBoard = this->boardWhitePlayer;
        opponentBoard = this->boardBlackPlayer;
    }
    
    // Flip pieces using optimized algorithm
    flipPieces(piecePlayed, playerBoard, opponentBoard);
    
    // Update boards
    if (player == Player::BLACK) {
        this->boardBlackPlayer = playerBoard;
        this->boardWhitePlayer = opponentBoard;
    } else {
        this->boardWhitePlayer = playerBoard;
        this->boardBlackPlayer = opponentBoard;
    }
    
    if (!simulation) {
        std::cout << (player == Player::BLACK ? "B " : "W ");
        Utils::printGridLocation(shift);
        std::cout << std::endl;
    }
    
    return true;
}

void OthelloBitBoard::flipPieces(uint64_t move, uint64_t& self, uint64_t& enemy) {
    self |= move;
    
    // Array of shift functions for all 8 directions
    auto processDirection = [&](auto shiftFunc) {
        uint64_t captured = shiftFunc(move) & enemy;
        for (int i = 0; i < 5; ++i) {
            captured |= shiftFunc(captured) & enemy;
        }
        if ((shiftFunc(captured) & self) != 0) {
            self |= captured;
            enemy &= ~captured;
        }
    };
    
    // Process all 8 directions
    processDirection([](uint64_t bb) { return shiftN(bb); });
    processDirection([](uint64_t bb) { return shiftS(bb); });
    processDirection([](uint64_t bb) { return shiftE(bb); });
    processDirection([](uint64_t bb) { return shiftW(bb); });
    processDirection([](uint64_t bb) { return shiftNE(bb); });
    processDirection([](uint64_t bb) { return shiftNW(bb); });
    processDirection([](uint64_t bb) { return shiftSE(bb); });
    processDirection([](uint64_t bb) { return shiftSW(bb); });
}

void OthelloBitBoard::findLegals(Player player) {
    legals = BitMask::EMPTY_BOARD;
    
    uint64_t playerBoard;
    uint64_t opponentBoard;
    
    if (player == Player::BLACK) {
        playerBoard = this->boardBlackPlayer;
        opponentBoard = this->boardWhitePlayer;
    } else {
        playerBoard = this->boardWhitePlayer;
        opponentBoard = this->boardBlackPlayer;
    }
    
    // Search in all 4 base directions (and their opposites)
    for (int i = 0; i < 4; ++i) {
        uint64_t mask;
        if (i == 0) {
            mask = BitMask::VERTICAL;
        } else if (i == 1) {
            mask = BitMask::HORIZONTAL;
        } else {
            mask = BitMask::ALL_SIDES;
        }
        
        searchDirection(playerBoard, opponentBoard, mask, Direction::BASE_OFFSETS[i]);
        searchOtherDirection(playerBoard, opponentBoard, mask, Direction::BASE_OFFSETS[i]);
    }
}

void OthelloBitBoard::searchDirection(uint64_t player, uint64_t opponent, 
                                      uint64_t mask, int16_t dir) {
    uint64_t maskedBoard = opponent & mask;
    uint64_t tmp = maskedBoard & (player << dir);
    
    // Propagate along the direction up to 6 times (max capture length)
    for (int i = 0; i < 6; ++i) {
        tmp |= maskedBoard & (tmp << dir);
    }
    
    uint64_t currentBoard = player | opponent;
    currentBoard = ~currentBoard;  // Empty squares
    
    this->legals |= (tmp << dir) & currentBoard;
}

void OthelloBitBoard::searchOtherDirection(uint64_t player, uint64_t opponent,
                                          uint64_t mask, int16_t dir) {
    uint64_t maskedBoard = opponent & mask;
    uint64_t tmp = maskedBoard & (player >> dir);
    
    for (int i = 0; i < 6; ++i) {
        tmp |= maskedBoard & (tmp >> dir);
    }
    
    uint64_t currentBoard = player | opponent;
    currentBoard = ~currentBoard;
    
    this->legals |= (tmp >> dir) & currentBoard;
}

bool OthelloBitBoard::isLegalMove(uint64_t putted) const {
    // Special case: pass move when no legals available
    if (legals == BitMask::EMPTY_BOARD && putted & (1ULL << 27)) {
        return true;
    }
    
    return (putted & legals) != BitMask::EMPTY_BOARD;
}

bool OthelloBitBoard::gameOver(Player currentPlayer) {
    uint64_t currentBoard = this->boardBlackPlayer | this->boardWhitePlayer;
    
    findLegals(currentPlayer);
    
    // Board full?
    if (currentBoard == BitMask::FULL_BOARD) {
        return true;
    }
    
    // Current player has no moves?
    if (this->legals == BitMask::EMPTY_BOARD) {
        // Check if opponent has moves
        Player opponent = (currentPlayer == Player::BLACK) ? 
                         Player::WHITE : Player::BLACK;
        findLegals(opponent);
        
        if (this->legals == BitMask::EMPTY_BOARD) {
            return true;  // Neither player can move
        }
        
        // Restore legals for current player
        findLegals(currentPlayer);
    }
    
    return false;
}

bool OthelloBitBoard::isPass() const {
    return this->legals == BitMask::EMPTY_BOARD;
}

int OthelloBitBoard::getPieceCount(Player player) const {
    uint64_t board = (player == Player::BLACK) ? 
                     boardBlackPlayer : boardWhitePlayer;
    
    // Use hardware popcount instruction for blazing fast bit counting
    return __builtin_popcountll(board);
}

void OthelloBitBoard::printBoard(uint64_t board) const {
    for (int i = 7; i >= 0; --i) {
        std::cout << "    ";
        for (int j = 7; j >= 0; --j) {
            if (board & (1ULL << (8 * i + j))) {
                std::cout << "1  ";
            } else {
                std::cout << "0  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void OthelloBitBoard::printBoardWithLegalMoves() const {
    int16_t count = 1;
    std::cout << "    a  b  c  d  e  f  g  h" << std::endl;
    
    for (int i = 7; i >= 0; --i) {
        std::cout << " " << count << "  ";
        ++count;
        
        for (int j = 7; j >= 0; --j) {
            uint64_t pos = 1ULL << (8 * i + j);
            
            if (boardBlackPlayer & pos) {
                std::cout << "B  ";
            } else if (boardWhitePlayer & pos) {
                std::cout << "W  ";
            } else if (legals & pos) {
                std::cout << "+  ";
            } else {
                std::cout << "_  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void OthelloBitBoard::printBoard() const {
    int16_t count = 1;
    std::cout << "    a  b  c  d  e  f  g  h" << std::endl;
    
    for (int i = 7; i >= 0; --i) {
        std::cout << " " << count << "  ";
        ++count;
        
        for (int j = 7; j >= 0; --j) {
            uint64_t pos = 1ULL << (8 * i + j);
            
            if (boardBlackPlayer & pos) {
                std::cout << "B  ";
            } else if (boardWhitePlayer & pos) {
                std::cout << "W  ";
            } else {
                std::cout << "_  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void OthelloBitBoard::announceWinner() const {
    // Use optimized popcount
    int blackCount = getPieceCount(Player::BLACK);
    int whiteCount = getPieceCount(Player::WHITE);
    
    Player winner = (blackCount > whiteCount) ? Player::BLACK : Player::WHITE;
    
    std::cout << "Black/White count: " << blackCount << "/" << whiteCount << ". ";
    Utils::printPlayerColor(winner);
    std::cout << " wins the game!" << std::endl;
}

} // namespace Othello