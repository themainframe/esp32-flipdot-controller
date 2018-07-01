#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "xtensa/core-macros.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "flipdot.h"

static const char* TAG = "FD";

static bool dotboard_synced = false;
static dotboard_t last_written_dotboard;

/**
 * Initialse the Flipdot Controller interface.
 */
void flipdot_init()
{
    // Set up the GPIO pins for the signalling
    ESP_LOGI(TAG, "setting up GPIO pins");

    // Use pins for GPIO
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[PIN_LED_YELLOW], PIN_FUNC_GPIO);
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[PIN_LED_GREEN], PIN_FUNC_GPIO);

    // All outputs
    gpio_set_direction(PIN_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_COLUMN_ADVANCE, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_RESET, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_SET_UNSET, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_ROW_ADVANCE, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_COIL_DRIVE, GPIO_MODE_OUTPUT);
    
    // LEDs
    gpio_set_direction(PIN_LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_LED_YELLOW, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_LED_RED, GPIO_MODE_OUTPUT);

    // All pins start low
    gpio_set_level(PIN_ENABLE, 0);
    gpio_set_level(PIN_COLUMN_ADVANCE, 0);
    gpio_set_level(PIN_RESET, 0);
    gpio_set_level(PIN_SET_UNSET, 0);
    gpio_set_level(PIN_ROW_ADVANCE, 0);
    gpio_set_level(PIN_COIL_DRIVE, 0);
    gpio_set_level(PIN_LED_GREEN, 0);
    gpio_set_level(PIN_LED_YELLOW, 0);
    gpio_set_level(PIN_LED_RED, 0);

    // Create space for a backing store for the last-written dotboard
    for (uint c = 0; c < DOT_COLUMNS; c ++) {
        for (uint r = 0; r < DOT_ROWS; r ++) {
            last_written_dotboard[c][r] = 0;
        }
    }

    gpio_set_level(PIN_LED_GREEN, 1);
}

// Insert a wait
static void wait(uint wait_us)
{
    uint32_t c_current = 0, c_start = XTHAL_GET_CCOUNT();
    do {
        c_current = XTHAL_GET_CCOUNT();
    } while (c_current - c_start < CYCLES_PER_US * wait_us);
}

// Setup the dot state
static void setup_state(bool dot_state)
{
    gpio_set_level(PIN_SET_UNSET, dot_state);
}

// Reset
static void reset()
{
    gpio_set_level(PIN_RESET, 1);
    wait(1);
    gpio_set_level(PIN_RESET, 0);
}

// Advance 1 row
static void row_advance()
{
    gpio_set_level(PIN_ROW_ADVANCE, 1);
    wait(1);
    gpio_set_level(PIN_ROW_ADVANCE, 0);
}

// Advance 1 column
static void column_advance()
{
    gpio_set_level(PIN_COLUMN_ADVANCE, 1);
    wait(1);
    gpio_set_level(PIN_COLUMN_ADVANCE, 0);
}

// Pulse the coil
static void pulse_coil()
{
    gpio_set_level(PIN_COIL_DRIVE, 1);
    wait(PULSE_TIME_US);
    gpio_set_level(PIN_COIL_DRIVE, 0);
}

// Enable
static void enable()
{
    gpio_set_level(PIN_ENABLE, 1);
}

// Disable
static void disable()
{
    gpio_set_level(PIN_ENABLE, 0);
}

/**
 * Write an entire dotboard.
 * 
 * Only dots that need to change state will be pulsed unless is_keyframe is stated.
 * In calling code, I'd ensure is_keyframe is occasionally set in case the dotboard is
 * ever disconnected from the controller for a moment.
 */
void write_dotboard(dotboard_t* dots, bool is_keyframe)
{
    gpio_set_level(PIN_LED_RED, 1);

    // Force an update if we don't have a dotboard buffer yet
    is_keyframe = dotboard_synced ? is_keyframe : true;

    // Enable while updating
    enable();
    wait(1);

    // Reset
    reset();
    wait(1);

    // For each column
    for (uint c = 0; c < DOT_COLUMNS; c ++) {

        // For each row in the column...
        for (uint r = 0; r < DOT_ROWS; r ++) {
        
        // Skip dots that don't need updating
        if (is_keyframe || (*dots)[c][r] != last_written_dotboard[c][r]) {

            // Set up the set/unset pin
            setup_state((*dots)[c][r]);
            wait(1);

            // Pulse the coil drive
            pulse_coil();

        }

        // Advance the row
        row_advance();
        wait(1);
        }

        // Advance the column
        column_advance();
        wait(1);

    }

    // Disable when done
    disable();
    gpio_set_level(PIN_LED_RED, 0);

    // Update the buffer with the new dotboard
    for (uint c = 0; c < DOT_COLUMNS; c ++) {
        for (uint r = 0; r < DOT_ROWS; r ++) {
        last_written_dotboard[c][r] = (*dots)[c][r];
        }
    }

    dotboard_synced = true;
}
