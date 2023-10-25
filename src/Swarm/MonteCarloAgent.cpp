//
// Created by cblah on 3/4/2022.
//

#include "../../includes/MonteCarloAgent.h"
default_random_engine mt(time(nullptr));
//default_random_engine mt(100);

/**
 * This MonteCarlo agent will be the parent class for all the swarm agents.
 * The Agent() class will send down N number of swarm agents that will
 * inherit the MonteCarloAgent as they will all dive in the same fashion.
 * This class should be able to function on its own as a regular MCTS.
 *
 * The constructor is going to take in the GameTree being acted on.
 * It will also set the current node to nullptr and the winvalue to 0.
 *
 */


MonteCarloAgent::MonteCarloAgent(GameTree *gameTree) : gameTree(gameTree) {
    this->currentNode = nullptr;
    this->winValue = 0;

    //TODO try setting the other swarms out of the MonteCarloAgent's inheritance so they are not children.
    // this may be causing the bug.

}
/**
 * This method will simulate a game on the gameTree.
 * It will choose moves at random down to a leaf node
 * or end of game.
 * @param node
 */
void MonteCarloAgent::simulate(GameTreeNode *node) {
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
    this->currentNode = node;
    if (currentNode->getState() == NOT_SCANNED) {
        currentNode->setState(SCANNING);
        gameTree->scanNode(currentNode);
    }else if (omp_in_parallel() && currentNode->getState() == SCANNING) {
        currentNode = currentNode->getParent();
    }

    uniform_int_distribution<int> dist(0,RAND_MAX);
    double rando = (double )dist(mt)/RAND_MAX;
    if (currentNode->getChild()!= nullptr){
        currentNode = currentNode->getChild();
    }else{
        node->setLock(false);
        return;
    }
    while(currentNode->getSibling() != nullptr){
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
/**
 * This method will evaluate the leaf node and determine who the winner is.
 */
void MonteCarloAgent::findWinValue() {
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
void MonteCarloAgent::backPropagate(GameTreeNode *node, short playerColor) {
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

short *MonteCarloAgent::getGeneticAlgorithm() const {
    return geneticAlgorithm;
}

void MonteCarloAgent::setGeneticAlgorithm(short *geneticAlgorithm) {
    MonteCarloAgent::geneticAlgorithm = geneticAlgorithm;
}

void MonteCarloAgent::makeNewGa() {
    //cout<<name<< " debug GA creation"<<endl;
    for (int i = 0; i < 31; ++i) {

        uniform_int_distribution<int> dist(0,RAND_MAX);
        geneticAlgorithm[i] = dist(mt)%65;
    }
    geneticAlgorithm[31]=0;
    //cout<<name<< " debug GA creation done"<<endl;

}









