//
// Created by cblah on 5/5/2022.
//

#pragma once

#include "OthelloBitBoard.h"
#include "Move.h"
#include "Constants.h"
#include <memory>
#include <mutex>
#include <atomic>

namespace Othello {

/**
 * Node in the game tree
 * Thread-safe for parallel Monte Carlo simulations
 */
class GameTreeNode : public std::enable_shared_from_this<GameTreeNode> {
public:
    explicit GameTreeNode(std::shared_ptr<OthelloBitBoard> board);
    
    // Disable copying (nodes should be unique)
    GameTreeNode(const GameTreeNode&) = delete;
    GameTreeNode& operator=(const GameTreeNode&) = delete;
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    
    const Move& getLocation() const { return location; }
    ScanState getState() const { return state.load(); }
    Player getColor() const { return color; }
    double getRank() const { return rank; }
    int getWins() const { return wins.load(); }
    int getSimulations() const { return simulations.load(); }
    bool isLocked() const { return locked.load(); }
    
    std::shared_ptr<OthelloBitBoard> getBoard() const { return board; }
    std::shared_ptr<GameTreeNode> getParent() const { return parent.lock(); }
    std::shared_ptr<GameTreeNode> getChild() const { return child; }
    std::shared_ptr<GameTreeNode> getSibling() const { return sibling; }
    
    // ========================================================================
    // SETTERS
    // ========================================================================
    
    void setLocation(const Move& loc) { location = loc; }
    void setState(ScanState s) { state.store(s); }
    void setColor(Player c) { color = c; }
    void setRank(double r) { rank = r; }
    void setWins(int w) { wins.store(w); }
    void setSimulations(int s) { simulations.store(s); }
    void setLocked(bool l) { locked.store(l); }
    
    /**
     * Set child node
     * @param childNode The child to set
     * @param move The move that leads to this child
     */
    void setChild(std::shared_ptr<GameTreeNode> childNode, const Move& move);
    
    /**
     * Set sibling node
     * @param siblingNode The sibling to set
     * @param move The move for this sibling
     */
    void setSibling(std::shared_ptr<GameTreeNode> siblingNode, const Move& move);
    
    // ========================================================================
    // THREAD-SAFE OPERATIONS
    // ========================================================================
    
    /**
     * Atomically increment simulation count
     */
    void incrementSimulations() {
        simulations.fetch_add(1, std::memory_order_relaxed);
    }
    
    /**
     * Atomically increment wins
     */
    void incrementWins() {
        wins.fetch_add(1, std::memory_order_relaxed);
    }
    
    /**
     * Compare-and-swap state atomically
     * @param expected The expected current state
     * @param desired The desired new state
     * @return true if swap succeeded
     */
    bool compareAndSwapState(ScanState expected, ScanState desired) {
        return state.compare_exchange_strong(expected, desired);
    }
    
    /**
     * Get mutex for locking this node
     */
    std::mutex& getMutex() { return nodeMutex; }

private:
    // ========================================================================
    // NODE DATA
    // ========================================================================
    
    Move location;
    std::atomic<ScanState> state;
    Player color;
    double rank;  // Evaluation rank after exploitation
    
    // Thread-safe counters
    std::atomic<int> wins;         // Number of wins in simulations
    std::atomic<int> simulations;  // Number of simulations through this node
    std::atomic<bool> locked;      // Lock for tree modifications
    
    // Board state at this node
    std::shared_ptr<OthelloBitBoard> board;
    
    // Tree structure
    std::weak_ptr<GameTreeNode> parent;      // Weak to avoid cycles
    std::shared_ptr<GameTreeNode> child;     // First child (first legal move)
    std::shared_ptr<GameTreeNode> sibling;   // Next sibling (alternative move)
    
    // Thread synchronization
    std::mutex nodeMutex;
};

} // namespace Othello