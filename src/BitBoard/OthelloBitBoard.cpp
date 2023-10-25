//
// Created by cblah on 3/4/2022.
//
/**
 * This is a bitboard representation for the othello program.
 * methods for the bitboard found here. https://linuxtut.com/en/09f114799319c7ba19dc/
 */

#include "../../includes/OthelloBitBoard.h"

/**
 * This constructor sets the starting board for the black and white player.
 * It also sets a legal board to an empty board.
 */

OthelloBitBoard::OthelloBitBoard() : boardBlackPlayer(
        INITIAL_BLACK_OTHELLO_BITBOARD), boardWhitePlayer(INITIAL_WHITE_OTHELLO_BITBOARD), legals(EMPTY_OTHELLO_BITBOARD) {

}

OthelloBitBoard::OthelloBitBoard(OthelloBitBoard *boardToCopy) {
    this->boardBlackPlayer = boardToCopy->getBoardBlackPlayer();
    this->boardWhitePlayer = boardToCopy->getBoardWhitePlayer();
    this->legals = boardToCopy->getLegals();
}

/**
 * This is the method that will allow the player to take action on the board.
 * It will take the x and y coordinates and make a putted board and then
 * or that with the player board making the move.
 * Then the flipping helper methods will find any pieces that will need to be flipped.
 * @param player The player making the move
 * @param y The letter a-h
 * @param x the number 1-8
 */

bool OthelloBitBoard::playPiece(short player, short y, short x, bool simulation) {
    short shift = Utils::convertToGrid(y,x);
    uint_fast64_t piecePlayed = 1ULL<<shift;
    if(!isLegalMove(piecePlayed)){
        if(isLegalMove(63-piecePlayed)){
//            cout<< name << " Your board is flipped the move should be ";
//            Utils::printGridLocation(63-shift);
//            cout<<" C instead of ";
//            Utils::printGridLocation(shift);
//            printBoardWithLegalMoves();
        }else {
//            cout << this->name << " ";
//            Utils::printGridLocation(shift);
//            cout << "C is not a legal move. Try again." << endl;
//            printBoardWithLegalMoves();
        }
        return false;
    }else {
        //player == BLACK ? this->boardBlackPlayer |= piecePlayed : this->boardWhitePlayer |= piecePlayed;
        //uint_fast64_t mask;
//        for (int i = 0; i < 4; ++i) {
//            if (i == 0) {
//                mask = BITBOARD_VERTICAL_MASK;
//            } else if (i == 1) {
//                mask = BITBOARD_HORIZONTAL_MASK;
//            } else {
//                mask = BITBOARD_ALL_SIDES_MASK;
//            }
//            flipPiecesForMove(player, piecePlayed, mask, DIR[i]);
//            flipPiecesForMoveOther(player, piecePlayed, mask, DIR[i]);
//        }
        uint_fast64_t playerBoard;
        uint_fast64_t opponentBoard;
        if (player==BLACK){
            playerBoard = this->boardBlackPlayer;
            opponentBoard = this->boardWhitePlayer;
        }else{
            playerBoard = this->boardWhitePlayer;
            opponentBoard = this->boardBlackPlayer;
        }
        // refactored piece flipping algorithm. the other one was not working.
        flipPiecesForMoveOtherOther(piecePlayed, playerBoard, opponentBoard, player);

        if (!simulation) {
            player == BLACK ? cout << "B " : cout << "W ";
            Utils::printGridLocation(shift);
        }
        return true;
    }
}
/**
 * This private helper method will flip the proper pieces when a move is played.
 * like the search for legal moves, there are two methods that will search both ways for each direction.
 * @param player This is the player that is making the move.
 * @param putted This is a board that contains a 1 at the move being made
 * @param mask This is the horizontal, vertical, or all sides mask that will be applied
 * @param dir The direction being traversed
 */
void OthelloBitBoard::flipPiecesForMove(short player, uint_fast64_t putted, uint_fast64_t mask, short dir) {
    uint_fast64_t puttedMask = putted;
    uint_fast64_t playerBoard;
    uint_fast64_t opponentBoard;
    uint_fast64_t reversed = 0;
    if (player==BLACK){
        playerBoard = this->boardBlackPlayer;
        opponentBoard = this->boardWhitePlayer;
    }else{
        playerBoard = this->boardWhitePlayer;
        opponentBoard = this->boardBlackPlayer;
    }
    uint_fast64_t tmp = (puttedMask<<dir) & mask;
    uint_fast64_t reverseTmp = tmp & opponentBoard;
    //tmp<<=dir;
    while(tmp!=0 && (tmp&opponentBoard) != 0){
        reverseTmp |= tmp;
        tmp<<=dir;
    }
    if ((tmp&playerBoard)!=0){
        reversed |= reverseTmp;
    }

    playerBoard ^= puttedMask | reversed;
    opponentBoard ^= reversed;
    if (player==BLACK) {
        this->boardBlackPlayer |= playerBoard;
        this->boardWhitePlayer &= opponentBoard;
    }else{
        this->boardWhitePlayer |= playerBoard;
        this->boardBlackPlayer &= opponentBoard;
    }
}
/**
 * This private helper method will flip the proper pieces when a move is played.
 * like the search for legal moves, there are two methods that will search both ways for each direction.
 * @param player This is the player that is making the move.
 * @param putted This is a board that contains a 1 at the move being made
 * @param mask This is the horizontal, vertical, or all sides mask that will be applied
 * @param dir The direction being traversed
 */

void OthelloBitBoard::flipPiecesForMoveOther(short player, uint_fast64_t putted, uint_fast64_t mask, short dir) {
    uint_fast64_t puttedMask = putted;
    uint_fast64_t playerBoard;
    uint_fast64_t opponentBoard;
    uint_fast64_t reversed = 0;
    if (player==BLACK){
        playerBoard = this->boardBlackPlayer;
        opponentBoard = this->boardWhitePlayer;
    }else{
        playerBoard = this->boardWhitePlayer;
        opponentBoard = this->boardBlackPlayer;
    }
    uint_fast64_t tmp = (puttedMask>>dir) & mask;
    uint_fast64_t reverseTmp = tmp & opponentBoard;
    //tmp>>=dir;
    while(tmp!=0 && (tmp&opponentBoard) != 0){
        reverseTmp |= tmp;
        tmp>>=dir;
    }
    if ((tmp&playerBoard)!=0){
        reversed |= reverseTmp;
    }
    playerBoard ^= puttedMask | reversed;
    opponentBoard ^= reversed;
    if (player==BLACK) {
        this->boardBlackPlayer |= playerBoard;
        this->boardWhitePlayer &= opponentBoard;
    }else{
        this->boardWhitePlayer |= playerBoard;
        this->boardBlackPlayer &= opponentBoard;
    }
}


void OthelloBitBoard::findLegals(short player) {
    legals = EMPTY_OTHELLO_BITBOARD;
    uint_fast64_t playerBoard;
    uint_fast64_t opponentBoard;
    if(player==BLACK){
        playerBoard = this->boardBlackPlayer;
        opponentBoard = this->boardWhitePlayer;
    }else{
        playerBoard = this->boardWhitePlayer;
        opponentBoard = this->boardBlackPlayer;
    }
    uint_fast64_t mask;
    for (int i=0; i<4; ++i) {
        if (i==0){
            mask = BITBOARD_VERTICAL_MASK;
        }else if (i==1){
            mask = BITBOARD_HORIZONTAL_MASK;
        }else{
            mask = BITBOARD_ALL_SIDES_MASK;
        }
        searchDirection(playerBoard, opponentBoard, mask, DIR[i]);
        searchOtherDirection(playerBoard, opponentBoard, mask, DIR[i]);
    }
}

/**
 * this private helper method will search one way in the given direction
 * and search for legal moves to show on the board
 * @param player this is the current player's board being acted on
 * @param opponent this is the opponent's board being acted
 * @param mask this is the mask that is being applied. either horizontal, vertical, or all sides
 * @param dir the direction that will be traversed to find legal moves
 */

void OthelloBitBoard::searchDirection(uint_fast64_t player, uint_fast64_t opponent, uint_fast64_t mask, short dir) {

    uint_fast64_t maskedBoard = opponent&mask;
    uint_fast64_t tmp = maskedBoard & (player<<dir);
    for (int i = 0; i < 6; ++i) {
        tmp|=maskedBoard&(tmp<<dir);
    }
    uint_fast64_t currentBoard = player|opponent;
    currentBoard = ~currentBoard;
    this->legals|= (tmp<<dir)&currentBoard;

}
/**
 * This private helper method will search the way in the given direction
 * and search for legal moves to show on the board.
 * Even though this way results in repetitive code, I found it easier than resetting the shifted boards
 * within one method and going the other direction.
 * @param player this is the current player's board being acted on
 * @param opponent this is the opponent's board being acted
 * @param mask this is the mask that is being applied. either horizontal, vertical, or all sides
 * @param dir the direction that will be traversed to find legal moves
 */

void OthelloBitBoard::searchOtherDirection(uint_fast64_t player, uint_fast64_t opponent, uint_fast64_t mask, short dir) {
    uint_fast64_t maskedBoard = opponent&mask;
    uint_fast64_t tmp = maskedBoard & (player>>dir);
    for (int i = 0; i < 6; ++i) {
        tmp|=maskedBoard&(tmp>>dir);
    }
    uint_fast64_t currentBoard = player|opponent;
    currentBoard = ~currentBoard;
    this->legals|= (tmp>>dir)&currentBoard;
}

/**
 * This will print out any board you put in as the parameter.
 * @param board
 */

void OthelloBitBoard::printBoard(uint_fast64_t board) const {
    cout<<this->name<<endl;
    for (int i = 7; i >= 0 ; --i) {
        cout<< "C    ";
        for (int j = 7; j >= 0 ; --j) {
            if (board & (1ULL << (8* i + j))){
                cout<< "1  ";
            }else{
                cout << "0  ";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

/**
 * This will print the board with the current legal moves that can be played
 * legal moves are marked with an x.
 */
void OthelloBitBoard::printBoardWithLegalMoves() const {
    short count = 1;
    cout<< "C    a  b  c  d  e  f  g  h"<<endl;
    for (int i = 7; i >= 0 ; --i) {
        cout<<"C " << count<< "  ";
        ++count;
        for (int j = 7; j >= 0 ; --j) {
            if (boardBlackPlayer & (1ULL << (8* i + j))){
                cout<< "B  ";
            }else if (boardWhitePlayer & (1ULL << (8* i + j))){
                cout << "W  ";
            }else if(legals & (1ULL << (8*i+j))){
                cout<< "+  ";
            }else{
                cout << "_  ";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

/**
 * prints the current state of the board without legal moves
 */
void OthelloBitBoard::printBoard() const {
    short count = 1;
    cout<< "C    a  b  c  d  e  f  g  h"<<endl;
    for (int i = 7; i >= 0 ; --i) {
        cout<<"C " << count<< "  ";
        ++count;
        for (int j = 7; j >= 0 ; --j) {
            if (boardBlackPlayer & (1ULL << (8* i + j))){
                cout<< "B  ";
            }else if (boardWhitePlayer & (1ULL << (8* i + j))){
                cout << "W  ";
            }else{
                cout << "_  ";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}
/**
 * This method will take a bitboard representation of the move being played
 * and & it with the current legal moves. If the result is an empty
 * bitboard, the move is not legal.
 * @param putted This is a bitboard representing the move being played.
 * @return Returns true if the move is legal, and false if not.
 */
bool OthelloBitBoard::isLegalMove(uint_fast64_t putted) const {
    if (legals == EMPTY_OTHELLO_BITBOARD && putted & 1ULL<<27){//checks for pass move if a pass move is legal
        return true;
    }
    if ((putted&legals)!= EMPTY_OTHELLO_BITBOARD){
        return true;
    }else{
        return false;
    }
}
/**
 * method will check for game over by comparing the current state
 * of the game board with
 * @param currentPlayer
 * @return true if the game is over, and false if the game can keep going.
 */
bool OthelloBitBoard::gameOver(short currentPlayer) {
    uint_fast64_t currentBoard = this->boardBlackPlayer|this->boardWhitePlayer;
    //printBoard(currentBoard);
    findLegals(currentPlayer);
    if(currentBoard == FULL_OTHELLO_BITBOARD){
        return true;
    }else if( this->legals == EMPTY_OTHELLO_BITBOARD){
        findLegals(currentPlayer*-1);
        if(this->legals == EMPTY_OTHELLO_BITBOARD){
            return true;
        }
        findLegals(currentPlayer);
    }
    return false;
}

bool OthelloBitBoard::isPass() const {
    if(this->legals == EMPTY_OTHELLO_BITBOARD){
        return true;
    }else{
        return false;
    }

}

uint_fast64_t OthelloBitBoard::getLegals() const {
    return legals;
}

uint_fast64_t OthelloBitBoard::getBoardBlackPlayer() const {
    return boardBlackPlayer;
}

uint_fast64_t OthelloBitBoard::getBoardWhitePlayer() const {
    return boardWhitePlayer;
}

void OthelloBitBoard::announceWinner() {
    short winValue = 0;
    short blackCount = 0;
    short whiteCount = 0;
    for (int i = 7; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            uint_fast64_t location = 1ULL <<(8*i+j);
            if (boardBlackPlayer & location) {
                ++blackCount;
            }else if (boardWhitePlayer & location){
                ++whiteCount;
            }
        }
    }
    blackCount > whiteCount ? winValue=BLACK : winValue=WHITE;
    cout<<this->name<<" black/white count: "<<blackCount<<"/"<<whiteCount<<". ";
    Utils::printPlayerColor(winValue);
    cout<<"wins the game!"<<endl;

}

uint_fast64_t OthelloBitBoard::shiftN(uint_fast64_t bb) {
    return bb<<8;
}

uint_fast64_t OthelloBitBoard::shiftS(uint_fast64_t bb) {
    return bb>>8;
}

uint_fast64_t OthelloBitBoard::shiftE(uint_fast64_t bb) {
    return (bb&BITBOARD_EAST_MASK)>>1;
}

uint_fast64_t OthelloBitBoard::shiftW(uint_fast64_t bb) {
    return (bb&BITBOARD_WEST_MASK)<<1;
}

uint_fast64_t OthelloBitBoard::shiftNW(uint_fast64_t bb) {
    return shiftN(shiftW(bb));
}

uint_fast64_t OthelloBitBoard::shiftNE(uint_fast64_t bb) {
    return shiftN(shiftE(bb));
}

uint_fast64_t OthelloBitBoard::shiftSW(uint_fast64_t bb) {
    return shiftS(shiftW(bb));
}

uint_fast64_t OthelloBitBoard::shiftSE(uint_fast64_t bb) {
    return shiftS(shiftE(bb));
}
/**
 * taken from github.com/expxpace/Minimax-Othello-Bitboard .
 * The other piece flipping algorithm was turning over other
 * pieces without any rhyme or reason.
 * had to redo it. this one works much better.
 * @param move bitboard representing the move being played
 * @param self player's bitboard
 * @param enemy opponent's bitboard
 * @param player player color to determine where to assign the results
 */
void OthelloBitBoard::flipPiecesForMoveOtherOther(uint_fast64_t move, uint_fast64_t self, uint_fast64_t enemy, short player) {
    uint_fast64_t captured;

    self|= move;

    //North
    captured = shiftN(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftN(captured) & enemy;
    }
    if ((shiftN(captured)&self)!=0){
        self |= captured;
        enemy &= ~captured;
    }

    //South
    captured = shiftS(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftS(captured) & enemy;
    }
    if ((shiftS(captured) & self)!=0){
        self|=captured;
        enemy&=~captured;
    }

    //East
    captured = shiftE(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftE(captured) & enemy;
    }
    if ((shiftE(captured) & self)!=0){
        self|=captured;
        enemy&=~captured;
    }

    //West
    captured = shiftW(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftW(captured) & enemy;
    }
    if ((shiftW(captured) & self)!=0){
        self|=captured;
        enemy&=~captured;
    }

    //Northeast
    captured = shiftNE(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftNE(captured) & enemy;
    }
    if ((shiftNE(captured) & self)!=0){
        self|=captured;
        enemy&=~captured;
    }

    //Northwest
    captured = shiftNW(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftNW(captured) & enemy;
    }
    if ((shiftNW(captured) & self)!=0){
        self|=captured;
        enemy&=~captured;
    }

    //Southeast
    captured = shiftSE(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftSE(captured) & enemy;
    }
    if ((shiftSE(captured) & self)!=0){
        self|=captured;
        enemy&=~captured;
    }

    //Southwest
    captured = shiftSW(move) & enemy;
    for (int i = 0; i < 5; ++i) {
        captured |= shiftSW(captured) & enemy;
    }
    if ((shiftSW(captured) & self)!=0){
        self|=captured;
        enemy&=~captured;
    }

    if (player == BLACK){
        this->boardBlackPlayer = self;
        this->boardWhitePlayer = enemy;
    }else{
        this->boardWhitePlayer = self;
        this->boardBlackPlayer = enemy;
    }

}








