//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_AGENT_H
#define SWARMBOT_AGENT_H


#include "Player.h"
#include "GameTree.h"
#include "GameTreeNode.h"
#include "MonteCarloAgent.h"
#include "AntColonySwarm.h"
#include "FireflySwarm.h"
#include "CuckooBirdSwarm.h"
#include "Evaluate.h"
#include "Constants.h"

#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <numeric>

class Agent : public Player{
public:
    Agent(OthelloGame *game, GameTree *gameTree);
    void decide();
    void printTreeToTxt(GameTreeNode *node);
    void printAverageTimeToFile();

private:
    string name = "C Agent.cpp";
    vector<double> diveTimes;
    int treeCounter;
    GameTree *gameTree;
    //MonteCarloAgent *monteCarloAgent;
    vector<MonteCarloAgent> mcswarm;
    vector<AntColonySwarm> acswarm;
    vector<FireflySwarm> ffswarm;
    vector<CuckooBirdSwarm> cbswarm;
    GameTreeNode *bookmark;
    void agentAddToStrategy();
    void moveBookmark();
    GameTreeNode *analyze();
    void monteCarloParallel(GameTreeNode *node);
    void printTreeToTxtHelper(GameTreeNode *node, int level, int parentCount, ofstream *outfile);

};


#endif //SWARMBOT_AGENT_H
