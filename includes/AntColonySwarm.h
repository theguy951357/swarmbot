//
// Created by cblah on 4/14/2022.
//

#ifndef SWARMBOT_ANTCOLONYSWARM_H
#define SWARMBOT_ANTCOLONYSWARM_H

#include <iostream>
#include <omp.h>
#include "GameTree.h"
#include "Evaluate.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <ctime>
#include <omp.h>


using namespace std;



class AntColonySwarm{

public:
    AntColonySwarm(GameTree *gameTree);
    void simulate(GameTreeNode *node);
    void findWinValue();
    void backPropagate(GameTreeNode *node, short playerColor);
private:

    string name = "C AntColonySwarm.cpp";
    GameTree *gameTree;
    GameTreeNode *currentNode;
    short winValue;

};


#endif //SWARMBOT_ANTCOLONYSWARM_H
