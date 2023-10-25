//
// Created by cblah on 4/14/2022.
//

#include "../../includes/CuckooBirdSwarm.h"

default_random_engine mt4(time(nullptr));
//default_random_engine mt4(100);


CuckooBirdSwarm::CuckooBirdSwarm(GameTree *gameTree) :  gameTree(gameTree){
    makeNewGa();
    this->currentNode = nullptr;
    this->winValue = 0;
    this->caught = false;
    this->caughtCount = 0;
    this->destroyedCount = 0;
    this->blackCount = 0;
    this->whiteCount = 0;
}

void CuckooBirdSwarm::simulate(GameTreeNode *node) {
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

void CuckooBirdSwarm::simulateHelper(GameTreeNode *node, short count) {

    double GA = 0;
    this->currentNode = node;
    if (currentNode->getState() == NOT_SCANNED && currentNode->getChild() == nullptr) {
        currentNode->setState(SCANNING);

        gameTree->scanNode(currentNode);

    } else if (omp_in_parallel() && currentNode->getState() == SCANNING) {
        if (currentNode->getSibling() != nullptr) {
            currentNode = currentNode->getSibling();
        } else {
            currentNode = currentNode->getParent();
        }

    }
    uniform_int_distribution<int> dist(0, RAND_MAX);
    double rando = (double) dist(mt4) / RAND_MAX;
    if (currentNode->getChild() != nullptr) {
        currentNode = currentNode->getChild();
    } else {
        node->setLock(false);
        return;
    }
    while (currentNode->getSibling() != nullptr) {
        Move location = currentNode->getLocation();
        if (geneticAlgorithm[count / 2] == Utils::convertToGrid(location.getCol(), location.getRow())) {
            GA = geneticAlgorithm[30] / 64.0;

        }
        rando += GA / 100.0;
        if (rando < .5f) {
            if (currentNode->getSibling() != nullptr) {
                currentNode = currentNode->getSibling();
            } else {
                currentNode = node->getChild();
            }
        } else {
            break;
        }
    }
    node->setLock(false);
    simulateHelper(currentNode, ++count);


}

void CuckooBirdSwarm::findWinValue() {
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

void CuckooBirdSwarm::backPropagate(GameTreeNode *node, short playerColor) {
    playerColor==BLACK?this->geneticAlgorithm[30]=this->blackCount:this->geneticAlgorithm[30]=this->whiteCount;
    uniform_int_distribution<int> dist(0,RAND_MAX);
    double rando = (double )dist(mt4)/RAND_MAX;
    rando+= this->geneticAlgorithm[30]/64;
//    if (rando < .5){
//        this->caught=true;
//    }
    if (rando<.5){
        //cout<<name << " cuckoo bird got caught."<<endl;
        ++caughtCount;
        rando = (double )dist(mt4)/RAND_MAX;
        rando+= this->geneticAlgorithm[30]/64;
        short gaThing;
        if (rando < .5){
            //cout<< name<< " cuckoo bird was destroyed."<<endl;
            ++destroyedCount;
            //cuckoo bird swarm couldn't handle an inherited genetic algorithm from montecarloagent.
            // It was causing segfaults, so it has its own GA. I wonder if this is also affecting
            // the illegal moves during simulation???
            for (int i = 0; i < 30; ++i) {

                uniform_int_distribution<int> dist(0,RAND_MAX);
                geneticAlgorithm[i] = dist(mt4)%65;
            }
            geneticAlgorithm[30]=0;
        }
    }
    //this->caught = false;
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

short *CuckooBirdSwarm::getGeneticAlgorithm() const {
    return geneticAlgorithm;
}

void CuckooBirdSwarm::setGeneticAlgorithm(short *geneticAlgorithm) {
    CuckooBirdSwarm::geneticAlgorithm = geneticAlgorithm;
}

void CuckooBirdSwarm::makeNewGa() {
    //cout<<name<< " debug GA creation"<<endl;
    for (int i = 0; i < 30; ++i) {

        uniform_int_distribution<int> dist(0,RAND_MAX);
        geneticAlgorithm[i] = dist(mt4)%65;
    }
    geneticAlgorithm[30]=0;
    //cout<<name<< " debug GA creation done"<<endl;

}

