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

#pragma once
#include <grove_constants.h>
#include <pytypes.h>

enum GESTURE_MAP {
    NONE,
    FORWORD,
    BACKWORD,
    RIGHT,
    LEFT,
    UP,
    DOWN,
    CLK_WISE,
    AN_CLK_WISE,
    WAVE
};

typedef py_int grove_gesture;

// Device lifetime functions

/* Open a Gesture module connected to the specified port with the default 
 * I2C address
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G3, PMOD_G4, ARDUINO_SEEED_I2C 
 * 
 * Returns
 * -------
 * grove_gesture
 *     The device object if successful
 *     -EIO device not present (raises exception)
 * 
 */
grove_gesture grove_gesture_open(int grove_id);

/* Open a Gesture module connected to the specified port and a non-default
 * I2C address
 *
 * Unique I2C address is needed when two modules of same type are connected
 * to the same I2C channel. The two modules should have unique I2C
 * address
 *
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G3, PMOD_G4, ARDUINO_SEEED_I2C 
 * address: int
 *     IC2 address of the device
 * 
 * Returns
 * -------
 * grove_adc
 *     The device object if successful
 *     -EIO device not present (raises exception)
 *
 */
grove_gesture grove_gesture_open_at_address(int grove_id, int address);

/* Release the Gesture by closing the device 
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
void grove_gesture_close(grove_gesture gesture);


// Device functions

/* Read the Gesture sensor and return gesture code
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *   int:
 *      NONE (0)
 *      FORWORD (1)
 *      BACKWORD (2)
 *      RIGHT (3)
 *      LEFT (4)
 *      UP (5)
 *      DOWN (6)
 *      CLK_WISE (7)
 *      AN_CLK_WISE (8)
 *      WAVE (9)
 *
 */
py_int grove_gesture_gesture(grove_gesture gesture);
