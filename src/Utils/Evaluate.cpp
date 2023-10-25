//
// Created by cblah on 3/4/2022.
//


#include "../../includes/Evaluate.h"

default_random_engine mteval(time(nullptr));
/**
 * this exploit algorithm is taken from
 * Chang, Hyeong Soo; Fu, Michael C.; Hu, Jiaqiao; Marcus, Steven I. (2005).
 * "An Adaptive Sampling Algorithm for Solving Markov Decision Processes"
 * (PDF). Operations Research. 53: 126–139. doi:10.1287/opre.1040.0145. hdl:1903/6264.
 * on wikipedia.
 *
 * @param node
 */

void Evaluate::exploit(GameTreeNode *node) {
    double wi = node->getWi();
    double ni = node->getNi();
    double Ni = node->getParent()->getNi();
    int b = BOARD_HEURISTIC[Utils::convertToGrid(node->getLocation().getCol(),node->getLocation().getRow())];
    double c = sqrt(2);
    double result = (wi/ni)+(c * sqrt(log(Ni)/ni))+(b/ni);
    node->setRank(result);
}
/**
 * this mutate algorithm will take two Genetic algorithms as parameters,
 * go through the moveset in the array,
 * and randomly pick out one to succeed for the next algorithm.
 * @param one
 * @param two
 * @return
 */
void Evaluate::mutate(short *one, short *two) {
    short *higher;
    short *lower;
    if(one[30]>=two[30]){
        two[30]=one[30];
        higher=one;
        lower=two;
    }else{
        one[30]=two[30];
        higher=two;
        lower=one;
    }
    uniform_int_distribution<int> dist(0,RAND_MAX);
    for (int i = 0; i < 30; ++i) {
        double rando = (double )dist(mteval)/RAND_MAX+higher[30]/64;
        rando>.5?lower[i]=higher[i]:higher[i]=lower[i];
    }
}

