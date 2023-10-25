//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_EVALUATE_H
#define SWARMBOT_EVALUATE_H
#include "GameTreeNode.h"
#include "Utils.h"
#include "Constants.h"
#include <cmath>
#include <iostream>
#include <random>
#include <ctime>


using namespace std;

class Evaluate {
private:

    short board_heuristic[64] = {
            //   a    b    c    d    e    f    g    h

              100, -64,  20,   10,   10,   20,  -64,  100,
              -64, -64, -32, -32, -32, -32, -64, -64,
              20,-32,  10,  10,  10,  10, -32,  20,
              10,-32,  10, 000, 000,  10, -32,  10,
              10,-32,  10, 000, 000,  10, -32,  10,
              20,-32,  10,  10,  10,  10, -32,  20,
             -64,-64, -32, -32, -32, -32, -64, -64,
             100,-64,  20,  10,  10,  20, -64, 100,

    };

public:
    static void exploit(GameTreeNode *node);
    static void mutate(short *one, short *two);
    string name = "C Evaluate.cpp";

};


#endif //SWARMBOT_EVALUATE_H
