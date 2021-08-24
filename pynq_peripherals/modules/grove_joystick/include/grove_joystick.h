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
#include <sys/errno.h>

// Device typedef
/* Joystick class
 *
 * Available Methods:
 *    open, close, x, y, is_clicked
 *    
 */
typedef py_int grove_joystick;

// Device lifetime functions
/* Open a Joystick device connected to the specified port 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G1, PMOD_G2, PMOD_G3, PMOD_G4, ARDUINO_SEEED_D2,
 *     ARDUINO_SEEED_D3,ARDUINO_SEEED_D4, ARDUINO_SEEED_D5,
 *     ARDUINO_SEEED_D6, ARDUINO_SEEED_D7, ARDUINO_SEEED_D8
 * 
 * Returns
 * -------
 * grove_joystick
 *     The device object  
 * 
 */
grove_joystick grove_joystick_open(int grove_id);

/* Release the Joystick by closing the device 
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
void grove_joystick_close(grove_joystick joystick);

// Device functions
/* Returns posotion of the Joystick
 * 
 * Returns the X-Coordinate of the Joystick
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     A float value if successful
 *
 */
py_float grove_joystick_x(grove_joystick joystick);

/* Returns posotion of the Joystick
 * 
 * Returns the Y-Coordinate of the Joystick
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     A float value if successful
 *
 */
py_float grove_joystick_y(grove_joystick joystick);

/* Returns a valid value if the Joystick is clicked
 * 
 * 
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     A 1 value if joystick is clicked
 */
py_int grove_joystick_is_clicked(grove_joystick joystick);
