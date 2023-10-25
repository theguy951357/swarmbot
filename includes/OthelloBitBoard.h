//
// Created by cblah on 3/4/2022.
//

#ifndef SWARMBOT_OTHELLOBITBOARD_H
#define SWARMBOT_OTHELLOBITBOARD_H

#include <cstdint>
#include <cstdio>
#include <iostream>

#include "Constants.h"
#include "Utils.h"

using namespace std;

class OthelloBitBoard {

private:
    string name = "C OthelloBitBoard.cpp";
    uint_fast64_t boardBlackPlayer;
    uint_fast64_t boardWhitePlayer;
    uint_fast64_t legals;


public:
    uint_fast64_t getBoardBlackPlayer() const;

    uint_fast64_t getBoardWhitePlayer() const;

    explicit OthelloBitBoard(OthelloBitBoard *boardToCopy);

    void printBoard() const;

    bool playPiece(short player, short y, short x, bool simulation);

    bool gameOver(short currentPlayer);

    void announceWinner();

    bool isPass() const;

    void findLegals(short player);

    OthelloBitBoard();

    void printBoardWithLegalMoves() const;

    void printBoard(uint_fast64_t board) const;

    uint_fast64_t getLegals() const;

private:

    void searchDirection(uint_fast64_t player, uint_fast64_t opponent, uint_fast64_t mask, short dir);

    void flipPiecesForMove(short player, uint_fast64_t putted, uint_fast64_t mask, short dir);

    void flipPiecesForMoveOther(short player, uint_fast64_t putted, uint_fast64_t mask, short dir);

    void searchOtherDirection(uint_fast64_t player, uint_fast64_t opponent, uint_fast64_t mask, short dir);

    [[nodiscard]] bool isLegalMove(uint_fast64_t putted) const;

    void flipPiecesForMoveOtherOther(uint_fast64_t move, uint_fast64_t self, uint_fast64_t enemy, short player);

    uint_fast64_t shiftN(uint_fast64_t bb);

    uint_fast64_t shiftS(uint_fast64_t bb);

    uint_fast64_t shiftE(uint_fast64_t bb);

    uint_fast64_t shiftW(uint_fast64_t bb);

    uint_fast64_t shiftNW(uint_fast64_t bb);

    uint_fast64_t shiftNE(uint_fast64_t bb);

    uint_fast64_t shiftSW(uint_fast64_t bb);

    uint_fast64_t shiftSE(uint_fast64_t bb);

};


#endif //SWARMBOT_OTHELLOBITBOARD_H
