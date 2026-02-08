//
// Created by cblah on 3/4/2022.
//


#include "../includes/Agent.h"
#include "../includes/AntColonySwarm.h"
#include "../includes/Evaluate.h"
#include "../includes/Utils.h"
#include <omp.h>
#include <iostream>
#include <fstream>
#include <numeric>

namespace Othello {

Agent::Agent(std::shared_ptr<OthelloGame> game,
             std::shared_ptr<GameTree> gameTree)
    : Player(game),
      gameTree(gameTree),
      bookmark(gameTree->getRoot()),
      treeCounter(0) {
    
    // Set agent color
    color = game->isAgentBlack() ? Player::BLACK : Player::WHITE;
    
    std::cout << "Agent created as ";
    Utils::printPlayerColor(color);
    std::cout << std::endl;
    
    // Initialize swarm agents
    initializeSwarms();
    
    std::cout << "Swarm initialized with " << swarms.size() 
              << " agents" << std::endl;
}

void Agent::initializeSwarms() {
    int swarmSize = SwarmConfig::getMaxSwarmSize();
    swarms.reserve(swarmSize);
    
    // Create swarm agents (using AntColonySwarm as default)
    for (int i = 0; i < swarmSize; ++i) {
        swarms.push_back(std::make_unique<AntColonySwarm>(gameTree));
    }
}

void Agent::makeMove() {
    decide();
}

void Agent::decide() {
    // Move bookmark to opponent's last move
    moveBookmark();
    
    // Scan current node if needed
    if (bookmark->getState() != ScanState::SCANNED) {
        gameTree->scanNode(bookmark);
    }
    
    if (game->getCurrentPlayer() != color) {
        return;  // Not agent's turn
    }
    
    getLegalMoves();
    bookmark->getBoard()->printBoardWithLegalMoves();
    
    // Run simulations on all children
    auto children = bookmark->getChild();
    while (children) {
        monteCarloParallel(children);
        children = children->getSibling();
    }
    
    // Select best move
    if (!legalMoves.empty()) {
        auto bestNode = analyze();
        const Move& bestMove = bestNode->getLocation();
        
        game->getBoard()->playPiece(color, bestMove.getCol(), 
                                    bestMove.getRow(), false);
        game->setLastMove(Utils::convertToGrid(bestMove.getCol(), 
                                               bestMove.getRow()));
    } else {
        // Pass move
        game->getBoard()->playPiece(color, 'e', 5, false);
        game->setLastMove(Position::PASS);
        bookmark = bookmark->getChild();
    }
    
    std::cout << "Agent plays ";
    Utils::printGridLocation(game->getLastMove());
    std::cout << std::endl;
    
    game->switchPlayer();
    addToStrategy();
}

void Agent::moveBookmark() {
    int16_t lastMove = game->getLastMove();
    
    if (lastMove == Position::NO_MOVE_MADE_YET) {
        std::cout << "No move made yet, bookmark not moved." << std::endl;
        return;
    }
    
    if (!bookmark->getChild()) {
        std::cout << "Bookmark has no children." << std::endl;
        return;
    }
    
    // Move to child
    bookmark = bookmark->getChild();
    
    // Find the child matching the last move
    while (bookmark && 
           lastMove != Utils::convertToGrid(bookmark->getLocation().getCol(),
                                           bookmark->getLocation().getRow())) {
        bookmark = bookmark->getSibling();
    }
    
    if (bookmark) {
        std::cout << "Bookmark moved to ";
        Utils::printGridLocation(lastMove);
        std::cout << std::endl;
    }
}

std::shared_ptr<GameTreeNode> Agent::analyze() {
    auto bestMove = bookmark->getChild();
    auto current = bookmark->getChild();
    
    while (current) {
        Evaluate::exploit(*current);
        
        if (current->getRank() > bestMove->getRank()) {
            bestMove = current;
        }
        
        current = current->getSibling();
    }
    
    bookmark = bestMove;
    return bestMove;
}

void Agent::monteCarloParallel(std::shared_ptr<GameTreeNode> node) {
    const int numThreads = SwarmConfig::getMaxSwarmSize();
    const int totalDives = SwarmConfig::NUMBER_OF_DIVES;
    const int divesPerThread = totalDives / numThreads;
    
    omp_set_num_threads(numThreads);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < totalDives; ++i) {
            // Use modulo to distribute work among swarms
            int swarmId = i % numThreads;
            swarms[swarmId]->simulate(node);
            swarms[swarmId]->findWinValue();
            swarms[swarmId]->backPropagate(gameTree->getRoot(), color);
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    std::cout << totalDives << " dives completed in " 
              << elapsed.count() << " seconds with swarm size " 
              << numThreads << std::endl;
    
    diveTimes.push_back(elapsed.count());
}

void Agent::printTreeToFile(const std::string& filename) {
    std::ofstream output(filename);
    
    if (!output.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }
    
    treeCounter = 0;
    
    // Write Google Charts TreeMap JavaScript
    output << "google.charts.setOnLoadCallback(drawChart);\n"
           << "function drawChart() {\n"
           << "var data = google.visualization.arrayToDataTable([\n"
           << "  ['Move', 'Parent', 'Win Rate'],\n"
           << "  ['0-Root', null, 0],\n";
    
    printTreeHelper(bookmark->getChild(), 1, 0, output);
    
    output << "]);\n"
           << "var tree = new google.visualization.TreeMap("
           << "document.getElementById('tree_div'));\n"
           << "tree.draw(data, {\n"
           << "  minColor: '#f00',\n"
           << "  midColor: '#ddd',\n"
           << "  maxColor: '#0d0',\n"
           << "  headerHeight: 15,\n"
           << "  fontColor: 'black',\n"
           << "  showScale: true\n"
           << "});\n"
           << "}\n";
    
    output.close();
    std::cout << "Tree written to " << filename << std::endl;
}

void Agent::printTreeHelper(std::shared_ptr<GameTreeNode> node,
                            int level,
                            int parentCount,
                            std::ofstream& outfile) {
    if (!node || level >= 10) return;
    
    ++treeCounter;
    int currentId = treeCounter;
    
    int sims = node->getSimulations();
    int wins = node->getWins();
    double winRate = (sims > 0) ? (wins * 100.0 / sims) : 0.0;
    
    outfile << "  ['" << currentId << "-"
            << node->getLocation().getCol() << node->getLocation().getRow()
            << "', '" << parentCount << "-";
    
    auto parent = node->getParent();
    if (parent) {
        outfile << parent->getLocation().getCol() 
                << parent->getLocation().getRow();
    } else {
        outfile << "Root";
    }
    
    outfile << "', " << winRate << "],\n";
    
    // Recurse
    printTreeHelper(node->getSibling(), level, parentCount, outfile);
    printTreeHelper(node->getChild(), level + 1, currentId, outfile);
}

void Agent::printAverageTime() {
    if (diveTimes.empty()) {
        std::cout << "No timing data available." << std::endl;
        return;
    }
    
    double total = std::accumulate(diveTimes.begin(), diveTimes.end(), 0.0);
    double average = total / diveTimes.size();
    
    std::cout << "Average dive time: " << average << " seconds" << std::endl;
}

} // namespace Othello