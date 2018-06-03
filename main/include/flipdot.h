#ifndef FLIPDOT_H
#define FLIPDOT_H

#include <stdint.h>

// Dimensions
#define DOT_ROWS 14
#define DOT_COLUMNS 20

// GPIO Pin Definitions
#define PIN_ENABLE 0
#define PIN_COLUMN_ADVANCE 4
#define PIN_RESET 32
#define PIN_SET_UNSET 2
#define PIN_ROW_ADVANCE 16
#define PIN_COIL_DRIVE 17

// Cycle count in 1 µS
#define CYCLES_PER_US 600

// Timings (µS)
#define PULSE_TIME_US 150

// Types
typedef uint8_t column_t[DOT_COLUMNS];
typedef column_t dotboard_t[DOT_ROWS];

// Public subs
void flipdot_init();
void write_dotboard(dotboard_t* dots);

#endif