//
// Created by cblah on 3/4/2022.
//

#include "../includes/Move.h"

namespace Othello {

Move::Move() : col('a'), row(1) {
}

Move::Move(char col, int16_t row) : col(col), row(row) {
}

} // namespace Othello
