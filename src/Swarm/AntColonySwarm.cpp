//
// Created by cblah on 4/14/2022.
//

#include "../../includes/AntColonySwarm.h"

default_random_engine mt2(time(nullptr));


AntColonySwarm::AntColonySwarm(GameTree *gameTree): gameTree(gameTree)  {
    this->currentNode = nullptr;
    this->winValue = 0;
}


/**
 * This method will simulate a game on the gameTree.
 * It will choose moves at random down to a leaf node
 * or end of game.
 * @param node
 */
void AntColonySwarm::simulate(GameTreeNode *node) {


     // TODO Ant colony works. however, I have been able to beat it multiple times.
     // This suggests maybe it might be honing in on local maxima.
     // May need to find a way to get other ants to explore more
     // to help hone in on more global maxima in terms of finding
     // the optimal path to a win.


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
    double pheromone = 0.0f;
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
    double rando = (double )dist(mt2)/RAND_MAX;
    if (currentNode->getChild()!= nullptr){
        currentNode = currentNode->getChild();
    }else{
        node->setLock(false);
        return;
    }

    GameTreeNode *bestNode = nullptr;

    while(currentNode->getSibling() != nullptr){
        //this if statement will determine the pheromone trail that the ant colony will travel on.
        if (currentNode->getNi()!=0){

            pheromone = (double)currentNode->getWi()/currentNode->getNi();
        }
        rando+=pheromone;
        //cout<<name<<" pheromone probability is: "<<rando<<endl;


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
    simulate(currentNode);
}

void AntColonySwarm::findWinValue() {
    //cout<<"C finding the winner."<<endl;
    short blackCount = 0;
    short whiteCount = 0;
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
//    cout<<"C black/white count: "<<blackCount<<"/"<<whiteCount<<". ";
//    Utils::printPlayerColor(winValue);
//    cout<<"wins the simulation!"<<endl;
}

/**
 * The backPropagate function will send the MonteCarloAgent back up the tree to the root.
 * All the parent nodes in the path will have their wi and ni updated as the agent passes
 * that node.
 * @param node
 * @param playerColor
 */
void AntColonySwarm::backPropagate(GameTreeNode *node, short playerColor) {
    //cout<<"C back propagating."<<endl;
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




