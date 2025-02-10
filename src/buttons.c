#include "buttons.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "ssd1306.h"
#include "leds.h"

// Variáveis globais para debounce e estados dos botões
volatile uint32_t last_interrupt_time = 0;
volatile bool green_led_on = false;
volatile bool blue_led_on = false;
volatile int selected_number = -1;

// Declarações externas de funções e variáveis de outros módulos
extern void draw_text(screen_t *screen, const char *text, uint8_t x, uint8_t y);
extern void send_screen_data(screen_t *screen);
extern void set_led_default_pattern(const uint8_t pattern[5][5]);
extern void write_leds(void);
extern const uint8_t number_led_patterns[10][5][5];
extern screen_t screen;
extern const uint PIN_GREEN_LED;
extern const uint PIN_BLUE_LED;

void button_callback(uint gpio, uint32_t events) {
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_interrupt_time > DEBOUNCE_DELAY_MS) {
        last_interrupt_time = now;
        if (gpio == BUTTON_A_PIN) {
            green_led_on = !green_led_on;
            gpio_put(PIN_GREEN_LED, green_led_on);
            draw_text(&screen, green_led_on ? "G ON " : "G OFF", 8, 48);
            printf(green_led_on ? "Green LED ON\n" : "Green LED OFF\n");
        } else if (gpio == BUTTON_B_PIN) {
            blue_led_on = !blue_led_on;
            gpio_put(PIN_BLUE_LED, blue_led_on);
            draw_text(&screen, blue_led_on ? "B ON " : "B OFF", 80, 48);
            printf(blue_led_on ? "Blue LED ON\n" : "Blue LED OFF\n");
        }
        // Se um número (0-9) estiver selecionado, atualiza os LEDs com o padrão correspondente
        if (selected_number >= 0 && selected_number <= 9) {
            set_led_default_pattern(number_led_patterns[selected_number]);
            write_leds();
        }
        send_screen_data(&screen);
    }
}

void initialize_buttons(void) {
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, button_callback);
}
