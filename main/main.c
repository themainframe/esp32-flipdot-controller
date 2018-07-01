#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "xtensa/core-macros.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "flipdot.h"
#include "fill.h"
#include "main.h"
#include "scroll.h"
#include "snake.h"

static const char* TAG = "Main";

static bool button_held = false;

// Mode state
static sys_mode_t mode = MODE_SCROLL;

void app_main()
{
    ESP_LOGI(TAG, "Flipdot Display Controller System startup");

    // Set up the button input
    gpio_set_direction(PIN_BUTTON, GPIO_MODE_INPUT);

    // Initialise display
    flipdot_init();

    // Initialise snake game
    snake_init();

    // A clean board to write after mode changes
    dotboard_t clean_board;
    fill_off(&clean_board);

    // Continually update the display
    while (1) {

        // Detect mode changes
        if (!gpio_get_level(PIN_BUTTON)) {
            button_held = true;
        } else {
            if (button_held) {
                // Button has now been released, enact the change
                mode = mode == MODE_MAX - 1 ? 0 : mode + 1;
                write_dotboard(&clean_board, false);
                ESP_LOGI(TAG, "Changed mode to %d", mode);
                button_held = false;
            }
        }

        switch (mode) {
        
        case MODE_SCROLL:
            scroll_update();
            break;

        case MODE_SNAKE:
            snake_update();
            break;

        default:
            break;
            
        }

        vTaskDelay(50 / portTICK_RATE_MS);
    }
}
