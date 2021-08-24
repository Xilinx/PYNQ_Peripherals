/******************************************************************************
 *  Copyright (c) 2021, Xilinx, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2.  Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *  3.  Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#include <grove_interfaces.h>
#include <grove_led_stick.h>
#include <gpio.h>
#include <xgpio.h>
#include <timer.h>

#define TIMER_ADDR 0x41C00000
#define PMOD_ADDR 0x40000000
#define ARDUINO_ADDR 0x40020000
#define DEVICE_MAX 4
#define LED_MAX 10

extern const unsigned char digital_pins[][2];

struct info {
    gpio pin;
    int pin_id;
    int count;
    unsigned int data[LED_MAX];
    unsigned int addr;
};

static struct info info[DEVICE_MAX];

/*
 * Documentation for public functions is provided as part of the external 
 * public header file.
 * Documentation for static/private functions is provided above each
 * function in this file.
 */ 

/* Get next instance space
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Return
 * ------
 * int
 *     The next index. Return -12 (ENOMEM, no memory) if errors on read.
 * 
 */
static int next_index() {
    for (int i = 0; i < DEVICE_MAX; ++i) {
        if (info[i].count == 0) return i;
    }
    return -ENOMEM;
}

/* Create a zero signal to write to the LED
 * 
 * Parameters
 * ----------
 * gpio_addr: int
 *     Address of the GPIO register being written
 * high: int
 *     set pulse to 1 
 * low: int
 *     set pulse to 0
 * 
 * Returns
 * -------
 *     None
 *
 */
static void write_zero(unsigned int gpio_addr, unsigned int high, unsigned int low) {
    XGpio_WriteReg(gpio_addr, 0, high);
    XGpio_WriteReg(gpio_addr, 0, high);
    XGpio_WriteReg(gpio_addr, 0, high);
    
    XGpio_WriteReg(gpio_addr, 0, low);
    XGpio_WriteReg(gpio_addr, 0, low);
    XGpio_WriteReg(gpio_addr, 0, low);
    XGpio_WriteReg(gpio_addr, 0, low);
    XGpio_WriteReg(gpio_addr, 0, low);
}


/* Create a one signal to write to the LED
 * 
 * Parameters
 * ----------
 * gpio_addr: int
 *     Address of the GPIO register being written
 * high: int
 *     set pulse to 1 
 * low: int
 *     set pulse to 0
 * 
 * Returns
 * -------
 *     None
 *
 */
static void write_one(unsigned int gpio_addr, unsigned int high, unsigned int low) {
    XGpio_WriteReg(gpio_addr, 0, high);
    XGpio_WriteReg(gpio_addr, 0, high);
    XGpio_WriteReg(gpio_addr, 0, high);
    XGpio_WriteReg(gpio_addr, 0, high);
    XGpio_WriteReg(gpio_addr, 0, high);
    
    XGpio_WriteReg(gpio_addr, 0, low);
    XGpio_WriteReg(gpio_addr, 0, low);
    XGpio_WriteReg(gpio_addr, 0, low);
}

grove_led_stick grove_led_stick_open(int grove_id) {
    grove_led_stick dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].pin = gpio_open_grove(grove_id);
        info[dev_id].pin_id = digital_pins[grove_id][0];
        if (grove_id > 6) {
            info[dev_id].addr = ARDUINO_ADDR;
        } else {
            info[dev_id].addr = PMOD_ADDR;
        }
        gpio_set_direction(info[dev_id].pin, GPIO_OUT);
    }
    return dev_id;
}

void grove_led_stick_close(grove_led_stick led_stick) {
    if (--info[led_stick].count != 0) return;
    grove_led_stick_clear(led_stick);
    grove_led_stick_show(led_stick);
    gpio pin = info[led_stick].pin;
    gpio_close(pin);
}

py_void grove_led_stick_show(grove_led_stick led_stick) {
    int pin_id = info[led_stick].pin_id;
    gpio pin = info[led_stick].pin;
    unsigned int gpio_addr = info[led_stick].addr;
    gpio_write(pin, 0);
    unsigned int mask, index, read_value, high_value, low_value;
    read_value = XGpio_ReadReg(gpio_addr, 0);
    mask = ~((0x1 << (pin_id + 1)) - (0x1 << pin_id));
    high_value = (read_value & mask) | (0x1 << pin_id);
    low_value = (read_value & mask) | (0x0 << pin_id);
    delay_us(300);

    for (int i = 0; i < 10; i++) {
        index = 0x00800000;
        for (int j = 0; j < 24; j++) {
            if (info[led_stick].data[i] & index) {
                write_one(gpio_addr, high_value, low_value);
            } else {
                write_zero(gpio_addr, high_value, low_value);
            }
            index = index >> 1;
        }
    }
    return PY_SUCCESS;
}

py_void grove_led_stick_set_pixel(grove_led_stick led_stick, int number, unsigned int RGB) {
    unsigned char R, G, B;
    R = ((RGB & 0xff0000) >> 16);
    G = ((RGB & 0x00ff00) >> 8);
    B = (RGB & 0x0000ff);
    info[led_stick].data[number] = (G << 16) | (R << 8) | B;
    return PY_SUCCESS;
}

py_void grove_led_stick_clear(grove_led_stick led_stick) {
    for (int i = 0; i < 10; i++) {
        info[led_stick].data[i] = 0;
    }
    grove_led_stick_show(led_stick);
    return PY_SUCCESS;
}
