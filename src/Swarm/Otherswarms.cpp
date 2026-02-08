#include "../includes/OtherSwarms.h"

namespace Othello {

FireflySwarm::FireflySwarm(std::shared_ptr<GameTree> gameTree)
    : MonteCarloAgent(gameTree) {
}

CuckooBirdSwarm::CuckooBirdSwarm(std::shared_ptr<GameTree> gameTree)
    : MonteCarloAgent(gameTree) {
}

} // namespace Othello