//
// Created by cblah on 4/14/2022.
//

#ifndef SWARMBOT_FIREFLYSWARM_H
#define SWARMBOT_FIREFLYSWARM_H

#include "GameTree.h"
#include "Evaluate.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <ctime>
#include <omp.h>


using namespace std;
class FireflySwarm {
public:
    short *getGeneticAlgorithm() const;

    void setGeneticAlgorithm(short *geneticAlgorithm);

    explicit FireflySwarm(GameTree *gameTree);
    void simulate(GameTreeNode *node);
    void simulateHelper(GameTreeNode *node, short count);
    void findWinValue();
    void backPropagate(GameTreeNode *node, short playerColor);
    void makeNewGa();


private:
    string name = "C FireflySwarm.cpp";
    GameTree *gameTree;
    short geneticAlgorithmArray[31];
    short *geneticAlgorithm = geneticAlgorithmArray;
    short blackCount;
    short whiteCount;
    GameTreeNode *currentNode;
    short winValue;

};


#endif //SWARMBOT_FIREFLYSWARM_H
