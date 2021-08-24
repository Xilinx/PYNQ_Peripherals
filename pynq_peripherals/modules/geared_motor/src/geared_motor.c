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
#include <geared_motor.h>
#include <gpio.h>

#define DEVICE_MAX 4
#define PERIOD 2000000
#define DUTY_MAX 1999999
#define DUTY_MIN 20000

struct info {
    timer motor_pin;
    gpio dir_pin;
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
geared_motor geared_motor_open(int grove_id) {
    geared_motor dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].motor_pin = timer_open_grove_b(grove_id);
        info[dev_id].dir_pin = gpio_open_grove_a(grove_id);
        timer_pwm_generate(info[dev_id].motor_pin, PERIOD, 0);
        gpio_set_direction(info[dev_id].dir_pin, GPIO_OUT);
        gpio_write(info[dev_id].dir_pin, 1);
    }
    return dev_id;
}

void geared_motor_close(geared_motor p) {
    if (--info[p].count != 0) return;
    timer motor_pin = info[p].motor_pin;
    gpio dir_pin = info[p].dir_pin;
    timer_pwm_stop(motor_pin);
    timer_close(motor_pin);
    gpio_close(dir_pin);
}

py_void geared_motor_set_speed(geared_motor p, int value) {
    timer pin = info[p].motor_pin;
    if (value >= 0 && value <= 100) {
        int duty_cycle = DUTY_MAX * value / 100;
        timer_pwm_generate(pin, PERIOD, duty_cycle);
        return PY_SUCCESS;
    } else {
        return -EPERM;
    }
}

void geared_motor_forward(geared_motor p) {
    gpio dir_pin = info[p].dir_pin;
    gpio_write(dir_pin, 1);
}

void geared_motor_backward(geared_motor p) {
    gpio dir_pin = info[p].dir_pin;
    gpio_write(dir_pin, 0);
}
