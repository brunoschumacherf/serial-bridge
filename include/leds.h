#ifndef LEDS_H
#define LEDS_H

#include "pico/stdlib.h"
#include "hardware/pio.h"

// LED matrix settings
#define NUM_LEDS_MTX 25
#define LED_MATRIX_LEVEL 51
#define LED_MATRIX_PIN 7

extern const uint LED_GREEN_PIN;
extern const uint LED_BLUE_PIN;

// Structure to store LED color (GRB format)
typedef struct {
    uint8_t R, G, B;
} led_t;

// Global LED module variables
extern led_t led_matrix[NUM_LEDS_MTX];
// Numeric patterns (0-9) in a 5x5 matrix
extern const uint8_t led_number_pattern[10][5][5];

// LED control function prototypes
void set_led(uint id, uint8_t R, uint8_t G, uint8_t B);
void clear_leds(void);
uint32_t rgb_value(uint8_t B, uint8_t R, uint8_t G);
void write_leds(void);
void set_led_by_pattern(const uint8_t pattern[5][5]);

#endif // LEDS_H
