//
// Created by cblah on 3/4/2022.
//

#include "../../includes/Player.h"
//player class will loop the game instead of the Game itself.

/**
 * The player will start the game.
 * In this constructor, the player will take in a game as a parameter,
 * determine what color each player has chosen, and then
 * get the legal moves to be played while there is still
 * a next move to be played (not game over)
 * @param game
 */

Player::Player(OthelloGame *game) : game(game) {

    this->moves = new MoveSet();
    game->isAgentBlack() ? this->color = -1: this->color = 1;
    this->color == BLACK ? this->playerColor = 'B':this->playerColor = 'W';
}

/**
 * This method will get a set of legal moves for the current player
 * It will get the current bitboard from the game containing the current
 * set of legal moves.
 * Then it will go through that board and fill a vector
 * with pointers to those moves on the MoveSet.
 */

void Player::getLegalMoves() {
    this->legalMoves.clear();
    this->legalBoard = this->game->getBoard()->getLegals();
    for (int i = 7; i >= 0 ; --i) {
        for (int j = 7; j >= 0 ; --j) {
            if (this->legalBoard & (1ULL << (8* i + j))){
                //cout<<"C debug number "<<64-(8*i+j)<<endl;
                this->legalMoves.push_back(this->moves->getMove(63-(8*i+j)));
            }
        }
    }
    printMoves();
}
/**
 * Prints out the vector of current legal moves that can be played by the current player
 */
void Player::printMoves() {
    cout<<"C legal moves for ";
    Utils::printPlayerColor(this->game->getCurrentPlayer());
    cout<<": ";
    for (it = legalMoves.begin(); it!=legalMoves.end(); ++it) {
        Move *temp = *it;
        temp->printMove();
        if (it != legalMoves.end()-1){
            cout<<", ";
        }
    }
    cout<<endl;
}
/**
 * this is a method to print the strategy of the current player.
 * in the Player class, this would be the opponent.
 */
void Player::printStrategy() {
    if (this->strategy.empty()){
        return;
    }
    cout<<"C "<<this->playerColor<<" moves played: ";
    printStrategyHelper();
    cout<<endl;
}
/**
 * recursive helper method for printing the strategy stack
 */
void Player::printStrategyHelper() {
    if (this->strategy.empty()){
        return;
    }
    Move *x = this->strategy.top();
    this->strategy.pop();
    printStrategyHelper();
    x->printMove();
    cout<<", ";
    this->strategy.push(x);

}
/**
 * This is the initial method for the player to make a move.
 * it will get the list of legal moves, wait for input from the user
 * or the referee passing along the other agent, then it will add that move
 * to the strategy stack.
 */
void Player::makeMove() {
    getLegalMoves();
    game->interpretInput();
    addToStrategy();
}
/**
 * This method will add a pointer to a move in a stack
 * this will dictate all the moves that player has made.
 * when the swarm agents start, these stacks will be in a
 * genetic algorithm that will dictate the behavior of
 * each swarm agent.
 */
void Player::addToStrategy() {
    short temp = this->game->getLastMovePlayed();
    if (temp!=-1 && this->game->getCurrentPlayer()!=this->color){
        if (this->game->isMoveMade()){
            this->strategy.push(this->moves->getMove(63-temp));
        }
        printStrategy();
    }
}

short Player::getColor() const {
    return color;
}

