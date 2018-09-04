#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/uart.h"
#include "flipdot.h"
#include "snake.h"
#include "fill.h"
#include "text.h"

static const char* TAG = "Snake";

// The dotboard for the snake mode
static dotboard_t dots;

// Define our snake
static snake_t snake;

// Define a treat
static coordinate_t treat;

/**
 * Listen for keystrokes on the UART and update the snake direction.
 */
static void update_direction_user(snake_t* snake)
{
    uint8_t read_char;
    STATUS s = uart_rx_one_char(&read_char);
    if (s == OK) {
        // Change the direction as appropriate
        switch (read_char) {
            case 'w':
                if (snake->direction != DIRECTION_DOWN) {
                    snake->direction = DIRECTION_UP;
                    ESP_LOGI(TAG, "Changed direction to UP");
                }
                break;
            case 'a':
                if (snake->direction != DIRECTION_RIGHT) {
                    snake->direction = DIRECTION_LEFT;
                    ESP_LOGI(TAG, "Changed direction to LEFT");
                }
                break;
            case 's':
                if (snake->direction != DIRECTION_UP) {
                    snake->direction = DIRECTION_DOWN;
                    ESP_LOGI(TAG, "Changed direction to DOWN");
                }
                break;
            case 'd':
                if (snake->direction != DIRECTION_LEFT) {
                    snake->direction = DIRECTION_RIGHT;
                    ESP_LOGI(TAG, "Changed direction to RIGHT");
                }
                break;
        }
    }
}


/**
 * Use a fill algorithm to decide the next best direction change.
 */
static void update_direction_ai(snake_t* snake)
{
    uint8_t read_char;
    STATUS s = uart_rx_one_char(&read_char);
    if (s == OK) {
        // Change the direction as appropriate
        switch (read_char) {
            case 'w':
                if (snake->direction != DIRECTION_DOWN) {
                    snake->direction = DIRECTION_UP;
                    ESP_LOGI(TAG, "Changed direction to UP");
                }
                break;
            case 'a':
                if (snake->direction != DIRECTION_RIGHT) {
                    snake->direction = DIRECTION_LEFT;
                    ESP_LOGI(TAG, "Changed direction to LEFT");
                }
                break;
            case 's':
                if (snake->direction != DIRECTION_UP) {
                    snake->direction = DIRECTION_DOWN;
                    ESP_LOGI(TAG, "Changed direction to DOWN");
                }
                break;
            case 'd':
                if (snake->direction != DIRECTION_LEFT) {
                    snake->direction = DIRECTION_RIGHT;
                    ESP_LOGI(TAG, "Changed direction to RIGHT");
                }
                break;
        }
    }
}

/**
 * Advance the snake position by one tick.
 */
static void update_position(snake_t* snake)
{   
    // Capture the previous head position
    coordinate_t old_head;
    old_head.x = snake->head.x;
    old_head.y = snake->head.y;

    // Update with the new head position based on direction
    snake->head.y -= (snake->direction >> 0) & 1;
    snake->head.y += (snake->direction >> 1) & 1;
    snake->head.x -= (snake->direction >> 2) & 1;
    snake->head.x += (snake->direction >> 3) & 1;

    // Shift the tail along
    for (int tail_i = snake->tail_length; tail_i > 0; tail_i --) {
        snake->tail[tail_i] = snake->tail[tail_i - 1];
    }

    // Change the tail segment right next to the head to be the "old" head
    snake->tail[0] = old_head;
}

/**
 * Randomly place a treat on the board.
 */
static void place_treat()
{
    treat.x = 1 + (rand() % (DOT_COLUMNS - 2));
    treat.y = 1 + (rand() % (DOT_ROWS - 2));
    ESP_LOGI(TAG, "Treat placed @ %d x %d", treat.x, treat.y);
}

/**
 * Detect collisions with walls, snake tail segments and treats.
 */
static void detect_collisions(snake_t* snake)
{
    // We extend by one if we eat a treat
    if (snake->head.x == treat.x && snake->head.y == treat.y) {
        snake->tail_length ++;
        place_treat();
        ESP_LOGI(TAG, "Ate a treat, tail length extended to %d", snake->tail_length);
    }

    #ifdef TOROIDAL

        // Implement a toroidal map
        if (snake->head.x == 0 && snake->direction == DIRECTION_LEFT) {
            snake->head.x = DOT_COLUMNS - 1;
        }
        if (snake->head.x == DOT_COLUMNS && snake->direction == DIRECTION_RIGHT) {
            snake->head.x = 0;
        }
        if (snake->head.y == 0 && snake->direction == DIRECTION_UP) {
            snake->head.y = DOT_ROWS - 1;
        }
        if (snake->head.y == DOT_ROWS && snake->direction == DIRECTION_DOWN) {
            snake->head.y = 0;
        }

    #else

        // We're dead if we touch the walls
        if (
            (snake->head.x == 0 && snake->direction == DIRECTION_LEFT) ||
            (snake->head.x == DOT_COLUMNS - 1 && snake->direction == DIRECTION_RIGHT) ||
            (snake->head.y == 0 && snake->direction == DIRECTION_UP) ||
            (snake->head.y == DOT_ROWS - 1 && snake->direction == DIRECTION_DOWN)
        ) {
            ESP_LOGI(TAG, "Wall collision detected - snake killed");
            snake->is_dead = true;
            return;
        }

    #endif

    // We're dead if we touch our tail
    for (int tail_i = 0; tail_i < snake->tail_length; tail_i ++) {
        if (snake->head.x == snake->tail[tail_i].x && snake->head.y == snake->tail[tail_i].y) {
            ESP_LOGI(TAG, "Tail collision detected - snake killed");
            snake->is_dead = true;
            return;
        }
    }

}

/**
 * Initialise the snake game.
 */
void snake_init()
{
    snake.is_dead = false;
    snake.head.x = DOT_COLUMNS / 2;
    snake.head.y = DOT_ROWS / 2;
    snake.direction = DIRECTION_RIGHT;
    snake.tail_length = 3;
    place_treat();
}

/**
 * We've died, show the score and check for any key to resume
 */
void death_screen()
{
    // Draw "GAME OVER" text
    fill_off(&dots);
    render_text_4x5(&dots, 0, 1, "GAME");
    render_text_4x5(&dots, 1, 7, "OVER");
    write_dotboard(&dots, false);

    uint8_t read_char;
    STATUS s = uart_rx_one_char(&read_char);
    if (s == OK) {
        ESP_LOGI(TAG, "Starting new game!");
        snake_init();
    }
}

void snake_update()
{
    // Detect any collisions that occurred in the last frame
    if (!snake.is_dead) {
        // Alter direction
        update_direction_user(&snake);
        detect_collisions(&snake);
    }

    if (snake.is_dead) {
        death_screen();
        return;
    }

    // Clear the frame-buffer
    fill_off(&dots);

    // Advance the snake position
    update_position(&snake);

    // Draw the snake head to the dotboard
    dots[snake.head.x][snake.head.y] = 1;

    // Draw the snake tail to the dotboard
    for (int tail_i = 0; tail_i < snake.tail_length; tail_i ++) {
        dots[snake.tail[tail_i].x][snake.tail[tail_i].y] = 1;
    }

    // Draw the treat
    dots[treat.x][treat.y] = 1;

    // Draw the dotboard
    write_dotboard(&dots, false);

    // Inter-frame delay inversely proportional to the tail length
    vTaskDelay((100 - snake.tail_length * 2) / portTICK_RATE_MS);
}
