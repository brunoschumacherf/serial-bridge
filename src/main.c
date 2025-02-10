#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

#include "ssd1306.h"
#include "font.h"
#include "ws2812.pio.h"

#include "leds.h"
#include "buttons.h"

// LED Pins
const uint GREEN_LED_PIN = 11;
const uint BLUE_LED_PIN = 12;

// I2C Configuration
#define I2C_PORT i2c1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15
#define DISPLAY_ADDRESS 0x3C

// Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Terminal Parameters
#define MAX_ROWS 3
#define MAX_COLUMNS 16

// Terminal Buffer
char terminal_buffer[MAX_ROWS][MAX_COLUMNS + 1];
int current_row = 0;
int current_column = 0;

// Global PIO Variables
PIO pio;
uint sm;

// Display Structure
tela_t screen;

void update_terminal(tela_t *screen) {
    preencher_tela(screen, false);
    for (int i = 0; i < MAX_ROWS; i++) {
        int y = 10 + i * 9;
        desenhar_texto(screen, terminal_buffer[i], 8, y);
    }
    char green_status[10];
    char blue_status[10];
    sprintf(green_status, "V %s", gpio_get(GREEN_LED_PIN) ? "ON" : "OFF");
    sprintf(blue_status, "A %s", gpio_get(BLUE_LED_PIN) ? "ON" : "OFF");
    desenhar_texto(screen, green_status, 8, 48);
    desenhar_texto(screen, blue_status, 80, 48);
    enviar_dados_tela(screen);
}

void scroll_terminal(void) {
    for (int i = 0; i < MAX_ROWS - 1; i++) {
        strcpy(terminal_buffer[i], terminal_buffer[i + 1]);
    }
    terminal_buffer[MAX_ROWS - 1][0] = '\0';
    current_row = MAX_ROWS - 1;
    current_column = 0;
}

void initialize_gpio(void) {
    gpio_init(GREEN_LED_PIN);
    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

void initialize_pio(uint pin) {
    uint offset = pio_add_program(pio0, &ws2812_program);
    pio = pio0;
    sm = pio_claim_unused_sm(pio, false);
    ws2812_program_init(pio, sm, offset, pin, 800000, false);
}

void initialize_display(void) {
    inicializar_tela(&screen, SCREEN_WIDTH, SCREEN_HEIGHT, false, DISPLAY_ADDRESS, I2C_PORT);
    configurar_tela(&screen);
    preencher_tela(&screen, false);
    enviar_dados_tela(&screen);
}

int main(void) {
    stdio_init_all();
    sleep_ms(500);

    initialize_gpio();
    initialize_pio(PIN_LED_MTX);
    clear_leds();
    write_leds();
    i2c_init(I2C_PORT, 400 * 1000);
    initialize_display();
    initialize_buttons();

    strcpy(terminal_buffer[0], "Digite...");
    current_row = 0;
    current_column = strlen(terminal_buffer[0]);
    for (int i = 1; i < MAX_ROWS; i++) {
        terminal_buffer[i][0] = '\0';
    }
    update_terminal(&screen);

    char c;
    while (true) {
        sleep_ms(10);
        scanf("%c", &c);

        if (current_row == 0 && strcmp(terminal_buffer[0], "Digite...") == 0) {
            terminal_buffer[0][0] = '\0';
            current_column = 0;
        }

        if (c == '\n' || c == '\r') {
            if (current_row == MAX_ROWS - 1) {
                scroll_terminal();
            } else {
                current_row++;
            }
            current_column = 0;
        }
        else if (c == 0x08 || c == 127) {
            if (current_column > 0) {
                current_column--;
                terminal_buffer[current_row][current_column] = '\0';
            }
        }
        else {
            if (c >= '0' && c <= '9') {
                int num = c - '0';
                set_led_por_padrao(padrao_numeros_led[num]);
                write_leds();
            }
            terminal_buffer[current_row][current_column] = c;
            current_column++;
            terminal_buffer[current_row][current_column] = '\0';

            if (current_column >= MAX_COLUMNS) {
                if (current_row == MAX_ROWS - 1) {
                    scroll_terminal();
                } else {
                    current_row++;
                }
                current_column = 0;
            }
        }
        update_terminal(&screen);
    }
}