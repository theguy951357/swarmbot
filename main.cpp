#include <iostream>
#include "includes/OthelloBitBoard.h"
#include "includes/OthelloGame.h"
#include "includes/MoveSet.h"
#include "includes/Player.h"
#include "includes/Agent.h"
#include "includes/GameTree.h"
#include "includes/MonteCarloAgent.h"
//#include "includes/Utils.h"

int main() {
    //TODO agent segfaults when human error is made.
    // Wouldn't be a big deal with agent vs agent, but when human plays, it should handle it.
    //make player first, then the agent. agent won't know its color the other way around.
    auto *moves = new MoveSet();
    auto *board = new OthelloBitBoard();
    auto *gametree = new GameTree(board,moves);
//    auto *mcAgent = new MonteCarloAgent(gametree);
    //gameplay
    auto *game = new OthelloGame(board);
    auto *player = new Player(game);
    auto *player2 = new Player(game);
    auto *agent = new Agent(game,gametree);
    do{
        if (game->nextMove()){
            agent->decide();
        }
        if (game->nextMove()){
            player->makeMove();
        }
//        if (game->nextMove()){
//            player2->makeMove();
//        }
    }while(game->nextMove());
    agent->printTreeToTxt(gametree->getRoot());
    agent->printAverageTimeToFile();
    //end of gameplay


    //testing for monteCarlo agent
//    GameTreeNode *testNode = gametree->getRoot();
//    mcAgent->simulate(testNode);
//    mcAgent->findWinValue();
//    mcAgent->backPropagate(testNode,BLACK);
//    testNode = testNode->getChild();
//    int wi = testNode->getParent()->getWi();
//    int ni = testNode->getParent()->getNi();
//    cout<<"C wi/ni for "<<testNode->getLocation().getCol()<<testNode->getLocation().getRow()<<": "<<wi<<"/"<<ni<<endl;
//    do {
//        testNode = testNode->getSibling();
//        mcAgent->simulate(testNode);
//        mcAgent->findWinValue();
//        mcAgent->backPropagate(testNode,BLACK);
//        wi = testNode->getWi();
//        ni = testNode->getNi();
//        cout<<"C wi/ni for "<<testNode->getLocation().getCol()<<testNode->getLocation().getRow()<<": "<<wi<<"/"<<ni<<endl;
//    } while (testNode->getSibling()!= nullptr);
//    wi = gametree->getRoot()->getWi();
//    ni = gametree->getRoot()->getNi();
//    cout<<"C wi/ni for root: "<<wi<<"/"<<ni<<endl;
//    if (wi>=ni/2){
//        cout<<"C simulations favoring agent so far."<<endl;
//    }else{
//        cout<<"C simulations favoring opponent so far."<<endl;
//    }


// testing for scanning a node
//    gametree->scanNode(gametree->getRoot());
//    gametree->scanNode(gametree->getRoot()->getChild()->getSibling()->getSibling());
//    gametree->scanNode(gametree->getRoot()->getChild()->getSibling()->getSibling()->getChild());
//    gametree->printTree(gametree->getRoot(),0);
// testing for adding children/siblings
//    auto *gtnode = new GameTreeNode(board);
//
//    gtnode->setChild(new GameTreeNode(new OthelloBitBoard(board)),
//                     gtnode,
//                     moves->getMove(63-Utils::convertToGrid('d',3)));
//
//    gtnode->getChild()->setSibling(new GameTreeNode(new OthelloBitBoard(gtnode->getCurrentBoard())),
//                                   gtnode->getChild(),
//                                   moves->getMove(63-Utils::convertToGrid('c',4)));
//
//    gtnode->getCurrentBoard()->printBoard();
//    gtnode->getChild()->getCurrentBoard()->printBoard();
//    gtnode->getChild()->getSibling()->getCurrentBoard()->printBoard();
// testing for bitboard
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'e', 6, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'f', 4, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'f', 3, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'f', 2, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'g', 4, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'f', 5, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'e', 2, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'd', 3, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'e', 3, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'h', 4, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'f', 1, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'c', 5, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'g', 5, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'g', 1, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'h', 1, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();
//    board->playPiece(WHITE, 'g', 2, false);
//    board->findLegals(BLACK);
//    board->printBoardWithLegalMoves();
//    board->playPiece(BLACK, 'h', 2, false);
//    board->findLegals(WHITE);
//    board->printBoardWithLegalMoves();






    return 0;
}
