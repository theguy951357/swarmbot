//
// Created by cblah on 5/5/2022.
//

#include "../../includes/GameTreeNode.h"


GameTreeNode::GameTreeNode(OthelloBitBoard *currentBoard) : currentBoard(currentBoard) {
    //cout<<"C Node made"<<endl;
    this->parent = nullptr;
    this->child = nullptr;
    this->sibling = nullptr;
    this->state = NOT_SCANNED;
    this->color = WHITE;
    this->rank = 0.0f;
    this->wi = 0;
    this->ni = 0;
    this->lock=false;
}

bool GameTreeNode::isLock() const {
    return lock;
}

void GameTreeNode::setLock(bool lock) {
    GameTreeNode::lock = lock;
}

const Move &GameTreeNode::getLocation() const {
    return location;
}

void GameTreeNode::setLocation(const Move &location) {
    GameTreeNode::location = location;
}

short GameTreeNode::getState() const {
    return state;
}

void GameTreeNode::setState(short state) {
    GameTreeNode::state = state;
}

short GameTreeNode::getColor() const {
    return color;
}

void GameTreeNode::setColor(short color) {
    GameTreeNode::color = color;
}

double GameTreeNode::getRank() const {
    return rank;
}

void GameTreeNode::setRank(double rank) {
    GameTreeNode::rank = rank;
}

int GameTreeNode::getWi() const {
    return wi;
}

void GameTreeNode::setWi(int wi) {
    GameTreeNode::wi = wi;
}

int GameTreeNode::getNi() const {
    return ni;
}

void GameTreeNode::setNi(int ni) {
    GameTreeNode::ni = ni;
}

OthelloBitBoard *GameTreeNode::getCurrentBoard() const {
    return currentBoard;
}

void GameTreeNode::setCurrentBoard(OthelloBitBoard *currentBoard) {
    GameTreeNode::currentBoard = currentBoard;
}

GameTreeNode *GameTreeNode::getParent() const {
    return parent;
}

void GameTreeNode::setParent(GameTreeNode *parent) {
    GameTreeNode::parent = parent;
}

GameTreeNode *GameTreeNode::getChild() const {
    return child;
}

void GameTreeNode::setChild(GameTreeNode *child) {
    GameTreeNode::child = child;
}

GameTreeNode *GameTreeNode::getSibling() const {
    return sibling;
}

void GameTreeNode::setSibling(GameTreeNode *sibling) {
    GameTreeNode::sibling = sibling;
}

void GameTreeNode::setChild(GameTreeNode *child, GameTreeNode *parent, Move *move) {
    child->setParent(parent);//set the parent
    parent->setChild(child);//set the parent's child.
    child->setLocation(*move);//set the pointer to the move
    short childColor;
    child->setCurrentBoard(new OthelloBitBoard(parent->getCurrentBoard()));
    parent->getColor() == BLACK? childColor = WHITE : childColor = BLACK;
    child->setColor(childColor);//set the color of the current player at the ith move
    child->getCurrentBoard()->findLegals(child->getColor());//find legals for the current board
    if (child->getCurrentBoard()->getLegals() != EMPTY_OTHELLO_BITBOARD){//if it is not a pass
        if (child->getCurrentBoard()->getLegals()==child->getParent()->getCurrentBoard()->getLegals()){//first compare the legals
            child->getCurrentBoard()->playPiece(child->getColor(), move->getCol(), move->getRow(), true);
        }else{
            cout<<name<< " the child's board before playing a move does not match the parent. attempting to fix."<<endl;
            parent->setCurrentBoard(new OthelloBitBoard(parent->getParent()->getCurrentBoard()));
            parent->getCurrentBoard()->findLegals(parent->getColor()*-1);
            parent->getCurrentBoard()->playPiece(parent->getColor(),parent->getLocation().getCol(), parent->getLocation().getRow(),true);

            child->setCurrentBoard(new OthelloBitBoard(child->getParent()->getCurrentBoard()));
            child->getCurrentBoard()->findLegals(child->getColor()*-1);
            if (child->getCurrentBoard()->getLegals()==child->getParent()->getCurrentBoard()->getLegals()){//first compare the legals
                child->getCurrentBoard()->playPiece(child->getColor(),move->getCol(),move->getRow(),true);
            }else{
                //cout<<name<< " child fix did not work."<<endl;
            }
        }

    }//otherwise just leave it as is.

}

void GameTreeNode::setSibling(GameTreeNode *sibling, GameTreeNode *upperSibling, Move *move) {
    sibling->setParent(upperSibling->getParent());//set the parent to the upper sibling's parent.
    upperSibling->setSibling(sibling);//set the upper sibling's sibling to the current sibling.
    sibling->setLocation(*move);//set a pointer to the move.
    sibling->setColor(upperSibling->getColor());//set the color to the upper sibling's color. same moveset.
    //no need to check if the sibling is a pass move or not.
    // If the initial child has a move, then the rest of the siblings have a move.
    sibling->setCurrentBoard(new OthelloBitBoard(sibling->getParent()->getCurrentBoard()));
    sibling->getCurrentBoard()->findLegals(sibling->getColor());
    sibling->getParent()->getCurrentBoard()->findLegals(sibling->getColor());
    //the sibling's current board at this point should match the parent's current board before a move is applied to the sibling.
    if (sibling->getCurrentBoard()->getLegals()==sibling->getParent()->getCurrentBoard()->getLegals()){//first compare the legals
        sibling->getCurrentBoard()->playPiece(sibling->getColor(),move->getCol(),move->getRow(),true);
    }else{
        cout<<name<< " the siblings board before playing a move does not match the parent. attempting to fix."<<endl;
        sibling->setCurrentBoard(new OthelloBitBoard(sibling->getParent()->getCurrentBoard()));
        sibling->getCurrentBoard()->findLegals(sibling->getColor());
        if (sibling->getCurrentBoard()->getLegals()==sibling->getParent()->getCurrentBoard()->getLegals()){//first compare the legals
            sibling->getCurrentBoard()->playPiece(sibling->getColor(),move->getCol(),move->getRow(),true);
        }else{
            //cout<<name<< " sibling fix did not work."<<endl;
        }
    }

}







