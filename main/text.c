#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "fonts/font_4x5.h"
#include "flipdot.h"

/**
 * Render a text string to the dotboard using the 4x5 font.
 */
void render_text_4x5(dotboard_t* dotboard, uint8_t x, uint8_t y, char* text)
{
  // Draw the requested text
  for (uint8_t i = 0; i < strlen(text); i ++) {
    for (uint8_t c = x; c < x + 4; c ++) {
      for (uint8_t r = y; r < y + 5; r ++) {
        uint8_t c_row = r - y;
        uint8_t c_col = c - x;
        *dotboard[c][r] = (font_4x5[(int)text[i]][c_row] & (1 << (3 - c_col))) ? 1 : 0;
      }
    }
    
    // 1-dot space between letters
    x += 5;
  }
}