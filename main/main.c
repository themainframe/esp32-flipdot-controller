#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "flipdot.h"
#include "text.h"

static const char* TAG = "Main";

static dotboard_t dots;

void app_main()
{
  ESP_LOGI(TAG, "Flipdot Display Controller System startup");

  // Allocate a flipdot array
  ESP_LOGI(TAG, "Allocated %d bytes for dotboard", sizeof(*dots));

  // Initialise
  flipdot_init();

  uint i = 0;
  char text[4] = "";
  while (1) {

    // Unset the whole board
    for (uint c = 0; c < DOT_COLUMNS; c ++) {
      for (uint r = 0; r < DOT_ROWS; r ++) {
        dots[c][r] = i == c ? 1 : 0;
      }
    }

    // Draw some text
    sprintf(text, "%X", i);
    // render_text_4x5(&dots, 3, 3, text);
    
    // Write the dotboard repeatedly
    ESP_LOGI(TAG, "Writing dotboard %d", i);
    write_dotboard(&dots, false);
    
    i ++;
    vTaskDelay(1000 / portTICK_RATE_MS);

  }
}
