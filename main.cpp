#include "includes/OthelloBitBoard.h"
#include "includes/OthelloGame.h"
#include "includes/MoveSet.h"
#include "includes/Player.h"
#include "includes/Agent.h"
#include "includes/GameTree.h"
#include <iostream>
#include <memory>

using namespace Othello;

int main() {
    std::cout << "=== Swarmbot Othello ===" << std::endl;
    std::cout << "An AI powered by swarm intelligence" << std::endl;
    std::cout << std::endl;
    
    // Create game components with smart pointers (no memory leaks!)
    auto moves = std::make_shared<MoveSet>();
    auto board = std::make_shared<OthelloBitBoard>();
    auto gameTree = std::make_shared<GameTree>(board, moves);
    auto game = std::make_shared<OthelloGame>(board);
    
    // Initialize game (ask for player colors)
    game->initialize();
    
    // Create players
    auto player = std::make_unique<Player>(game);
    auto agent = std::make_unique<Agent>(game, gameTree);
    
    // Game loop
    std::cout << std::endl;
    std::cout << "=== Game Start ===" << std::endl;
    std::cout << std::endl;
    
    while (game->nextMove()) {
        if (game->getCurrentPlayer() == agent->getColor()) {
            agent->makeMove();
        } else {
            player->makeMove();
        }
    }
    
    // Game over
    std::cout << std::endl;
    std::cout << "=== Game Over ===" << std::endl;
    
    // Print statistics
    agent->printAverageTime();
    
    // Optional: Export game tree
    std::cout << std::endl;
    std::cout << "Export game tree? (y/n): ";
    std::string exportChoice;
    std::getline(std::cin, exportChoice);
    
    if (!exportChoice.empty() && (exportChoice[0] == 'y' || exportChoice[0] == 'Y')) {
        agent->printTreeToFile("gametree.js");
        std::cout << "Game tree exported to gametree.js" << std::endl;
    }
    
    std::cout << "Thanks for playing!" << std::endl;
    
    // All memory automatically cleaned up by smart pointers!
    return 0;
}