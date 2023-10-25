//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_MONTECARLOAGENT_H
#define SWARMBOT_MONTECARLOAGENT_H
#include "GameTree.h"
#include "Evaluate.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <ctime>
#include <omp.h>

class MonteCarloAgent {
private:
    string name = "C MonteCarloAgent.cpp";
private:
    GameTree *gameTree;
    short geneticAlgorithmArray[31];
    short *geneticAlgorithm = geneticAlgorithmArray;

public:
    short *getGeneticAlgorithm() const;

    void setGeneticAlgorithm(short *geneticAlgorithm);

    void makeNewGa();

private:

    GameTreeNode *currentNode;
    short winValue;

public:
    MonteCarloAgent(GameTree *gameTree);

    virtual void simulate(GameTreeNode *node);
    virtual void findWinValue();
    virtual void backPropagate(GameTreeNode *node, short playerColor);
};


#endif //SWARMBOT_MONTECARLOAGENT_H
