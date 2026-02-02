#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

// === PIN DEFINITIONS ===

#define PIN_D2          0
#define PIN_D3          1
#define PIN_D4          2
#define PIN_D5          3
#define PIN_D6          4
#define PIN_D7          5
#define PIN_D8          6
#define PIN_D9          7
#define PIN_PCLK        8
#define PIN_VSYNC       9
#define PIN_HREF        10
#define PIN_XCLK        11
#define PIN_SCCB_SDA    12
#define PIN_SCCB_SCL    13
#define PIN_RST         14
#define PIN_PWDN        15

// OV5640 ID registers
#define OV5640_CHIP_ID_HIGH 0x300A
#define OV5640_CHIP_ID_LOW  0x300B

// === INITS ===

void init_xclk() {
    gpio_set_function(PIN_XCLK, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(PIN_XCLK);
    uint channel = pwm_gpio_to_channel(PIN_XCLK);

    pwm_set_clkdiv(slice, 1.0f);   // system clock (125 MHz)
    pwm_set_wrap(slice, 1);        // divide by 2 -> ~62.5 MHz / 2 ≈ 31 MHz CHECK THIS
    pwm_set_chan_level(slice, channel, 1);

    pwm_set_enabled(slice, true);
    sleep_ms(20);
}

void init_i2c() {
    i2c_init(i2c0, 100 * 1000); // 100kHz -> CHECK THIS
    gpio_set_function(PIN_SCCB_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SCCB_SCL, GPIO_FUNC_I2C);
    sleep_ms(20);
}

void init_camera_power() {
    // RESET is active LOW - set HIGH for normal operation
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);
    gpio_put(PIN_RST, 1);
    
    // PWDN is active HIGH - set LOW for normal operation
    gpio_init(PIN_PWDN);
    gpio_set_dir(PIN_PWDN, GPIO_OUT);
    gpio_put(PIN_PWDN, 0);
    
    sleep_ms(20);
}

// === READ & WRITE REGISTERS ===

void camera_write_reg(uint16_t reg, uint8_t value) {
    uint8_t data[3];
    data[0] = (reg >> 8) & 0xFF;  // High byte
    data[1] = reg & 0xFF;         // Low byte  
    data[2] = value;
    i2c_write_blocking(i2c0, 0x3C, data, 3, false);
}

uint8_t camera_read_reg(uint16_t reg) {
    uint8_t addr[2];
    addr[0] = (reg >> 8) & 0xFF;
    addr[1] = reg & 0xFF;
    
    uint8_t value;
    i2c_write_blocking(i2c0, 0x3C, addr, 2, true);   // Keep control
    i2c_read_blocking(i2c0, 0x3C, &value, 1, false);
    return value;
}

// === TESTS ===

void test_chip_id() {
    uint8_t id_high = camera_read_reg(0x300A);
    uint8_t id_low = camera_read_reg(0x300B);
    
    if (id_high == 0x56 && id_low == 0x40) {
        printf("OV5640 detected!\n");
    } else {
        printf("Wrong chip ID!\n");
    }
}

// === MAIN ===

int main() {
    stdio_init_all();

    // Give USB time to connect
    sleep_ms(5000);

    init_camera_power();
    init_xclk();
    printf("XCLK started!\n");

    init_i2c();
    printf("I2C ready!\n");

    test_chip_id();

    printf("Testing register write...\n");
    camera_write_reg(0x4300, 0x60);  // Set RGB565 format
    printf("Format register set to RGB565!\n");
    uint8_t format = camera_read_reg(0x4300);

    if (format == 0x60) {
        printf("Confirmed: format register set to RGB565!\n");
    } else {
        printf("Something went wrong: format register is NOT set to RGB565.\n");
    }

    printf("All done! Looping forever...\n");
    while (1) {
        sleep_ms(1000);
    }
}