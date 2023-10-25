//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_MOVE_H
#define SWARMBOT_MOVE_H

#include <iostream>

using namespace std;
class Move {

public:
    Move();

    Move(char col, short row);

    char getCol() const;

    short getRow() const;

    void printMove();

private:
    string name = "C Move.cpp";
    char col;
    short row;
};


#endif //SWARMBOT_MOVE_H
