#include <stdlib.h>
#include "flipdot.h"

/**
 * Set the entire dotboard.
 */
void fill_on(dotboard_t* dotboard)
{
  for (uint8_t c = 0; c < DOT_COLUMNS; c ++) {
    for (uint8_t r = 0; r < DOT_ROWS; r ++) {
      (*dotboard)[c][r] = 1;
    }
  }
}

/**
 * Unset the entire dotboard.
 */
void fill_off(dotboard_t* dotboard)
{
  for (uint8_t c = 0; c < DOT_COLUMNS; c ++) {
    for (uint8_t r = 0; r < DOT_ROWS; r ++) {
      (*dotboard)[c][r] = 0;
    }
  }
}