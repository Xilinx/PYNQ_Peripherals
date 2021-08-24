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
#include "grove_constants.h"
#include <grove_light.h>
#include <grove_adc.h>

#define DEVICE_MAX 4

struct info {
    analog pin;
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

static grove_light grove_light_open_analog(analog pin) {
    grove_light dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].pin = pin;
    }
    return dev_id;
}

grove_light grove_light_open(int grove_id) {
    if (grove_id >= ARDUINO_DIGILENT_A1 && grove_id <= ARDUINO_SEEED_A3)
        return grove_light_open_analog(analog_open_grove(grove_id));
    else 
        return grove_light_open_analog(analog_open_grove_adc(grove_adc_open(grove_id)));
}

grove_light grove_light_open_at_address(int grove_id, int address) {
    return grove_light_open_analog(analog_open_grove_adc(grove_adc_open_at_address(grove_id, address)));
}

void grove_light_close(grove_light light) {
    if (--info[light].count != 0) return;
    analog pin = info[light].pin;
    analog_close(pin);
}

py_float grove_light_get_intensity(grove_light light) {
    analog pin = info[light].pin;
    return (float)analog_get_voltage(pin) * 100.0 / (float)analog_get_reference(pin);
}
