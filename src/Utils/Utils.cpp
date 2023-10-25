//
// Created by cblah on 3/4/2022.
//

#include "../../includes/Utils.h"

short Utils::convertToGrid(short col, short row) { //example c4
    if (col == 'P'){
        return PASS;
    }
    col-=97; // col is a character ex:2
    row-=1;  //adjustment for 1 indexing ex:3
    col=7-col; //adjustment for bitboard ex:5
    row=7-row;  //adjustment for bitboard ex:4
    //cout<< "C convertToGrid returns "<<row*8+col<<endl;
    return row*8+col; //returns the shift needed to place a piece in the right spot on a bitboard ex:37
}

void Utils::printGridLocation(short location) {
    short num = location%8;
    short row = location/8;
    num=(7-num);
    row=(7-row);
    ++row;
    char col = num+97;

    if (location == 27){
        cout<< " "<<endl;
    }else{
        cout<<  col << " " << row << endl;
    }


}

void Utils::printPlayerColor(short color) {
    color == 1? cout<< "Black ":cout<<"White ";

}