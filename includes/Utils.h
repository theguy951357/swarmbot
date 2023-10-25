//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_UTILS_H
#define SWARMBOT_UTILS_H

#include <iostream>
#include "Constants.h"



using namespace std;

class Utils {
private:
    string name = "C Utils.cpp";


public:
    static short convertToGrid(short col, short row);
    
    static void printGridLocation(short location);

    static void printPlayerColor(short color);




};


#endif //SWARMBOT_UTILS_H
