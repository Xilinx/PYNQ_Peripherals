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
#include <grove_pir.h>

#define DEVICE_MAX 4

struct info {
    gpio pin;
    int count;
};

static struct info info[DEVICE_MAX];

static int next_index() {
    for (int i = 0; i < DEVICE_MAX; ++i) {
        if (info[i].count == 0) return i;
    }
    return -ENOMEM;
}
grove_pir grove_pir_open(int grove_id) {
    grove_pir dev_id = next_index();
    info[dev_id].count++;
    info[dev_id].pin = gpio_open_grove(grove_id);
    gpio_set_direction(info[dev_id].pin, GPIO_IN);
    return dev_id;
}

void grove_pir_close(grove_pir pir) {
    if (--info[pir].count != 0) return;
    gpio pin = info[pir].pin;
    gpio_close(pin);
}

py_bool grove_pir_motion_detected(grove_pir pir) {
    gpio pin = info[pir].pin;
    return gpio_read(pin);
}

