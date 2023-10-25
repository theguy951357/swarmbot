//
// Created by cblah on 3/4/2022.
//


#include "../../includes/Agent.h"


/**
 * The constructor will inherit from the Player class.
 * The only difference is that if the game determines
 * that the agent is playing black, then it will set its own color to black
 * instead of the Player class setting its color to the opposite.
 * @param game
 */
Agent::Agent(OthelloGame *game, GameTree *gameTree) : Player(game), gameTree(gameTree) {
    game->isAgentBlack()? this->color = 1 : this->color = -1;
    this->color == BLACK ? this->playerColor = 'B':this->playerColor = 'W';
    cout<<name<<" Agent made"<<endl;
    for (int i = 0; i < MAX_SWARM_SIZE; ++i) {
        auto *swarm1 = new AntColonySwarm(gameTree);
        acswarm.emplace(acswarm.end(),*swarm1);
    }
    cout<<name<<" swarm made with "<<acswarm.size()<<" agents"<<endl;
    this->bookmark = gameTree->getRoot();
    this->treeCounter=0;

}
/**
 * This will begin the agents thinking process for each move.
 *
 */
void Agent::decide() {



    moveBookmark();

    if (this->bookmark->getState()!= SCANNED){
        gameTree->scanNode(this->bookmark);
    }

    if (this->game->getCurrentPlayer()==this->color) {
        getLegalMoves();
        this->bookmark->getCurrentBoard()->printBoardWithLegalMoves();

        GameTreeNode *children = this->bookmark->getChild();
        while(children != nullptr){
            monteCarloParallel(children);
            //gameTree->printGametree(gameTree->getRoot());

            children = children->getSibling();
        }

        if (!this->legalMoves.empty()){
            Move temp = analyze()->getLocation();
            this->game->getBoard()->playPiece(this->color, temp.getCol(), temp.getRow(),false);
            this->game->setLastMovePlayed(Utils::convertToGrid(temp.getCol(),temp.getRow()));
            //cout << this->playerColor << " " << temp->getCol() << " " << temp->getRow() << endl;
        }else{
            this->game->getBoard()->playPiece(this->color, 'e', 5,false);
            this->game->setLastMovePlayed(PASS);
            this->bookmark = this->bookmark->getChild();
        }
        agentAddToStrategy();
        cout<<this->name<<" Agent plays ";
        Utils::printGridLocation(this->game->getLastMovePlayed());
        //cout<<endl;
        this->game->switchPlayer();
    }
}
/**
 * The agent needs a different addToStrategy method, because the player's addToStrategy()
 * method needs to check if a move was made due to human error.
 * since the agent doesn't activate that check because it doesn't
 * use the interpretInput() method that alters that bool.
 * That check gets bypassed and thinks a move was not made.
 */
void Agent::agentAddToStrategy() {
    short temp = this->game->getLastMovePlayed();
    if (temp!=-1 && this->game->getCurrentPlayer()==this->color){
        this->strategy.push(this->moves->getMove(63-temp));
        printStrategy();
    }
}

void Agent::moveBookmark() {

    if (this->game->getLastMovePlayed() == NO_MOVE_MADE_YET){
        cout<<this->name<<" no move has been made yet. bookmark not moved"<<endl;
        return;
    }
    cout<<name<<" Bookmark is at "<<this->bookmark->getLocation().getCol()<<this->bookmark->getLocation().getRow()<<endl;
    this->bookmark->getCurrentBoard()->printBoardWithLegalMoves();
    short location = this->game->getLastMovePlayed();
    if (this->bookmark->getChild()!= nullptr){
        this->bookmark = this->bookmark->getChild();
        cout<<name<<" Bookmark is at "<<this->bookmark->getLocation().getCol()<<this->bookmark->getLocation().getRow()<<" with parent "<<this->bookmark->getParent()->getLocation().getCol()<<this->bookmark->getParent()->getLocation().getRow()<<endl;

        while (location != Utils::convertToGrid(this->bookmark->getLocation().getCol(),
                                                this->bookmark->getLocation().getRow())) {
            this->bookmark = this->bookmark->getSibling();
            //cout<<name<<" Bookmark is at "<<this->bookmark->getLocation().getCol()<<this->bookmark->getLocation().getRow()<<" with parent "<<this->bookmark->getParent()->getLocation().getCol()<<this->bookmark->getParent()->getLocation().getRow()<<endl;
        }

    }else{
        cout<<this->name<<" bookmark has no children"<<endl;
    }
    cout<<name<<" Bookmark has moved to "<<this->bookmark->getLocation().getCol()<<this->bookmark->getLocation().getRow()<<endl;
}

GameTreeNode* Agent::analyze() {
    GameTreeNode *bestMove = this->bookmark->getChild();
    GameTreeNode *tmp = this->bookmark->getChild();
    while(tmp!= nullptr) {
        Evaluate::exploit(tmp);
        if (tmp->getRank()>bestMove->getRank()){
            bestMove = tmp;
        }
        tmp = tmp->getSibling();
    }
    this->bookmark = bestMove;
    return bestMove;
}

void Agent::monteCarloParallel(GameTreeNode *node) {
    omp_set_num_threads(MAX_SWARM_SIZE);
    double start = omp_get_wtime();
#pragma omp parallel
    {
        int id = omp_get_thread_num();
#pragma omp for schedule(dynamic)
        for (int i = 0; i < NUMBER_OF_DIVES; ++i) {
            acswarm.at(id).simulate(node);
            acswarm.at(id).findWinValue();
            acswarm.at(id).backPropagate(gameTree->getRoot(), this->color);
        }
    }
    double end = omp_get_wtime();
    cout<<name<<" "<<NUMBER_OF_DIVES<<" dives in parallel ran in "<<end-start<<" seconds with a swarm size of: "<<MAX_SWARM_SIZE<<endl;
    this->diveTimes.push_back(end-start);
    //gameTree->printGametree(gameTree->getRoot());

    if (cbswarm.size()>1){
        cout<<name<< " mutating genetic algorithm"<<endl;
        short *temp;

        for(int i = 0; i < cbswarm.size()-1;++i) {

            Evaluate::mutate(cbswarm.at(i).getGeneticAlgorithm(), cbswarm.at(i+1).getGeneticAlgorithm());
            //cbswarm.at(i).setGeneticAlgorithm(temp);
        }
        cout<<name<<" done with mutation"<<endl;
    }

}

void Agent::printTreeToTxt(GameTreeNode *node) {
    string type;
    if (!mcswarm.empty()){
        type = "montecarlo";
    }else if (!acswarm.empty()){
        type = "antcolony";
    }else if (!ffswarm.empty()){
        type = "firefly";
    }else if (!cbswarm.empty()){
        type = "cuckoobird";
    }
    cout<<name<<" making " + type<<endl;
    this->treeCounter = 0;
    ofstream output;
    output.open("D:/Chris/csci480/swarmbot/"+type+".js");

    output<< "      google.charts.setOnLoadCallback(drawChart);\n"
             "      function drawChart() {\n"
             "      var "+type+"data = google.visualization.arrayToDataTable([\n"
            "          ['Move', 'Parent', 'Number of simulated wins'],\n"
            "           ['0-P0',null,0],\n";
    printTreeToTxtHelper(node->getChild(),1,0, &output);

    output<< "]);\n"
             "        tree = new google.visualization.TreeMap(document.getElementById('"+type+"_div'));\n"
             "\n"
             "        tree.draw("+type+"data, {\n"
             "          minColor: '#f00',\n"
             "          midColor: '#ddd',\n"
             "          maxColor: '#0d0',\n"
             "          headerHeight: 15,\n"
             "          fontColor: 'black',\n"
             "          showScale: true\n"
             "        });\n"
             "\n"
             "      }";
    output.close();
    cout<<name<<"done"<<endl;

}


void Agent::printTreeToTxtHelper(GameTreeNode *node, int level, int parentCount, ofstream *outfile) {
// a unique id is needed so the google charts api can build the tree. The nature of the gametree has a lot of repetitive data in different spots.
// The level is still needed so the method can stop in the right spot. There is a treeCounter as a global variable to provide the unique id.

    if (node != nullptr){
        if (level<10){
            ++treeCounter; //move the unique id up
            int temp = treeCounter; //keep the count at that spot so children can point to the right parent.
            double win = node->getWi();
            double dive = node->getNi();
            double percent;
            dive==0?percent =0: percent = win/dive*100;

            string output = "'"+to_string(treeCounter)+"-"; //unique id for node
            output += node->getLocation().getCol()+ to_string(node->getLocation().getRow())+"',"; //board location for node
            output += "'"+to_string(parentCount)+"-"; //unique id from the parent
            output += node->getParent()->getLocation().getCol()+ to_string(node->getParent()->getLocation().getRow())+"',"; //location of parent node
            output += to_string(percent)+","; //simulated wins
            *outfile<<"["+output+"],\n";

            //recurse by sibling first to get all the siblings at the same parent
            printTreeToTxtHelper(node->getSibling(),level, parentCount,  outfile);
            //when recursing by child set parentCount to the recorded temp count.
            printTreeToTxtHelper(node->getChild(),++level, temp,  outfile);

        }
    }
}

void Agent::printAverageTimeToFile() {

    string type;
    if (!mcswarm.empty()){
        type = "montecarlo";
    }else if (!acswarm.empty()){
        type = "antcolony";
    }else if (!ffswarm.empty()){
        type = "firefly";
    }else if (!cbswarm.empty()){
        type = "cuckoobird";
    }
    ofstream output;
    output.open("D:/Chris/csci480/swarmbot/"+type + to_string(MAX_SWARM_SIZE) + to_string(NUMBER_OF_DIVES) + ".txt");

    output<<accumulate(this->diveTimes.begin(), this->diveTimes.end(),0.0)/this->diveTimes.size();

}



