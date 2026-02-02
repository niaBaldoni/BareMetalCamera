#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

// #define I2C_PORT i2c0
#define I2C_SDA_PIN 12
#define I2C_SCL_PIN 13

#define CLK         11

#define PIN_RST     14
#define PIN_PWDN    15

// OV5640 SCCB address
#define OV5640_ADDR 0x3C

// OV5640 ID registers
#define OV5640_CHIP_ID_HIGH 0x300A
#define OV5640_CHIP_ID_LOW  0x300B

extern i2c_inst_t i2c0_inst;

void xclk_init(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);

    pwm_set_clkdiv(slice, 1.0f);   // system clock (125 MHz)
    pwm_set_wrap(slice, 1);        // divide by 2 -> ~62.5 MHz / 2 ≈ 31 MHz
    pwm_set_chan_level(slice, channel, 1);

    pwm_set_enabled(slice, true);
}

uint8_t ov5640_read_reg(i2c_inst_t* ptrI2c, uint16_t reg) {
    int32_t ret = 0;

    uint8_t buf[2];
    buf[0] = (reg >> 8) & 0xFF;  // high byte
    buf[1] = reg & 0xFF;         // low byte

    // Write register address (no stop)
    ret = i2c_write_blocking(ptrI2c, OV5640_ADDR, buf, 2, true);

    // Read 1 byte
    uint8_t value;
    ret = i2c_read_blocking(ptrI2c, OV5640_ADDR, &value, 1, false);


    return value;
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
    
    sleep_ms(20);  // Let camera wake up
}

int main() {
    stdio_init_all();

    // Give USB time to connect
    sleep_ms(5000);

    // 
    init_camera_power();
    xclk_init(CLK);
    sleep_ms(10);

    // Initialize I2C
    /*
    Test I2C init
    */
    i2c_inst_t* ptrI2c = &i2c0_inst;
    i2c_init(ptrI2c, 100 * 1000);
    // i2c_init(I2C_PORT, 100 * 1000); // 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(I2C_SDA_PIN);
    // gpio_pull_up(I2C_SCL_PIN);

    printf("OV5640 SCCB test\n");

    uint8_t cidh = ov5640_read_reg(ptrI2c, OV5640_CHIP_ID_HIGH);
    uint8_t cidl = ov5640_read_reg(ptrI2c, OV5640_CHIP_ID_LOW);

    printf("Chip ID High Byte: 0x%02X\n", cidh);
    printf("Chip ID Low Byte: 0x%02X\n", cidl);
    printf("Chip ID: 0x%02X%02X\n", cidh, cidl);

    while (1) {
        sleep_ms(1000);
    }
}