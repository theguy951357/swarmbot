//
// Created by cblah on 3/4/2022.
//

#include "../../includes/Player.h"
#include "../../includes/Utils.h"
#include <iostream>

namespace Othello {

Player::Player(std::shared_ptr<OthelloGame> game)
    : legalBoard(BitMask::EMPTY_BOARD),
      color(Player::WHITE),
      game(game),
      moves(std::make_shared<MoveSet>()) {
    
    // Set player color opposite to agent
    color = game->isAgentBlack() ? Player::WHITE : Player::BLACK;
}

void Player::makeMove() {
    if (game->getCurrentPlayer() != color) {
        return;  // Not this player's turn
    }
    
    getLegalMoves();
    game->getBoard()->printBoardWithLegalMoves();
    
    std::cout << "Your move (format: 'B c 4' or 'W d 3'): ";
    std::string input;
    std::getline(std::cin, input);
    
    if (game->interpretInput(input)) {
        addToStrategy();
    }
}

void Player::getLegalMoves() {
    legalMoves.clear();
    legalBoard = game->getBoard()->getLegals();
    
    // Use bit manipulation to find legal moves
    uint64_t board = legalBoard;
    while (board != 0) {
        int pos = __builtin_ctzll(board);
        int correctedPos = 63 - pos;
        legalMoves.push_back(moves->getMovePtr(correctedPos));
        board &= board - 1;
    }
}

void Player::addToStrategy() {
    int16_t lastMove = game->getLastMove();
    
    if (lastMove != Position::NO_MOVE_MADE_YET && game->isMoveMade()) {
        strategy.push(moves->getMovePtr(63 - lastMove));
    }
}

void Player::printMoves() const {
    std::cout << "Legal moves: ";
    for (const auto& move : legalMoves) {
        std::cout << move->getCol() << move->getRow() << " ";
    }
    std::cout << std::endl;
}

void Player::printStrategy() const {
    if (strategy.empty()) {
        std::cout << "No moves in strategy yet." << std::endl;
        return;
    }
    
    std::cout << "Move history: ";
    
    // Copy stack to print without modifying
    std::stack<std::shared_ptr<Move>> tempStack = strategy;
    std::vector<std::shared_ptr<Move>> moves;
    
    while (!tempStack.empty()) {
        moves.push_back(tempStack.top());
        tempStack.pop();
    }
    
    // Print in chronological order
    for (auto it = moves.rbegin(); it != moves.rend(); ++it) {
        std::cout << (*it)->getCol() << (*it)->getRow() << " ";
    }
    std::cout << std::endl;
}

} // namespace Othello