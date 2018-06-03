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

static const char* TAG = "Main";

void app_main()
{
  ESP_LOGI(TAG, "Flipdot Display Controller System startup");

  // Allocate a flipdot array
  dotboard_t* dots = calloc(1, sizeof(dotboard_t));
  ESP_LOGI(TAG, "Allocated %d bytes for dotboard", sizeof(*dots));

  // Initialise
  flipdot_init();

  uint i = 0;
  while (1) {

    // Set or unset the whole board
    for (uint c = 0; c < DOT_COLUMNS; c ++) {
      for (uint r = 0; r < DOT_ROWS; r ++) {
        *dots[c][r] = !(i % 20 == c);
      }
    }

    // Write the dotboard repeatedly
    ESP_LOGI(TAG, "Writing out dotboard...");
    write_dotboard(dots);
    
    i ++;
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}
