//
// Created by cblah on 3/4/2022.
//

#include "../../includes/OthelloGame.h"

/**
 * Constructor will initialize the inputted board and set up
 * the players for the game.
 * @param board This is the bitboard representation that the main game will play on.
 */

OthelloGame::OthelloGame(OthelloBitBoard *board) : board(board) {
    this->currentPlayer = BLACK;
    cout<<"C What color would you like the agent to play? "<<endl;
    cout<<"C enter 'I W' for white or 'I B' for black."<<endl;
    cin>>input>>col;
    while (input != 'I' || input != 'i'){
        if (col == 'W' || col == 'w'){
            agentBlack = false;
            cout<<"C Ok, Agent will play as white. Opponent will play as black."<<endl;
            outputPlayerColor = 'W';
            cout<< "R "<<outputPlayerColor<<endl;
            cin.sync();
            break;
        }else if (col == 'B' || col == 'b'){
            agentBlack = true;
            cout<<"C Ok, Agent will play as black. Opponent will play as white."<<endl;
            outputPlayerColor = 'B';
            cout<< "R "<<outputPlayerColor<<endl;
            cin.sync();
            break;
        }else{
            cout<<"C I don't understand. Please enter 'I W' for white or 'I B' for black."<<endl;
            cin.sync();
            cin>>input>>col;
        }
    }
    this->lastMovePlayed = NO_MOVE_MADE_YET;
    moveMade = false;
    board->findLegals(currentPlayer);
}

/**
 * This method will take input from the instream.
 * it will then determine who is playing and what
 * move they are playing. the method will also check if the move played
 * is legal or being played by the right player.
 */

void OthelloGame::interpretInput() {
    this->board->printBoardWithLegalMoves();
    string instream;
    getline(cin, instream);
    input = instream[0];
    if (input == 'W' || input == 'w'){
        if(this->currentPlayer != WHITE){
            cout<<"C It is not white's turn yet."<<endl;
            moveMade = false;
        }else{
            if (!this->board->isPass()) {
                this->col = instream[2];
                this->row = instream[4] - 48;
                if (this->board->playPiece(this->currentPlayer, col, row,false)) {
                    this->lastMovePlayed = Utils::convertToGrid(col, row);
                    switchPlayer();
                    moveMade = true;
                }else{
                    moveMade = false;
                }
            }else{
                cout<< "C White passes"<<endl;
                this->lastMovePlayed = PASS;
                switchPlayer();
                moveMade = true;
            }
        }
    }else if(input == 'B' || input == 'b'){
        if(this->currentPlayer != BLACK){
            cout<<"C It is not black's turn yet"<<endl;
            moveMade = false;
        }else{
            if(!this->board->isPass()) {
                this->col = instream[2];
                this->row = instream[4] - 48;
                if (this->board->playPiece(this->currentPlayer, col, row,false)) {
                    this->lastMovePlayed = Utils::convertToGrid(col, row);
                    switchPlayer();
                    moveMade = true;
                }else{
                    moveMade = false;
                }
            }else{
                cout<< "C Black passes"<<endl;
                this->lastMovePlayed = PASS;
                switchPlayer();
                moveMade = true;
            }
        }
    }else if(input == 'I' || input == 'i'){
        cout<< "C Players have already been initialized."<<endl;
        cout<< "C Please pick a move."<<endl;
        moveMade = false;
    }else{
        cout<< "C I don't understand. Please pick a move."<<endl;
        moveMade = false;
    }
}

/**
 * This method is the nextMove method for the game.
 * While the game is not over, get input and send the input to the board.
 */

bool OthelloGame::nextMove() {
    if (!board->gameOver(currentPlayer)){
        return true;
    }
    board->printBoard();
    board->announceWinner();
    return false;
}

/**
 * This method switches the current player
 */

void OthelloGame::switchPlayer() {
    this->currentPlayer *= -1;
    this->board->findLegals(currentPlayer);
}

/**
 * boolean telling what color the agent is.
 * @return true if the agent is playing as black and false if not.
 */

bool OthelloGame::isAgentBlack() const {
    return agentBlack;
}

OthelloBitBoard *OthelloGame::getBoard() const {
    return board;
}

short OthelloGame::getCurrentPlayer() const {
    return currentPlayer;
}

short OthelloGame::getLastMovePlayed() const {
    return lastMovePlayed;
}

bool OthelloGame::isMoveMade() const {
    return moveMade;
}

void OthelloGame::setLastMovePlayed(short lastMovePlayed) {
    OthelloGame::lastMovePlayed = lastMovePlayed;
}







