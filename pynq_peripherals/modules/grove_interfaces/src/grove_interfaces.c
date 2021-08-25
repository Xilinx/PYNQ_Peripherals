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

#include "grove_interfaces.h"
#include "grove_constants.h"
#include <xio_switch.h>

enum GROVE_MAX {
GROVE_GENERAL_MAX = ARDUINO_SEEED_D8,
GROVE_ANALOG_MAX = ARDUINO_SEEED_A3,
GROVE_I2C_MAX = GROVE2,
GROVE_UART_MAX = ARDUINO_SEEED_UART
};

unsigned char digital_pins[][2] = {
    {0, 4}, // PMOD
    {1, 5},
    {7, 3},
    {6, 2},
    {0, 1}, // GROVE
    {2, 3},
    {0, 1}, // ARDUINO DIGILENT UART
    {2, 3}, // ARDUINO DIGILENT G*
    {3, 4}, 
    {4, 5},
    {6, 7},
    {8, 9},
    {10, 11},
    {12, 13},
    {0, 1}, // ARDUINO SEEED UART
    {2, 3}, // ARDUINO SEEED D*
    {3, 4},
    {4, 5},
    {5, 6},
    {6, 7},
    {7, 8},
    {8, 9}
};

unsigned char analog_pins[][2] = {
    {1, 9},
    {6, 15},
    {15, 5},
    {5, 13},
    {1, 9},
    {9, 6},
    {6, 15},
    {15, 5}
};
#ifdef PYNQ_HAS_I2C
i2c i2c_open_grove(int grove_id) {
	if (grove_id == ARDUINO_SEEED_I2C || grove_id == ARDUINO_DIGILENT_I2C) {
		return i2c_open_device(0);
#ifdef XPAR_IO_SWITCH_NUM_INSTANCES
#ifdef XPAR_IO_SWITCH_0_I2C0_BASEADDR
	} else if (grove_id <= GROVE_I2C_MAX) {
		return i2c_open(digital_pins[grove_id][1],
				digital_pins[grove_id][0]);
#endif
#endif
	} else {
		return -1;
	}
}
#endif

#ifdef PYNQ_HAS_GPIO
static gpio gpio_open_grove_internal(int grove_id, int pin_id) {
	return gpio_open(digital_pins[grove_id][pin_id]);
};

gpio gpio_open_grove(int grove_id) {
	return gpio_open_grove_internal(grove_id, 0);
}
gpio gpio_open_grove_a(int grove_id) {
	return gpio_open_grove_internal(grove_id, 0);
}
gpio gpio_open_grove_b(int grove_id) {
	return gpio_open_grove_internal(grove_id, 1);
}
#endif

#ifdef PYNQ_HAS_UART
uart uart_open_grove(int grove_id) {
	if (grove_id <= GROVE_UART_MAX) {
		return uart_open(digital_pins[grove_id][1],
				 digital_pins[grove_id][0]);
	} else {
		return -1;
	}
}
#endif

static analog analog_open_grove_internal(int grove_id, int pin) {
	if (grove_id >= ARDUINO_DIGILENT_A1 && grove_id <= ARDUINO_SEEED_A3) {
		return analog_open_xadc(analog_pins[grove_id - ARDUINO_DIGILENT_A1][pin]);
	} else {
		return -1;
	}
}

analog analog_open_grove(int grove_id) {
	return analog_open_grove_internal(grove_id, 0);
}
analog analog_open_grove_a(int grove_id) {
	return analog_open_grove_internal(grove_id, 0);
}
analog analog_open_grove_b(int grove_id) {
	return analog_open_grove_internal(grove_id, 1);
}

static timer timer_open_grove_internal(int grove_id, int pin_id) {
   timer device = timer_open_device(0);
   init_io_switch();
   set_pin(digital_pins[grove_id][pin_id], PWM0);
	return device;
};

timer timer_open_grove(int grove_id) {
	return timer_open_grove_internal(grove_id, 0);
}
timer timer_open_grove_a(int grove_id) {
	return timer_open_grove_internal(grove_id, 0);
}
timer timer_open_grove_b(int grove_id) {
	return timer_open_grove_internal(grove_id, 1);
}
