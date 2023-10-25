//
// Created by cblah on 3/4/2022.
//

#include "../../includes/Move.h"
/**
 * This Move node will just have a simple row and column variable.
 * These moves will be pointed to by players and the Gametree.
 * The constructor with no parameter will initalize the move at P,0.
 * These will indicate pass moves if not changed.
 */

Move::Move() {
    this->col = 'P';
    this->row = 0;
}

/**
 * constructor will take the column and row parameters
 *
 * @param col
 * @param row
 */
Move::Move(char col, short row) : col(col), row(row) {}

/**
 * Getter for column
 * @return column
 */
char Move::getCol() const {
    return col;
}

/**
 * Getter for row
 * @return row
 */
short Move::getRow() const {
    return row;
}

/**
 * print function to print out the column and row of a given move.
 */
void Move::printMove() {
    cout<<this->col<<" "<<this->row;

}
