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
#include <grove_ledbar.h>
#include <circular_buffer.h>
#include <gpio.h>
#include <timer.h>

#define DEVICE_MAX 4
#define GLB_CMDMODE                 0x00
#define HIGH                        0xFF
#define LOW                         0x01
#define MED                         0xAA
#define OFF                         0x00


struct info {
    gpio data;
    gpio clk;
    int count;
    char ledbar_state[10] = {OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF};
    char current_state[10] = {OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF};
};

static struct info info[DEVICE_MAX];

// Current Level
static int level_holder = 0;

// Current direction: 0 => Red-to-Green, 1 => Green-to-Red
static int prev_inverse = 0;

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
grove_ledbar grove_ledbar_open(int grove_id) {
    grove_ledbar dev_id = next_index();
    info[dev_id].count++;
    info[dev_id].data = gpio_open_grove_a(grove_id);
    info[dev_id].clk = gpio_open_grove_b(grove_id);
    
    gpio_set_direction(info[dev_id].data, GPIO_OUT);
    gpio_set_direction(info[dev_id].clk, GPIO_OUT);
    grove_ledbar_set_level(dev_id, 0, 0, 0);
    return dev_id;
}

void grove_ledbar_close(grove_ledbar ledbar) {
    grove_ledbar_clear(ledbar);
    if (--info[ledbar].count != 0) return;
    gpio data = info[ledbar].data;
    gpio clk = info[ledbar].clk;
    gpio_close(data);
    gpio_close(clk);
}

/* Send 8-bit data to the LED Bar
 * 
 * Parameters
 * ----------
 * send_data: u8
 *     8-bit data shifted into the LED Bar
 * 
 * Returns
 * -------
 *     None
 *
 */
static void send_data(grove_ledbar ledbar, u8 send_data){
    gpio clk = info[ledbar].clk;
    gpio data = info[ledbar].data;
    int i;
    u32 data_state, clkval, data_internal;

    data_internal = send_data;

    clkval = 0;
    gpio_write(data, 0);
    // First toggle the clock 8 times
    for (i = 0; i < 8; ++i) {
         clkval ^= 1;
         gpio_write(clk, clkval);
    }

    // Working in 8-bit mode
    for (i = 0; i < 8; i++){
        /*
         * Read each bit of the data to be sent LSB first
         * Write it to the data_pin
         */
        data_state = (data_internal & 0x80) ? 0x00000001 : 0x00000000;
        gpio_write(data, data_state);
        clkval ^= 1;
        gpio_write(clk, clkval);

        // Shift Incoming data to fetch next bit
        data_internal = data_internal << 1;
    }
}

/* Send 4 pulses to latch data sent to the LED Bar
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     None
 *
 */
static void latch_data(grove_ledbar ledbar){
    gpio data = info[ledbar].data;
    gpio clk = info[ledbar].clk;
    int i;
    gpio_write(data, 0);
    delay_ms(10);

    // Generate four pulses on the data pin as per data sheet
    for (i = 0; i < 4; i++){
        gpio_write(data, 1);
        gpio_write(data, 0);
    }
}

/* Function to reverse incoming data 
 * 
 * Parameters
 * ----------
 * c: u16
 *     10-bit data + 6 padding bits shifted into the LED Bar
 * 
 * Returns
 * -------
 * result
 *      reversed data
 */
static u16 reverse_data(grove_ledbar ledbar, u16 c){
    gpio data = info[ledbar].data;
    gpio clk = info[ledbar].clk;
    /*
     * Function to reverse incoming data  
     * Allows LEDbar to be lit in reverse order
     */
    int shift0;
    u16 result = 0;

    for (shift0 = 0; shift0 < 16; shift0++){
        if (c & (0x0001 << shift0))
            result |= (0x8000 >> shift0);
    }

    // 10 LSBs are used as LED Control 6 MSBs are ignored
    result = result >> 6;
    return result;
}

/* Set the intensity of each LED in the LED Bar
 * 
 * Parameters
 * ----------
 * intensity: int
 *     set to low, med, high or off
 * h: int
 *     position of the led in the LED Bar
 *
 * Returns
 * -------
 *     None
 *
 */
static int set_intensity(grove_ledbar ledbar, int intensity, int h){
    if (intensity == 1) {
        info[ledbar].ledbar_state[h] = LOW;
    } else if (intensity == 2) {
        info[ledbar].ledbar_state[h] = MED;
    } else if (intensity == 3) {
        info[ledbar].ledbar_state[h] = HIGH;
    } else {
        info[ledbar].ledbar_state[h] = OFF;
    }
    return 0;
}

/* Update pixels on the LED Bar based on led bar state array
 * 
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     None
 *
 */
static int update_pixel(grove_ledbar ledbar) {
    send_data(ledbar, GLB_CMDMODE);

    for (int i = 0; i < 10; i++){
        send_data(ledbar, info[ledbar].ledbar_state[i]);
    }
    // Two extra empty bits for padding the command to the correct length
    send_data(ledbar, 0x00);
    send_data(ledbar, 0x00);
    latch_data(ledbar);
    // Store LEBbar state for reading purpose.
    for(int h=0; h<10; h++){
        info[ledbar].current_state[h] = info[ledbar].ledbar_state[h];
    }
    return 0;
}

py_void grove_ledbar_set_pixel(grove_ledbar ledbar, int index, int intensity) {
    if (index < 10 && index >= 0) {
        set_intensity(ledbar, intensity, index);
    } else {
        return -EIO;
    }
    update_pixel(ledbar);
    return PY_SUCCESS;
}

py_void grove_ledbar_set_level(grove_ledbar ledbar, int level, int intensity, int inverse){
    int h,i;
    int prev_inv ;

    prev_inv = prev_inverse;

    // Clear LED states from previous writes
    if (inverse != prev_inv) {
        for(h=0; h<10; h++){
            info[ledbar].ledbar_state[h] = OFF;
        }
    }

    if (inverse == 0) { 
        // Execute when direction is Red-to-Green
        if (level < level_holder) {
            for(h=level_holder-1; h>level-1; h--){
                info[ledbar].ledbar_state[h] = OFF;
            }
        }
        for(h=0; h<level; h++)
        {
            set_intensity(ledbar, intensity, h);
        }
        for(h=level; h>10; h++){
            info[ledbar].ledbar_state[h] = OFF;
        }
    } else if(inverse == 1) { // Execute when direction is Red-to-Green
        if (level < level_holder) {
            for(h=0; h>=10-level; h++)
            {
                info[ledbar].ledbar_state[h] = OFF;
            }
        }
        for(h=9; h>=10-level; h--)
        {
            set_intensity(ledbar, intensity, h);
        }
        if (level != 10) {
            for(h=10-level-1; h>=0; h--)
            {
                info[ledbar].ledbar_state[h] = OFF;
            }
        }
    } else { // Execute when direction is Invalid Integer
        for(h=0; h<10; h++){
            info[ledbar].ledbar_state[h] = OFF;
        }
    }
    // Store LEBbar Indication level for resetting level
    level_holder= level;
    // Store LEBbar direction for resetting direction
    prev_inverse = inverse;

    update_pixel(ledbar);
    return PY_SUCCESS;
}

py_int grove_ledbar_read(grove_ledbar ledbar){
    unsigned int bits;
    bits = 0x0000;
    for(int h=0; h<10; h++){
        if (info[ledbar].current_state[h] != OFF) {
            bits |= 0x0001 << h;
        }
    }
    bits = bits & 0x03FF;
    return bits;
}

py_void grove_ledbar_clear(grove_ledbar ledbar) {
    for(int h=0; h<10; h++){
        info[ledbar].ledbar_state[h] = OFF;
    }
    update_pixel(ledbar);
    return PY_SUCCESS;
}