//
// Created by cblah on 3/4/2022.
//

#pragma once

#include "Constants.h"
#include <cstdint>
#include <iostream>

namespace Othello {
namespace Utils {

/**
 * Convert column letter and row number to grid position (0-63)
 * @param col Column letter ('a'-'h' or 'A'-'H')
 * @param row Row number (1-8)
 * @return Grid position (0-63)
 */
int16_t convertToGrid(char col, int16_t row);

/**
 * Print grid location in human-readable format (e.g., "e4")
 * @param location Grid position (0-63)
 */
void printGridLocation(int16_t location);

/**
 * Print player color to console
 * @param player The player color
 */
void printPlayerColor(Player player);

/**
 * Convert grid position to column letter
 * @param location Grid position (0-63)
 * @return Column letter ('a'-'h')
 */
char gridToColumn(int16_t location);

/**
 * Convert grid position to row number
 * @param location Grid position (0-63)
 * @return Row number (1-8)
 */
int16_t gridToRow(int16_t location);

} // namespace Utils
} // namespace Othello