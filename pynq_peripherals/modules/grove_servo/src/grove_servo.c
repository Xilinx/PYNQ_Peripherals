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
#include <grove_servo.h>

#define DEVICE_MAX 4
#define PERIOD 2000000
#define DUTY_MIN 60000
#define DUTY_MAX 235000
#define ANGLE_MAX 180

struct info {
    timer pin;
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
grove_servo grove_servo_open(int grove_id) {
    grove_servo dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].pin = timer_open_grove(grove_id);
        timer_pwm_generate(info[dev_id].pin, PERIOD, DUTY_MIN);
    }
    return dev_id;
}

void grove_servo_close(grove_servo p) {
    if (--info[p].count != 0) return;
    timer pin = info[p].pin;
    timer_pwm_stop(pin);
    timer_close(pin);
}

py_void grove_servo_set_angular_position(grove_servo p, float angle) {
    if (angle >=0 && angle <= 180) {
        timer pin = info[p].pin;
        int duty_cycle = DUTY_MIN + (angle/ANGLE_MAX) * (DUTY_MAX - DUTY_MIN);
        timer_pwm_generate(pin, PERIOD, duty_cycle);
        return PY_SUCCESS;
    } else {
        return -EPERM;
    }
}
