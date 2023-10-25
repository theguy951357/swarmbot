//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_GAMETREE_H
#define SWARMBOT_GAMETREE_H
#include "GameTreeNode.h"
#include "OthelloBitBoard.h"
#include "MoveSet.h"
#include <iostream>
#include <omp.h>


class GameTree {
private:
    string name = "C GameTree.cpp";
    GameTreeNode *root;
    MoveSet *moves;
    void printGametree(const std::string& prefix, const GameTreeNode* node, bool isLeft);

public:
    GameTree(OthelloBitBoard *board, MoveSet *moves);

    void printGametree(const GameTreeNode* node);



    GameTreeNode *getRoot() const;

public:
    void scanNode(GameTreeNode *node);

};


#endif //SWARMBOT_GAMETREE_H
