#ifndef FLIPDOT_H
#define FLIPDOT_H

#include <stdint.h>

// Dimensions
#define DOT_ROWS 14
#define DOT_COLUMNS 20

// GPIO Pin Definitions
#define PIN_ENABLE 21
#define PIN_COLUMN_ADVANCE 18
#define PIN_RESET 19
#define PIN_SET_UNSET 17
#define PIN_ROW_ADVANCE 5
#define PIN_COIL_DRIVE 4

#define PIN_LED_RED 2
#define PIN_LED_YELLOW 15
#define PIN_LED_GREEN 13

// Cycle count in 1 µS
#define CYCLES_PER_US 600

// Timings (µS)
#define PULSE_TIME_US 200

// Types
typedef uint8_t column_t[DOT_ROWS];
typedef column_t dotboard_t[DOT_COLUMNS];

// Public subs
void flipdot_init();
void write_dotboard(dotboard_t* dots, bool is_keyframe);

#endif