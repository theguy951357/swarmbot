//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_CONSTANTS_H
#define SWARMBOT_CONSTANTS_H

#include <cstdint>
#include <omp.h>

//constant for pass
static const short PASS = 27;

//swarm.
static const short MAX_SWARM_SIZE = omp_get_num_procs();
static const short NUMBER_OF_DIVES = 20000;

//middle and endgame
static const short MID_GAME =20;
static const short END_GAME = 40;

//count for tree printing
static const short COUNT = 10;

//constant for lastMovePlayed starting point
static const short NO_MOVE_MADE_YET = -1;

//players
static const short BLACK =  1;
static const short WHITE = -1;

//state
static const short NOT_SCANNED = 0;
static const short SCANNING = 1;
static const short SCANNED = -1;

//directions
static const short DIR[4] = {
                             8,//down and up
                             1,//right and left
                             9,//downright and upleft
                             7,//downleft and upright
                             };

//board heuristic
static short BOARD_HEURISTIC[64] = {
        //   a    b    c    d    e    f    g    h

        100, -64,  20,   10,   10,   20,  -64,  100,
        -64, -64, -32, -32, -32, -32, -64, -64,
        20,-32,  10,  10,  10,  10, -32,  20,
        10,-32,  10, 000, 000,  10, -32,  10,
        10,-32,  10, 000, 000,  10, -32,  10,
        20,-32,  10,  10,  10,  10, -32,  20,
        -64,-64, -32, -32, -32, -32, -64, -64,
        100,-64,  20,  10,  10,  20, -64, 100,

};


//bitboard constants
static const uint_fast64_t INITIAL_WHITE_OTHELLO_BITBOARD = 0b0000000000000000000000000001000000001000000000000000000000000000ULL;
static const uint_fast64_t INITIAL_BLACK_OTHELLO_BITBOARD = 0b0000000000000000000000000000100000010000000000000000000000000000ULL;
static const uint_fast64_t FULL_OTHELLO_BITBOARD          = 0b1111111111111111111111111111111111111111111111111111111111111111ULL;
static const uint_fast64_t EMPTY_OTHELLO_BITBOARD         = 0b0000000000000000000000000000000000000000000000000000000000000000ULL;
static const uint_fast64_t BITBOARD_ALL_SIDES_MASK        = 0b0000000001111110011111100111111001111110011111100111111000000000ULL;
static const uint_fast64_t BITBOARD_VERTICAL_MASK         = 0b0000000011111111111111111111111111111111111111111111111100000000ULL;
static const uint_fast64_t BITBOARD_HORIZONTAL_MASK       = 0b0111111001111110011111100111111001111110011111100111111001111110ULL;
static const uint_fast64_t BITBOARD_EAST_MASK             = 0b1111111011111110111111101111111011111110111111101111111011111110ULL;
static const uint_fast64_t BITBOARD_WEST_MASK             = 0b0111111101111111011111110111111101111111011111110111111101111111ULL;


#endif //SWARMBOT_CONSTANTS_H
