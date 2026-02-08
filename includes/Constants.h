//
// Created by cblah on 3/4/2022.
//

#pragma once

#include <cstdint>
#include <array>
#include <omp.h>

namespace Othello {

// ============================================================================
// PLAYER REPRESENTATION
// ============================================================================

enum class Player : int16_t {
    WHITE = -1,
    BLACK = 1
};

// ============================================================================
// GAME PHASES
// ============================================================================

namespace GamePhase {
    constexpr int16_t MID_GAME = 20;
    constexpr int16_t END_GAME = 40;
}

// ============================================================================
// TREE NODE STATES
// ============================================================================

enum class ScanState : int16_t {
    NOT_SCANNED = 0,
    SCANNING = 1,
    SCANNED = -1
};

// ============================================================================
// SPECIAL POSITIONS
// ============================================================================

namespace Position {
    constexpr int16_t PASS = 27;
    constexpr int16_t NO_MOVE_MADE_YET = -1;
}

// ============================================================================
// SWARM CONFIGURATION
// ============================================================================

namespace SwarmConfig {
    // Runtime function - cannot be constexpr
    inline int getMaxSwarmSize() {
        return omp_get_num_procs();
    }
    
    constexpr int NUMBER_OF_DIVES = 20000;
}

// ============================================================================
// BITBOARD DIRECTIONS
// ============================================================================

namespace Direction {
    constexpr int16_t NORTH = 8;
    constexpr int16_t SOUTH = -8;
    constexpr int16_t EAST = -1;
    constexpr int16_t WEST = 1;
    constexpr int16_t NORTH_EAST = 7;
    constexpr int16_t NORTH_WEST = 9;
    constexpr int16_t SOUTH_EAST = -9;
    constexpr int16_t SOUTH_WEST = -7;
    
    constexpr std::array<int16_t, 4> BASE_OFFSETS = {8, 1, 9, 7};
}

// ============================================================================
// BOARD EVALUATION HEURISTIC
// ============================================================================

namespace Heuristic {
    constexpr std::array<int16_t, 64> BOARD_VALUES = {
        // Row 1 (a-h)
        100, -64,  20,  10,  10,  20, -64, 100,
        // Row 2
        -64, -64, -32, -32, -32, -32, -64, -64,
        // Row 3
         20, -32,  10,  10,  10,  10, -32,  20,
        // Row 4
         10, -32,  10,   0,   0,  10, -32,  10,
        // Row 5
         10, -32,  10,   0,   0,  10, -32,  10,
        // Row 6
         20, -32,  10,  10,  10,  10, -32,  20,
        // Row 7
        -64, -64, -32, -32, -32, -32, -64, -64,
        // Row 8
        100, -64,  20,  10,  10,  20, -64, 100
    };
}

// ============================================================================
// BITBOARD MASKS
// ============================================================================

namespace BitMask {
    constexpr uint64_t INITIAL_WHITE = 0x0000000810000000ULL;
    constexpr uint64_t INITIAL_BLACK = 0x0000001008000000ULL;
    constexpr uint64_t FULL_BOARD    = 0xFFFFFFFFFFFFFFFFULL;
    constexpr uint64_t EMPTY_BOARD   = 0x0000000000000000ULL;
    constexpr uint64_t ALL_SIDES     = 0x007E7E7E7E7E7E00ULL;
    constexpr uint64_t VERTICAL      = 0x00FFFFFFFFFFFF00ULL;
    constexpr uint64_t HORIZONTAL    = 0x7E7E7E7E7E7E7E7EULL;
    constexpr uint64_t EAST          = 0xFEFEFEFEFEFEFEFEULL;
    constexpr uint64_t WEST          = 0x7F7F7F7F7F7F7F7FULL;
}

// ============================================================================
// DISPLAY CONFIGURATION
// ============================================================================

namespace Display {
    constexpr int TREE_INDENT = 10;
}

} // namespace Othello