#ifndef BUTTONS_H
#define BUTTONS_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Pinos dos botões
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Atraso para debounce (ms)
#define DEBOUNCE_DELAY_MS 200

// Variáveis globais para controle dos botões
extern volatile uint32_t last_interrupt_time;
extern volatile bool green_led_on;
extern volatile bool blue_led_on;
extern volatile int selected_number;

// Protótipos das funções do módulo de botões
void initialize_buttons(void);
void button_callback(uint gpio, uint32_t events);

#endif // BUTTONS_H
