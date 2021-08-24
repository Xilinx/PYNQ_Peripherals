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
#include <grove_usranger.h>
#include "xparameters.h"
#include "xtmrctr.h"
#include "gpio.h"
#include "timer.h"

#define DEVICE_MAX 4
#define MAX_COUNT 0xFFFFFFFF
#define TIMEOUT 1e8

struct info {
    gpio pin;
    int count;
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

grove_usranger grove_usranger_open(int grove_id) {
    grove_usranger dev_id = next_index();
    if (dev_id >= 0 ) {
        info[dev_id].count++;
        info[dev_id].pin = gpio_open_grove(grove_id);
    }
    return dev_id;
}

void grove_usranger_close(grove_usranger p) {
    if (--info[p].count != 0) return;
    gpio pin = info[p].pin;
    gpio_close(pin);
}

/* Generate a 10 us pulse
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Return
 * ------
 *     None
 * 
 */
static void create_10us_pulse(gpio pin){
    gpio_set_direction(pin, GPIO_OUT);
    gpio_write(pin, 0);
    delay_us(2);
    gpio_write(pin, 1);
    delay_us(10);
    gpio_write(pin, 0);
}

/* Measure a pulse width
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Return
 * ------
 * pulse width: unsigned int
 * 
 */
static unsigned int capture_duration(gpio pin){
    unsigned int count0, count1, count2;
    count1 = 0;
    count2 = 0;
    XTmrCtr_WriteReg(XPAR_TMRCTR_0_BASEADDR, 0, TLR0, 0x0);
    XTmrCtr_WriteReg(XPAR_TMRCTR_0_BASEADDR, 0, TCSR0, 0x190);
    count0 = XTmrCtr_ReadReg(XPAR_TMRCTR_0_BASEADDR, 0, TCR0);
    count1=XTmrCtr_ReadReg(XPAR_TMRCTR_0_BASEADDR, 0, TCR0);
    while((!gpio_read(pin)) && ((count1 - count0) < TIMEOUT)) {
        count1=XTmrCtr_ReadReg(XPAR_TMRCTR_0_BASEADDR, 0, TCR0);
    }
    count2=XTmrCtr_ReadReg(XPAR_TMRCTR_0_BASEADDR, 0, TCR0);
    while(gpio_read(pin) && ((count2 - count1) < TIMEOUT)) {
        count2=XTmrCtr_ReadReg(XPAR_TMRCTR_0_BASEADDR, 0, TCR0);
    }
    
    if(count2 > count1) {
        return (count2 - count1);
    } else {
        return((MAX_COUNT - count1) + count2);  
    }
}

py_float grove_usranger_get_distance(grove_usranger usranger) {
    gpio pin = info[usranger].pin;
    unsigned int raw_value;
    unsigned int num_nanoseconds;
    float distance_read;
    float max_distance = 5; // in meters
    int clk_period_ns = 10;
    float sound_speed = 340.29;

    create_10us_pulse(pin);
    gpio_set_direction(pin, GPIO_IN);
    raw_value = capture_duration(pin);
    num_nanoseconds = raw_value * clk_period_ns;
    distance_read = num_nanoseconds * 1e-9 * sound_speed / 2;
    if (distance_read > max_distance) {
        return max_distance * 100;
    }
    else {
        return distance_read * 100;
    }
}