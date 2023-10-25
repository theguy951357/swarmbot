//
// Created by cblah on 3/4/2022.
//

#include "../../includes/MoveSet.h"

/**
 * Constructor is going to make a map.
 * The key is going to be
 * the number needed to shift a putted bitboard to the right location
 * for a move
 * The value is going to be a Move node that can be pointed to by a player.
 * The players will be able to create a stack of pointers to these move nodes.
 * That will let the program keep track of each player's strategy.
 * The map needs to be 0 indexed to match up with the bitboard locations
 */

MoveSet::MoveSet() {
    short row;
    char col;
    short num;
    for (short i = 63; i >= 0; --i) {
        short temp = i;
        if (temp==27||temp==28||temp==35||temp==36){
            moves[i] = Move();//The four starting squares can be used as pass moves.
        }else{
            num = temp%8;
            row = temp/8+1;
            col = num+97;
            moves[i] = Move(col, row);
        }
    }
}
/**
 * This getter will return the address of the inputted location.
 *
 * @param location This is the 0 indexed location from the putted bitboard.
 * @return Returns the address of the move needed.
 */
Move* MoveSet::getMove(short location) {
    return &this->moves.at(location);
}




