//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_OTHELLOGAME_H
#define SWARMBOT_OTHELLOGAME_H

#include "OthelloBitBoard.h"
#include "Constants.h"
#include "Utils.h"
#include <iostream>

using namespace std;

class OthelloGame {
public:
    explicit OthelloGame(OthelloBitBoard *board);
    short getCurrentPlayer() const;
    bool nextMove();
    [[nodiscard]] bool isAgentBlack() const;
    OthelloBitBoard *getBoard() const;



private:
    string name = "C OthelloGame.cpp";
    short currentPlayer;
    OthelloBitBoard *board;
    bool agentBlack;
    bool moveMade;
public:
    bool isMoveMade() const;

private:
    char input;
    char col;
    short row;
    short lastMovePlayed;
public:
    void setLastMovePlayed(short lastMovePlayed);

public:
    short getLastMovePlayed() const;
    void interpretInput();
    void switchPlayer();

private:
    char outputPlayerColor;




};


#endif //SWARMBOT_OTHELLOGAME_H
