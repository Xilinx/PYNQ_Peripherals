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
#include <grove_ph.h>
#include <grove_adc.h>
#include "grove_constants.h"
#include <pyprintf.h>

#define DEVICE_MAX 4

struct info {
    analog pin;
    int count;
};

static struct info info[DEVICE_MAX];

float vol_1 = 1.66;
float ph_1 = 9.18;
float vol_2 = 1.93;
float ph_2 = 4.00;
float k = -19.18518519;
float offset = 41.02740741;

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

static grove_ph grove_ph_open_analog(analog pin) {
    grove_ph dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].pin = pin;
    }
    return dev_id;
}

grove_ph grove_ph_open(int grove_id) {
    if (grove_id >= ARDUINO_DIGILENT_A1 && grove_id <= ARDUINO_SEEED_A3)
        return grove_ph_open_analog(analog_open_grove(grove_id));
    else 
        return grove_ph_open_analog(analog_open_grove_adc(grove_adc_open(grove_id)));
}

grove_ph grove_ph_open_at_address(int grove_id, int address) {
    return grove_ph_open_analog(analog_open_grove_adc(grove_adc_open_at_address(grove_id, address)));
}

void grove_ph_close(grove_ph ph) {
    if (--info[ph].count != 0) return;
    analog pin = info[ph].pin;
    analog_close(pin);
}

py_float grove_ph_first_calibrate(grove_ph p, float ph) {
    if (ph < 7 || ph > 14) return -1;
    ph_1 = ph;
    analog pin = info[p].pin;
    vol_1 = (float)analog_get_voltage(pin);
    return vol_1;
}

py_float grove_ph_second_calibrate(grove_ph p, float ph) {
    if (ph < 0 || ph > 7) return -1;
    ph_2 = ph;
    analog pin = info[p].pin;
    vol_2 = (float)analog_get_voltage(pin);
    return vol_2;
}

py_float grove_ph_get_ph(grove_ph ph) {
    analog pin = info[ph].pin;
    float current_voltage = (float)analog_get_voltage(pin);
    k = (ph_2 - ph_1) / (vol_2 - vol_1);
    offset = ((ph_2 + ph_1) - k*(vol_1 + vol_2))*0.5;
    float raw_value = k*current_voltage + offset;
    raw_value = (raw_value > 14) ? 14 : raw_value;
    raw_value = (raw_value < 0 ) ? 0 : raw_value;
    return raw_value;
}
