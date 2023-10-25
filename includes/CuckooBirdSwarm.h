//
// Created by cblah on 4/14/2022.
//

#ifndef SWARMBOT_CUCKOOBIRDSWARM_H
#define SWARMBOT_CUCKOOBIRDSWARM_H

#include "MonteCarloAgent.h"
#include "GameTree.h"
#include "Evaluate.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <ctime>
#include <omp.h>

#include <iostream>
using namespace std;
class CuckooBirdSwarm{

public:
    explicit CuckooBirdSwarm(GameTree *gameTree);
    void simulate(GameTreeNode *node);
    void simulateHelper(GameTreeNode *node, short count);
    void findWinValue();
    void backPropagate(GameTreeNode *node, short playerColor);
    void makeNewGa();
private:
    string name = "C CuckooBirdSwarm.cpp";
    short ccGeneticAlgorithmArray[31];
    short *geneticAlgorithm = ccGeneticAlgorithmArray;
    short blackCount;
    short whiteCount;
    bool caught;
    short caughtCount;
    short destroyedCount;
    GameTreeNode *currentNode;
    short winValue;
    GameTree *gameTree;
public:
    short *getGeneticAlgorithm() const;

    void setGeneticAlgorithm(short *geneticAlgorithm);
};


#endif //SWARMBOT_CUCKOOBIRDSWARM_H
