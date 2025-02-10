#include "leds.h"

// LED buffer and numeric patterns (0-9) in a 5x5 matrix
led_t led_matrix[NUM_LEDS_MTX];

const uint8_t led_number_pattern[10][5][5] = {
    [0] = { {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0} },
    [1] = { {0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 1, 0} },
    [2] = { {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0} },
    [3] = { {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0} },
    [4] = { {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0} },
    [5] = { {0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0} },
    [6] = { {0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0} },
    [7] = { {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0} },
    [8] = { {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0} },
    [9] = { {0, 1, 1, 1, 0}, {0, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0} }
};

void set_led(uint id, uint8_t R, uint8_t G, uint8_t B) {
    led_matrix[id].R = R;
    led_matrix[id].G = G;
    led_matrix[id].B = B;
}

void clear_leds(void) {
    for (uint i = 0; i < NUM_LEDS_MTX; i++) {
        led_matrix[i].R = 0;
        led_matrix[i].G = 0;
        led_matrix[i].B = 0;
    }
}

uint32_t rgb_value(uint8_t B, uint8_t R, uint8_t G) {
    return (G << 24) | (R << 16) | (B << 8);
}

// 'pio' and 'sm' are defined in main
extern PIO pio;
extern uint sm;

void write_leds(void) {
    uint32_t value;
    for (uint i = 0; i < NUM_LEDS_MTX; ++i) {
        value = rgb_value(led_matrix[i].B, led_matrix[i].R, led_matrix[i].G);
        pio_sm_put_blocking(pio, sm, value);
    }
}

void set_led_by_pattern(const uint8_t pattern[5][5]) {
    uint level = LED_MATRIX_LEVEL;
    int idx = 0;
    // Iterate from bottom to top in a zigzag pattern
    for (int row = 4; row >= 0; row--) {
        bool reverse = (row % 2 == 0);
        if (reverse) {
            for (int col = 4; col >= 0; col--) {
                if (pattern[row][col])
                    set_led(idx, 0, 0, level);
                else
                    set_led(idx, 0, 0, 0);
                idx++;
            }
        } else {
            for (int col = 0; col < 5; col++) {
                if (pattern[row][col])
                    set_led(idx, 0, 0, level);
                else
                    set_led(idx, 0, 0, 0);
                idx++;
            }
        }
    }
}
