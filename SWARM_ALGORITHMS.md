# Swarm Intelligence Algorithms

This document explains how each swarm algorithm works and how they differ from each other.

## 🎯 Overview

All swarm algorithms inherit from `SwarmAgent` and implement the Monte Carlo Tree Search (MCTS) framework, but each uses a different strategy for selecting which nodes to explore.

## 🐜 Ant Colony Swarm

**Inspiration**: Ant foraging behavior using pheromone trails

**How it works**:
1. Simulated "pheromones" are represented by win rates
2. Nodes with higher win rates are more attractive (stronger pheromone)
3. Uses probabilistic selection based on pheromone strength
4. Balances exploration (random) vs exploitation (follow strong trails)

**Key Parameters**:
- `pheromoneInfluence = 1.0` - How much weight to give to pheromone trails
- `pheromoneDecay = 0.9` - (Future) Rate at which old pheromones fade

**Selection Strategy**:
```cpp
double pheromone = pow(winRate, pheromoneInfluence);
// Select node with probability proportional to pheromone
```

**Best for**: 
- Exploitation of known good moves
- Reinforcement learning-like behavior
- Converging to optimal solutions

## 🔥 Firefly Swarm

**Inspiration**: Firefly mating behavior using bioluminescence

**How it works**:
1. Each node emits "light" based on its win rate
2. Light intensity decreases with "distance" (number of simulations)
3. Fireflies are attracted to brighter lights
4. This creates a balance between exploring new areas (dim lights) and exploiting good positions (bright lights)

**Key Parameters**:
- `attractiveness = 1.0` - Base attractiveness coefficient
- `lightAbsorption = 0.1` - How quickly light fades with distance

**Selection Strategy**:
```cpp
double distance = 1.0 / (1.0 + simulations);
double intensity = attractiveness * winRate * exp(-lightAbsorption * distance);
// Select probabilistically based on intensity
```

**Best for**:
- Balancing exploration and exploitation
- Avoiding premature convergence
- Finding globally optimal solutions

## 🐦 Cuckoo Bird Swarm

**Inspiration**: Cuckoo bird brood parasitism and Lévy flight patterns

**How it works**:
1. Uses Lévy flights for move selection (occasional long jumps)
2. Abandons poor "nests" (nodes with bad win rates) with probability `pa`
3. Performs local search (short steps) most of the time
4. Occasionally makes large jumps (Lévy flights) for global exploration

**Key Parameters**:
- `levyAlpha = 1.5` - Lévy flight exponent (controls step size distribution)
- `abandonmentRate = 0.25` - Probability of abandoning a poor nest

**Selection Strategy**:
```cpp
if (random() < abandonmentRate) {
    // Abandon poor nest, jump to best known node
    currentNode = bestPerformingNode;
} else {
    // Lévy flight: skip some siblings based on heavy-tailed distribution
    int steps = levyFlight() * numSiblings;
    skip(steps);
}
```

**Best for**:
- Escaping local optima
- Global exploration
- Handling rugged fitness landscapes

## 🎲 Monte Carlo Agent (Base)

**Inspiration**: Pure random sampling

**How it works**:
1. Randomly select child nodes
2. No bias toward good or bad positions
3. Pure exploration

**Selection Strategy**:
```cpp
currentNode = child;
while (sibling && random() < 0.5) {
    currentNode = sibling;
}
```

**Best for**:
- Baseline comparison
- Unbiased exploration
- Simple MCTS implementation

## 📊 Algorithm Comparison

| Algorithm | Exploration | Exploitation | Convergence Speed | Best Use Case |
|-----------|-------------|--------------|-------------------|---------------|
| Monte Carlo | High | None | Slow | Baseline/testing |
| Ant Colony | Medium | High | Fast | Known good positions |
| Firefly | High | Medium | Medium | Balanced search |
| Cuckoo Bird | Very High | Medium | Variable | Complex landscapes |

## 🔬 Technical Details

### Ant Colony Optimization
**Mathematical Model**:
```
P(node_i) = (pheromone_i)^α / Σ(pheromone_j)^α
```
Where α is the pheromone influence parameter.

### Firefly Algorithm
**Mathematical Model**:
```
Intensity_i = β₀ * winRate_i * e^(-γ * distance²)
```
Where:
- β₀ = base attractiveness
- γ = light absorption coefficient
- distance = 1/(1 + simulations)

### Cuckoo Search
**Mathematical Model**:
```
Lévy(α) = u / |v|^(1/α)
```
Where u, v are normally distributed random variables, and α ∈ (1, 2] controls the distribution.

## 🎮 Configuration

### Using a Single Swarm Type

Edit `Agent.cpp::initializeSwarms()`:

```cpp
// Use only Ant Colony
for (int i = 0; i < swarmSize; ++i) {
    swarms.push_back(std::make_unique<AntColonySwarm>(gameTree));
}
```

### Using Mixed Swarms

```cpp
// Use all four types equally
int perType = swarmSize / 4;
for (int i = 0; i < perType; ++i) {
    swarms.push_back(std::make_unique<MonteCarloAgent>(gameTree));
}
for (int i = 0; i < perType; ++i) {
    swarms.push_back(std::make_unique<AntColonySwarm>(gameTree));
}
for (int i = 0; i < perType; ++i) {
    swarms.push_back(std::make_unique<FireflySwarm>(gameTree));
}
for (int i = 0; i < perType; ++i) {
    swarms.push_back(std::make_unique<CuckooBirdSwarm>(gameTree));
}
```

### Custom Swarm Distribution

```cpp
// 50% Ant Colony, 30% Firefly, 20% Cuckoo
int ac_count = swarmSize / 2;
int ff_count = swarmSize * 3 / 10;
int cb_count = swarmSize - ac_count - ff_count;

for (int i = 0; i < ac_count; ++i) {
    swarms.push_back(std::make_unique<AntColonySwarm>(gameTree));
}
for (int i = 0; i < ff_count; ++i) {
    swarms.push_back(std::make_unique<FireflySwarm>(gameTree));
}
for (int i = 0; i < cb_count; ++i) {
    swarms.push_back(std::make_unique<CuckooBirdSwarm>(gameTree));
}
```

## 🧪 Experimental Results (Hypothetical)

Based on swarm intelligence literature, you might expect:

### Opening Game (Moves 1-15)
- **Cuckoo Bird**: Best (high exploration needed)
- **Firefly**: Good (balanced approach)
- **Ant Colony**: Medium (premature convergence risk)
- **Monte Carlo**: Slowest (pure random)

### Mid Game (Moves 16-40)
- **Ant Colony**: Best (exploitation of known positions)
- **Firefly**: Good (adaptive search)
- **Cuckoo Bird**: Good (occasional breakthroughs)
- **Monte Carlo**: Slowest

### End Game (Moves 41-60)
- **Ant Colony**: Best (converge to optimal)
- **Monte Carlo**: Good (less complex positions)
- **Firefly**: Medium
- **Cuckoo Bird**: Medium (too much exploration)

## 🔧 Tuning Parameters

### Ant Colony
```cpp
pheromoneInfluence = 1.5;  // Higher = more exploitation
pheromoneDecay = 0.95;     // Higher = slower forgetting
```

### Firefly
```cpp
attractiveness = 1.2;      // Higher = stronger attraction
lightAbsorption = 0.05;    // Lower = light travels further
```

### Cuckoo Bird
```cpp
levyAlpha = 1.8;           // Higher = shorter jumps
abandonmentRate = 0.15;    // Lower = less abandonment
```

## 📚 References

1. Dorigo, M., & Stützle, T. (2004). Ant Colony Optimization. MIT Press.
2. Yang, X. S. (2008). Nature-Inspired Metaheuristic Algorithms. Luniver Press.
3. Yang, X. S., & Deb, S. (2009). Cuckoo Search via Lévy Flights. Proc. of World Congress on Nature & Biologically Inspired Computing.

## 💡 Tips for Best Performance

1. **For faster AI**: Use pure Ant Colony (exploits known good moves)
2. **For stronger AI**: Mix 70% Firefly + 30% Cuckoo Bird
3. **For research**: Use all four equally to compare behaviors
4. **For specific positions**: Ant Colony in endgame, Cuckoo in opening

## 🎯 Summary

Each swarm algorithm offers different tradeoffs:
- **Ant Colony**: Fast convergence, exploitation-heavy
- **Firefly**: Balanced, adaptive search
- **Cuckoo Bird**: High exploration, escapes local optima
- **Monte Carlo**: Unbiased baseline

Choose based on your needs: speed (Ant Colony), strength (Firefly/Cuckoo mix), or research (all four).