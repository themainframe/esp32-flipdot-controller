#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "fonts/font_4x5.h"
#include "flipdot.h"

/**
 * Render a text string to the dotboard using the 4x5 font.
 */
void render_text_4x5(dotboard_t* dotboard, int x, int y, const char* text)
{
  // For each char in the requested text
  for (int i = 0; i < strlen(text); i ++) {

    for (int c_row = 0; c_row < 5; c_row ++) {
      for (int c_col = 0; c_col < 4; c_col ++) {

          // Calculate the X and Y positions for this pixel of the character
          int col = x + c_col;
          int row = y + c_row;

          // Bounds check for this pixel
          if (col < 0 || row < 0 || col >= DOT_COLUMNS || row >= DOT_ROWS) {
            // Don't try to draw this pixel, it's out of bounds
            continue;
          }

          (*dotboard)[col][row] = (font_4x5[(int)text[i]][c_row] & (1 << (3 - c_col))) ? 1 : 0;
        }
      }
    
    // 1-dot space between letters
    x += 5;
  }
}