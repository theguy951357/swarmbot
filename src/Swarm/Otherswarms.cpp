#include "../includes/OtherSwarms.h"
#include <random>
#include <cmath>

namespace Othello {

// Thread-local RNG for each swarm type
thread_local std::mt19937 ff_rng(std::random_device{}());
thread_local std::mt19937 cb_rng(std::random_device{}());

// ============================================================================
// FIREFLY SWARM IMPLEMENTATION
// ============================================================================

FireflySwarm::FireflySwarm(std::shared_ptr<GameTree> gameTree)
    : MonteCarloAgent(gameTree),
      attractiveness(1.0),
      lightAbsorption(0.1) {
}

void FireflySwarm::simulate(std::shared_ptr<GameTreeNode> node) {
    if (!node) return;
    
    std::unique_lock<std::mutex> lock(node->getMutex(), std::try_to_lock);
    if (!lock.owns_lock()) return;
    
    currentNode = node;
    
    // Scan if needed
    ScanState expected = ScanState::NOT_SCANNED;
    if (currentNode->getState() == expected) {
        if (currentNode->compareAndSwapState(expected, ScanState::SCANNING)) {
            gameTree->scanNode(currentNode);
        }
    }
    
    if (!currentNode->getChild()) return;
    
    // FIREFLY ALGORITHM: Select node based on "light intensity" (attractiveness)
    // Brighter fireflies (higher win rates) attract others
    currentNode = currentNode->getChild();
    
    std::vector<std::shared_ptr<GameTreeNode>> candidates;
    std::vector<double> intensities;
    
    // Calculate light intensity for each sibling
    auto tmpNode = currentNode;
    while (tmpNode) {
        candidates.push_back(tmpNode);
        
        int sims = tmpNode->getSimulations();
        if (sims > 0) {
            double winRate = static_cast<double>(tmpNode->getWins()) / sims;
            // Light intensity decreases with distance (exploration factor)
            double distance = 1.0 / (1.0 + sims);
            double intensity = attractiveness * winRate * std::exp(-lightAbsorption * distance);
            intensities.push_back(intensity);
        } else {
            intensities.push_back(1.0);  // High intensity for unexplored nodes
        }
        
        tmpNode = tmpNode->getSibling();
    }
    
    // Select node probabilistically based on intensity
    if (!candidates.empty()) {
        double totalIntensity = 0.0;
        for (double i : intensities) totalIntensity += i;
        
        std::uniform_real_distribution<double> dist(0.0, totalIntensity);
        double r = dist(ff_rng);
        
        double cumulative = 0.0;
        for (size_t i = 0; i < candidates.size(); ++i) {
            cumulative += intensities[i];
            if (r <= cumulative) {
                currentNode = candidates[i];
                break;
            }
        }
    }
    
    lock.unlock();
    simulate(currentNode);
}

// ============================================================================
// CUCKOO BIRD SWARM IMPLEMENTATION
// ============================================================================

CuckooBirdSwarm::CuckooBirdSwarm(std::shared_ptr<GameTree> gameTree)
    : MonteCarloAgent(gameTree),
      levyAlpha(1.5),
      abandonmentRate(0.25) {
}

void CuckooBirdSwarm::simulate(std::shared_ptr<GameTreeNode> node) {
    if (!node) return;
    
    std::unique_lock<std::mutex> lock(node->getMutex(), std::try_to_lock);
    if (!lock.owns_lock()) return;
    
    currentNode = node;
    
    // Scan if needed
    ScanState expected = ScanState::NOT_SCANNED;
    if (currentNode->getState() == expected) {
        if (currentNode->compareAndSwapState(expected, ScanState::SCANNING)) {
            gameTree->scanNode(currentNode);
        }
    }
    
    if (!currentNode->getChild()) return;
    
    // CUCKOO SEARCH ALGORITHM: Uses Lévy flights for exploration
    // Occasionally abandons bad nests (nodes with poor performance)
    
    currentNode = currentNode->getChild();
    
    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    double r = uniform(cb_rng);
    
    // Abandonment: Sometimes skip poorly performing nodes
    if (r < abandonmentRate) {
        // Find best performing sibling (abandon the worst)
        auto bestNode = currentNode;
        double bestScore = -1.0;
        
        auto tmpNode = currentNode;
        while (tmpNode) {
            int sims = tmpNode->getSimulations();
            if (sims > 0) {
                double winRate = static_cast<double>(tmpNode->getWins()) / sims;
                if (winRate > bestScore) {
                    bestScore = winRate;
                    bestNode = tmpNode;
                }
            } else {
                // Unexplored nodes are attractive
                bestNode = tmpNode;
                break;
            }
            tmpNode = tmpNode->getSibling();
        }
        
        currentNode = bestNode;
    } else {
        // Lévy flight: Long jumps occasionally, short steps usually
        double levyStep = generateLevyFlight();
        
        // Normalize to number of siblings
        int siblingCount = 0;
        auto tmpNode = currentNode;
        while (tmpNode) {
            siblingCount++;
            tmpNode = tmpNode->getSibling();
        }
        
        if (siblingCount > 1) {
            // Use Lévy flight to determine how many siblings to skip
            int stepsToSkip = static_cast<int>(levyStep * siblingCount) % siblingCount;
            
            for (int i = 0; i < stepsToSkip && currentNode->getSibling(); ++i) {
                currentNode = currentNode->getSibling();
            }
        }
    }
    
    lock.unlock();
    simulate(currentNode);
}

double CuckooBirdSwarm::generateLevyFlight() {
    // Mantegna's algorithm for Lévy flight
    std::normal_distribution<double> normal(0.0, 1.0);
    
    double sigma_u = std::pow(
        std::tgamma(1.0 + levyAlpha) * std::sin(M_PI * levyAlpha / 2.0) /
        (std::tgamma((1.0 + levyAlpha) / 2.0) * levyAlpha * std::pow(2.0, (levyAlpha - 1.0) / 2.0)),
        1.0 / levyAlpha
    );
    
    double u = normal(cb_rng) * sigma_u;
    double v = normal(cb_rng);
    
    double step = u / std::pow(std::abs(v), 1.0 / levyAlpha);
    return std::abs(step);
}

} // namespace Othello