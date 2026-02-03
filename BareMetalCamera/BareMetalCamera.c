#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

// === PIN DEFINITIONS ===

#define CAM_D2          0
#define CAM_D3          1
#define CAM_D4          2
#define CAM_D5          3
#define CAM_D6          4
#define CAM_D7          5
#define CAM_D8          6
#define CAM_D9          7
#define CAM_PCLK        8
#define CAM_VSYNC       9
#define CAM_HREF        10
#define CAM_XCLK        11
#define CAM_SCCB_SDA    12
#define CAM_SCCB_SCL    13
#define CAM_RST         14
#define CAM_PWDN        15

// OV5640 ID registers
#define OV5640_CHIP_ID_HIGH 0x300A
#define OV5640_CHIP_ID_LOW  0x300B

// === INITS ===

void init_xclk() {
    gpio_set_function(CAM_XCLK, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(CAM_XCLK);
    uint channel = pwm_gpio_to_channel(CAM_XCLK);

    pwm_set_clkdiv(slice, 2.604f);
    pwm_set_wrap(slice, 1);
    pwm_set_chan_level(slice, channel, 1);
    pwm_set_enabled(slice, true);

    sleep_ms(20);
}

void init_i2c() {
    i2c_init(i2c0, 100 * 1000); // 100kHz -> CHECK THIS
    gpio_set_function(CAM_SCCB_SDA, GPIO_FUNC_I2C);
    gpio_set_function(CAM_SCCB_SCL, GPIO_FUNC_I2C);
    sleep_ms(20);
}

void camera_set_power(bool enable) {
    gpio_init(CAM_PWDN);
    gpio_set_dir(CAM_PWDN, GPIO_OUT);

    if (enable) {
        gpio_put(CAM_PWDN, 0);  // normal operation
    } else {
        gpio_put(CAM_PWDN, 1);  // power-down
    }

    sleep_ms(5);
}

void camera_hard_reset(void) {
    gpio_init(CAM_RST);
    gpio_set_dir(CAM_RST, GPIO_OUT);

    // Assert reset (active low)
    gpio_put(CAM_RST, 0);
    sleep_ms(2);   // datasheet: >=1ms

    // Deassert reset
    gpio_put(CAM_RST, 1);
    sleep_ms(20);  // let it stabilize
}

void init_capture_pins() {
    // Data pins D2-D9 (GPIO 7-14) as inputs
    for (int pin = CAM_D2; pin <= CAM_D9; pin++) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
    }
    
    // Control pins as inputs  
    gpio_init(CAM_VSYNC); gpio_set_dir(CAM_VSYNC, GPIO_IN);
    gpio_init(CAM_HREF);  gpio_set_dir(CAM_HREF, GPIO_IN);
    gpio_init(CAM_PCLK);  gpio_set_dir(CAM_PCLK, GPIO_IN);
    
    printf("Capture pins ready!\n");
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

// === CAMERA CONFIG ===

void camera_basic_init() {
    camera_write_reg(0x3008, 0b10000010); // reset
    sleep_ms(10);
    camera_write_reg(0x3008, 0b01000010); // power down
    // bit[7]:  software reset
    // bit[6]:  software power down
    // bit[5:0] debug mode (but bit[1] default = 1)

    camera_write_reg(0x3039, 0b10000000); // PLL bypass

    camera_write_reg(0x302C, 0b11000010);

    camera_write_reg(0x3017, 0b11111111);
    camera_write_reg(0x3018, 0b11111100);

    camera_write_reg(0x4740, 0b00100010);
    /*
    bit[7]: debug
    bit[6]: debug
    bit[5]: PCKL polarity   (0 -> active low; 1 -> active high)
    bit[3]: gate PCLK under VSYNC
    bit[2]: gate PCLK under HREF
    bit[1]: HREF polarity   (0 -> active low; 1 -> active high)
    bit[0]: VSYNC polarity  (0 -> active low; 1 -> active high)
    */

    camera_write_reg(0x501F, 0x09);

    camera_write_reg(0x503D, 0b10000000); // enable color bar

    // SET ALL THE REGISTERS

    camera_write_reg(0x3008, 0b00000010); // normal operations
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

void test_reg_write() {
    printf("Testing register write...\n");
    camera_write_reg(0x4300, 0x60);  // Set RGB565 format
    printf("Format register set to RGB565!\n");
    uint8_t format = camera_read_reg(0x4300);

    if (format == 0x60) {
        printf("Confirmed: format register set to RGB565!\n");
    } else {
        printf("Something went wrong: format register is NOT set to RGB565.\n");
    }
}

uint8_t read_data_pins() {
    uint32_t gpio_state = gpio_get_all();
    return (gpio_state >> CAM_D2) & 0xFF;  // Extract 8 consecutive bits
}

void test_single_capture() {
    printf("Waiting for VSYNC...\n");
    while (!gpio_get(CAM_VSYNC)) { /* wait */ }
    printf("VSYNC detected! Trying to capture data...\n");
    
    uint8_t pixel_data = read_data_pins();
    printf("Captured byte: 0x%02X\n", pixel_data);
}

void test_vsync_pin() {
    for (int i = 0; i < 100; i++) {
        bool vsync = gpio_get(CAM_VSYNC);
        printf("VSYNC reading %d: %d\n", i, vsync);
        sleep_ms(10);
    }
}

void test_capture_single_line() {
    
    uint8_t line_buffer[3000];  // Adjust size as needed
    int pixel_count = 0;

    while (!gpio_get(CAM_VSYNC)) { /* wait for high */ }
    while (gpio_get(CAM_VSYNC)) { /* wait for low - active frame */ }
    while (gpio_get(CAM_HREF)) { /* wait */ }
    
    while (!gpio_get(CAM_HREF) && pixel_count < 3000) {
        while (!gpio_get(CAM_PCLK)) { /* wait for rising edge */ }
        
        line_buffer[pixel_count] = read_data_pins();
        pixel_count++;
        
        // Wait for PCLK to go low before next cycle
        while (gpio_get(CAM_PCLK)) { /* wait for falling edge */ }
    }
    
    printf("Captured %d bytes:\n", pixel_count);
    for (int i = 0; i < pixel_count; i++) {
        printf("0x%02X ", line_buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

// === MAIN ===

int main() {
    stdio_init_all();

    // Give USB time to connect
    sleep_ms(5000);

    camera_set_power(true);
    init_xclk();

    camera_hard_reset();

    init_i2c();

    camera_basic_init();
    init_capture_pins();
    test_capture_single_line();

    printf("All done! Looping forever...\n");
    while (1) {
        sleep_ms(1000);
    }
}