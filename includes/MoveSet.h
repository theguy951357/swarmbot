//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_MOVESET_H
#define SWARMBOT_MOVESET_H
#include <map>
#include "Move.h"
#include <iostream>

using namespace std;

class MoveSet {
public:
    MoveSet();

    Move* getMove(short location);

private:
    string name = "C MoveSet.cpp";
    map<short, Move> moves;

};


#endif //SWARMBOT_MOVESET_H
