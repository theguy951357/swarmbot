//
// Created by cblah on 5/5/2022.
//

#ifndef SWARMBOT_GAMETREENODE_H
#define SWARMBOT_GAMETREENODE_H

#include "OthelloBitBoard.h"
#include "Constants.h"
#include "Utils.h"
#include "Move.h"
#include <iostream>
#include <omp.h>
using namespace std;

class GameTreeNode {

private:
    string name = "C GameTreeNode.cpp";
    Move location;
    short state;
    short color;
    double rank; //rank of node after exploitation
    int wi; //number of wins found after the ith move.
    int ni; //number of simulations made after ith move.
    OthelloBitBoard *currentBoard; // the state of the board at the ith move
    GameTreeNode *parent;
    GameTreeNode *child; //first move.
    GameTreeNode *sibling; //the rest of the moves.
    bool lock;

public:
    GameTreeNode(OthelloBitBoard *currentBoard);

    const Move &getLocation() const;

    void setLocation(const Move &location);

    short getState() const;

    void setState(short state);

    short getColor() const;

    void setColor(short color);

    double getRank() const;

    void setRank(double rank);

    int getWi() const;

    void setWi(int wi);

    int getNi() const;

    void setNi(int ni);

    OthelloBitBoard *getCurrentBoard() const;

    void setCurrentBoard(OthelloBitBoard *currentBoard);

    GameTreeNode *getParent() const;

    void setParent(GameTreeNode *parent);

    GameTreeNode *getChild() const;

    void setChild(GameTreeNode *child);

    void setChild(GameTreeNode *child, GameTreeNode *parent, Move *move);

    GameTreeNode *getSibling() const;

    void setSibling(GameTreeNode *sibling);

    void setSibling(GameTreeNode *sibling, GameTreeNode *upperSibling, Move *move);

    bool isLock() const;

    void setLock(bool lock);
};


#endif //SWARMBOT_GAMETREENODE_H
