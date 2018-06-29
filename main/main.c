#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "flipdot.h"
#include "text.h"
#include "fill.h"

static const char* TAG = "Main";

static dotboard_t dots;

void app_main()
{
  ESP_LOGI(TAG, "Flipdot Display Controller System startup");

  // Allocate a flipdot array
  ESP_LOGI(TAG, "Allocated %d bytes for dotboard", sizeof(*dots));

  // Initialise
  flipdot_init();

  char text[] = "Flipdot Controller Mk.2 - DAMOW.NET";
  int x = DOT_COLUMNS;
  
  while (1) {

    // Unset the whole board
    fill_off(&dots);

    // Draw some text
    render_text_4x5(&dots, x, 4, text);
    
    // Write the dotboard repeatedly
    write_dotboard(&dots, false);
    x --;

    // Start scrolling again if we reach the end
    if (x == -(strlen(text) * 5)) {
      x = DOT_COLUMNS;
    }

    vTaskDelay(40 / portTICK_RATE_MS);
  }
}
