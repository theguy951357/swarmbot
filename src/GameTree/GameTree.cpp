//
// Created by cblah on 3/4/2022.
//

#include "../../includes/GameTree.h"

/**
 * The constructor will take in a bitboard and the moveSet to point at.
 * it will initialize a root node with the inputted board.
 * @param board a bitboard to start the tree at.
 * @param moves The MoveSet class to set pointers to on the nodes.
 */
GameTree::GameTree(OthelloBitBoard *board, MoveSet *moves): moves(moves) {
    this->root = new GameTreeNode(board);
}
/**
 *
 * The scanNode() method will take in a pointer to a node.
 * It will get the node's legal board and find the legal moves that can be played.
 * An initial child node will be made from the parent to represent the first move in the set of legal moves.
 * Then the rest of the moves will be represented as siblings of the initial child.
 * This will give the N-ary tree a binary tree structure so searching through the tree will be more efficient.
 * @param node
 */
void GameTree::scanNode(GameTreeNode *node) {
    //making a more defined color switching for now.
    short switchedColor;
    node->getColor()==BLACK?switchedColor=WHITE:switchedColor=BLACK;
    node->getCurrentBoard()->findLegals(switchedColor);//find legals for the next player.
    uint_fast64_t tempLegalBoard = node->getCurrentBoard()->getLegals();//get a temporary legal board for the legals we just found
    GameTreeNode *tmpNode = node;//make a temporary pointer to a node to traverse children and siblings.
    short correctedLocation;//more defined corrected location.
    if (tempLegalBoard!=EMPTY_OTHELLO_BITBOARD) {//if we are not at a leaf node.
        for (int i = 7; i >= 0; --i) {
            for (int j = 7; j >= 0; --j) {
                if (tempLegalBoard & (1ULL << (8 * i + j))) {//if the shifted board & the legal board have a match
                    correctedLocation = 63-(8*i+j);//get the corrected location for that move.
                    if (node->getChild()!= nullptr){//if the original node has a child.
                        //make a sibling
                        tmpNode->setSibling(new GameTreeNode(node->getCurrentBoard()),
                                            tmpNode,
                                            moves->getMove(correctedLocation));
                        tmpNode = tmpNode->getSibling();
                    }else{//if no child
                        //make the initial child
                        tmpNode->setChild(new GameTreeNode(node->getCurrentBoard()),
                                          node,
                                          moves->getMove(correctedLocation));
                        tmpNode = tmpNode->getChild();
                    }
                }
            }
        }
    }else{
        if (!node->getCurrentBoard()->gameOver(node->getColor())){//If the node is a pass move
            //make a pass move
            node->setChild(new GameTreeNode(node->getCurrentBoard()),
                           node,
                           moves->getMove(PASS));
        }else{
            //cout<<"C leaf node reached"<<endl;
        }
    }
    node->setState(SCANNED);
}

/**
 * this is a recursive print method for the gametree for debugging purposes.
 * @param node
 * @param space
 */
void GameTree::printGametree(const string &prefix, const GameTreeNode *node, bool isLeft) {
    if( node != nullptr )
    {
        cout << prefix;
        cout << (isLeft ? "|--" : "L__" );
        // print the value of the node
        cout << node->getLocation().getCol()<< node->getLocation().getRow() << endl;

        // enter the next tree level - left and right branch
        printGametree( prefix + (isLeft ? "|  " : "   "), node->getSibling(), true);
        printGametree( prefix + (isLeft ? "|  " : "   "), node->getChild(), false);
    }
}

void GameTree::printGametree(const GameTreeNode *node) {
    printGametree("", node, false);
}



/**
 * getter for the root node.
 * @return
 */
GameTreeNode *GameTree::getRoot() const {
    return root;
}

