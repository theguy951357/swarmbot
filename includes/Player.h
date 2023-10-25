//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_PLAYER_H
#define SWARMBOT_PLAYER_H
#include "MoveSet.h"
#include "Utils.h"
#include "Constants.h"
#include "OthelloGame.h"

#include <iostream>
#include <stack>
#include <vector>

using namespace std;

class Player {
private:
    string name = "C Player.cpp";

public:
    Player(OthelloGame *game);
    void getLegalMoves();
    void printMoves();
    void printStrategy();
    void makeMove();
    void addToStrategy();

protected:
    uint_fast64_t legalBoard;
    short color;
public:
    short getColor() const;

protected:
    char playerColor;
    vector<Move*>::iterator it;
    MoveSet *moves;
    stack<Move*> strategy;
    vector<Move*> legalMoves;
    OthelloGame *game;
    void printStrategyHelper();
};


#endif //SWARMBOT_PLAYER_H
