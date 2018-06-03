#include "esp_log.h"
#include "xtensa/core-macros.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "flipdot.h"

static const char* TAG = "FD";

/**
 * Initialse the Flipdot Controller interface.
 */
void flipdot_init()
{
  // Set up the GPIO pins for the signalling
  ESP_LOGI(TAG, "setting up GPIO pins");

  // All pins start low
  gpio_set_level(PIN_ENABLE, 0);
  gpio_set_level(PIN_COLUMN_ADVANCE, 0);
  gpio_set_level(PIN_RESET, 0);
  gpio_set_level(PIN_SET_UNSET, 0);
  gpio_set_level(PIN_ROW_ADVANCE, 0);
  gpio_set_level(PIN_COIL_DRIVE, 0);

  // All outputs
  gpio_set_direction(PIN_ENABLE, GPIO_MODE_OUTPUT);
  gpio_set_direction(PIN_COLUMN_ADVANCE, GPIO_MODE_OUTPUT);
  gpio_set_direction(PIN_RESET, GPIO_MODE_OUTPUT);
  gpio_set_direction(PIN_SET_UNSET, GPIO_MODE_OUTPUT);
  gpio_set_direction(PIN_ROW_ADVANCE, GPIO_MODE_OUTPUT);
  gpio_set_direction(PIN_COIL_DRIVE, GPIO_MODE_OUTPUT);
}

/**
 * Insert a wait.
 */
void wait(uint wait_us)
{
  uint32_t c_current = 0, c_start = XTHAL_GET_CCOUNT();
  do {
    c_current = XTHAL_GET_CCOUNT();
  } while (c_current - c_start < CYCLES_PER_US * wait_us);
}
           
/**
 * Write a column.
 */
void write_column(column_t* column)
{
  // For each row in the column...
  for (uint r = 0; r < DOT_ROWS; r ++) {
   
    // Set up the set/unset pin
    gpio_set_level(PIN_SET_UNSET, !(*column[r]));
    wait(1);

    // Pulse the coil drive
    gpio_set_level(PIN_COIL_DRIVE, 1);
    wait(PULSE_TIME_US);
    gpio_set_level(PIN_COIL_DRIVE, 0);
    wait(1);

    // Advance the row
    gpio_set_level(PIN_ROW_ADVANCE, 1);
    wait(1);
    gpio_set_level(PIN_ROW_ADVANCE, 0);

  }

  // Advance the column
  gpio_set_level(PIN_COLUMN_ADVANCE, 1);
  wait(1);
  gpio_set_level(PIN_COLUMN_ADVANCE, 0);
  wait(1);
}

/**
 * Write an entire dotboard.
 */
void write_dotboard(dotboard_t* dots)
{
  // Enable while updating
  gpio_set_level(PIN_ENABLE, 1);
  wait(1);

  // Reset
  gpio_set_level(PIN_RESET, 1);
  wait(1);
  gpio_set_level(PIN_RESET, 0);
  wait(1);

  // For each column
  for (uint c = 0; c < DOT_COLUMNS; c ++) {
      write_column(&(*dots[c]));
  }

  gpio_set_level(PIN_ENABLE, 0);
}
