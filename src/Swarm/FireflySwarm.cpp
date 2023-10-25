//
// Created by cblah on 4/14/2022.
//

#include "../../includes/FireflySwarm.h"


/**
 * The firefly swarm will behave close to your regular particle
 * swarm optimization. The agents will dive in a monte carlo style.
 * Their decisions will also be driven by a genetic algorithm (GA).
 * an array of 64 moves plus one for it's win value will determine
 * behavior of where the agent will travel. If the node at the ith level has
 * the same value as the GA at the ith index. that node will have an extra weight
 * giving that node a better chance of being chosen.
 * As the agents progress through the tree each time, their GAs will be merged
 * and create new GAs. This will evolve the behavior and direct the fireflies
 * towards the optimal goal.
 */

default_random_engine mt3(time(nullptr));
//default_random_engine mt3(100);


FireflySwarm::FireflySwarm(GameTree *gameTree) : gameTree(gameTree){
    makeNewGa();
    this->currentNode = nullptr;
    this->winValue = 0;
}

void FireflySwarm::simulate(GameTreeNode *node) {
    //cout<<name<<" simulating"<<endl;
    if (node == nullptr){
        return;
    }
    while (node->isLock()){
        //cout<<name<<"in lock loop"<<endl;
    }

#pragma omp critical
    {
        node->setLock(true);
    }

    simulateHelper(node, 0);
}

void FireflySwarm::simulateHelper(GameTreeNode *node, short count) {
    double GA = 0;
    this->currentNode = node;
    if (currentNode->getState() == NOT_SCANNED && currentNode->getChild()== nullptr){
        currentNode->setState(SCANNING);

        gameTree->scanNode(currentNode);

    }else if (omp_in_parallel() && currentNode->getState()==SCANNING){
        if (currentNode->getSibling()!= nullptr){
            currentNode = currentNode->getSibling();
        }else{
            currentNode=currentNode->getParent();
        }

    }
    uniform_int_distribution<int> dist(0,RAND_MAX);
    double rando = (double )dist(mt3)/RAND_MAX;
    if (currentNode->getChild()!= nullptr){
        currentNode = currentNode->getChild();
    }else{
        node->setLock(false);
        return;
    }
    Move location;
    while(currentNode->getSibling() != nullptr){
        location = currentNode->getLocation();
        if (geneticAlgorithm[count]==Utils::convertToGrid(location.getCol(),location.getRow())){
            GA = geneticAlgorithm[30]/64.0;
            //cout<<name<<" GA added "<<GA<<" to the probability"<<endl;
        }
        rando+=GA/100;
        //cout<<name<<" "<<rando<<endl;
        if (rando < .5f){
            if (currentNode->getSibling()!= nullptr){
                currentNode=currentNode->getSibling();
            }else{
                currentNode = node->getChild();
            }
        }else{
            break;
        }
    }

    node->setLock(false);
    simulateHelper(currentNode, ++count);
}

void FireflySwarm::findWinValue() {
    this->blackCount = 0;
    this->whiteCount = 0;
    uint_fast64_t blackBoard = currentNode->getCurrentBoard()->getBoardBlackPlayer();
    uint_fast64_t whiteBoard = currentNode->getCurrentBoard()->getBoardWhitePlayer();
    for (int i = 7; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            uint_fast64_t location = 1ULL <<(8*i+j);
            if (blackBoard & location) {
                ++blackCount;
            }else if (whiteBoard & location){
                ++whiteCount;
            }
        }
    }
    blackCount > whiteCount ? winValue=BLACK : winValue=WHITE;
}

void FireflySwarm::backPropagate(GameTreeNode *node, short playerColor) {

    playerColor==BLACK?this->geneticAlgorithm[30]=this->blackCount:this->geneticAlgorithm[30]=this->whiteCount;
    short wiAdd = 0;
    if (winValue==playerColor){
        wiAdd = 1;
    }
    if (currentNode->getNi()!=1){//if node hasn't been checked yet.
        while(currentNode != nullptr){
            currentNode->setNi(currentNode->getNi()+1);
            currentNode->setWi(currentNode->getWi()+wiAdd);
            currentNode = currentNode->getParent();
        }
    }
}

void FireflySwarm::makeNewGa() {
    //cout<<name<< " debug GA creation"<<endl;
    for (int i = 0; i < 30; ++i) {

        uniform_int_distribution<int> dist(0,RAND_MAX);
        geneticAlgorithm[i] = dist(mt3)%65;
    }
    geneticAlgorithm[30]=0;
    //cout<<name<< " debug GA creation done"<<endl;

}

short *FireflySwarm::getGeneticAlgorithm() const {
    return geneticAlgorithm;
}

void FireflySwarm::setGeneticAlgorithm(short *geneticAlgorithm) {
    FireflySwarm::geneticAlgorithm = geneticAlgorithm;
}








