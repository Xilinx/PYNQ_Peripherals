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
#include <grove_joystick.h>

#define DEVICE_MAX 4

struct info {
    analog X;
    analog Y;
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
grove_joystick grove_joystick_open(int grove_id) {
    grove_joystick dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].X = analog_open_grove_a(grove_id);
        info[dev_id].Y = analog_open_grove_b(grove_id);
    }
    return dev_id;
}

void grove_joystick_close(grove_joystick joystick) {
    if (--info[joystick].count != 0) return;
    analog X = info[joystick].X;
    analog Y = info[joystick].Y;
    analog_close(X);
    analog_close(Y);
}

py_float grove_joystick_x(grove_joystick joystick) {
    analog X = info[joystick].X;
    int x_raw = analog_get_raw(X);
    int x_max = analog_get_max(X);
    return (float)(x_max-x_raw)*10/x_raw;
}

py_float grove_joystick_y(grove_joystick joystick) {
    analog Y = info[joystick].Y;
    int y_raw = analog_get_raw(Y);
    int y_max = analog_get_max(Y);
    return (float)(y_max-y_raw)*10/y_raw;
}

py_int grove_joystick_is_clicked(grove_joystick joystick) {
    float X = grove_joystick_x(joystick);
    if (X < 0.1) {
        return 1;
    } else {
        return PY_SUCCESS;
    }
}