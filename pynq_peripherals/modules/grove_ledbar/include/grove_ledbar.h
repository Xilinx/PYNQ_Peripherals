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

/* ledbar class
 *
 * Available Methods:
 *    open, close, set_level, set_pixel, read, clear
 *    
 */
typedef py_int grove_ledbar;

// Device lifetime functions
/* Open an ledbar device connected to the specified port 
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
 * grove_ledbar
 *     The device object  
 * 
 */
grove_ledbar grove_ledbar_open(int grove_id);

/* Release the ledbar by closing the device 
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
void grove_ledbar_close(grove_ledbar ledbar);


// Device functions
/* Lit the leds from 1 to the specified level in the specified direction with thee sepcified intensity
 *
 * Parameters
 * ----------
 * level: int
 *     The level is the led number it lit to (1 being the left most led, valid value 1 to 10)
 * intensity: int
 *     Can be 1,2 or 3 based on Low, Med or High Intensity
 * inverse: int
 *     Direction left-to-right (Red-to-Green) when 0, right-to-left (green-to-red) when 1
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_ledbar_set_level(grove_ledbar ledbar, int level, int intensity, int inverse);

/* Set a pixel (led) to ON state with desired intensity
 *
 * Parameters
 * ----------
 * index: int
 *     The positon of the Led from 0-9
 * intensity: int
 *     Can be 1, 2 or 3 for Low, Med or High intensity respectively
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_ledbar_set_pixel(grove_ledbar ledbar, int index, int intensity);

/* Read the current state of the ledbar
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 * bits: int
 *     Binary value of the ledbar state, lsb represents index 0 or red led status
 */
py_int grove_ledbar_read(grove_ledbar ledbar);

/* Clear the state of the ledbar 
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
py_void grove_ledbar_clear(grove_ledbar ledbar);
