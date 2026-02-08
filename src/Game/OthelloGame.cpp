//
// Created by cblah on 3/4/2022.
//

#include "../includes/OthelloGame.h"
#include "../includes/Utils.h"
#include <iostream>
#include <sstream>
#include <cctype>

namespace Othello {

OthelloGame::OthelloGame(std::shared_ptr<OthelloBitBoard> board)
    : currentPlayer(Player::BLACK),
      board(board),
      agentBlack(false),
      moveMade(false),
      lastMovePlayed(Position::NO_MOVE_MADE_YET) {
}

void OthelloGame::initialize() {
    std::cout << "What color would you like the agent to play?" << std::endl;
    std::cout << "Enter 'W' for white or 'B' for black: ";
    
    std::string input;
    std::getline(std::cin, input);
    
    while (true) {
        if (input.empty()) {
            std::cout << "Please enter 'W' or 'B': ";
            std::getline(std::cin, input);
            continue;
        }
        
        char choice = std::toupper(input[0]);
        
        if (choice == 'W') {
            agentBlack = false;
            std::cout << "Agent will play as White. You will play as Black." << std::endl;
            break;
        } else if (choice == 'B') {
            agentBlack = true;
            std::cout << "Agent will play as Black. You will play as White." << std::endl;
            break;
        } else {
            std::cout << "Invalid input. Please enter 'W' or 'B': ";
            std::getline(std::cin, input);
        }
    }
    
    board->findLegals(currentPlayer);
}

bool OthelloGame::nextMove() {
    if (!board->gameOver(currentPlayer)) {
        return true;
    }
    
    board->printBoard();
    board->announceWinner();
    return false;
}

void OthelloGame::switchPlayer() {
    currentPlayer = (currentPlayer == Player::BLACK) ? 
                    Player::WHITE : Player::BLACK;
    board->findLegals(currentPlayer);
}

bool OthelloGame::interpretInput(const std::string& input) {
    if (input.length() < 5) {
        std::cout << "Invalid input format. Use: 'B c 4' or 'W d 3'" << std::endl;
        moveMade = false;
        return false;
    }
    
    board->printBoardWithLegalMoves();
    
    // Parse input: "B c 4" or "W d 3"
    char playerChar = std::toupper(input[0]);
    char col = std::tolower(input[2]);
    char rowChar = input[4];
    
    Player inputPlayer = (playerChar == 'B') ? Player::BLACK : Player::WHITE;
    int16_t row = rowChar - '0';
    
    // Validate player turn
    if (inputPlayer != currentPlayer) {
        std::cout << "It is not ";
        Utils::printPlayerColor(inputPlayer);
        std::cout << "'s turn yet." << std::endl;
        moveMade = false;
        return false;
    }
    
    // Check for pass
    if (board->isPass()) {
        std::cout << (inputPlayer == Player::BLACK ? "Black" : "White") 
                  << " passes" << std::endl;
        lastMovePlayed = Position::PASS;
        switchPlayer();
        moveMade = true;
        return true;
    }
    
    // Try to make the move
    if (board->playPiece(currentPlayer, col, row, false)) {
        lastMovePlayed = Utils::convertToGrid(col, row);
        switchPlayer();
        moveMade = true;
        return true;
    } else {
        moveMade = false;
        return false;
    }
}

} // namespace Othello