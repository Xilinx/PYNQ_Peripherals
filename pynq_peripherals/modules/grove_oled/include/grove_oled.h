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

/* OLED class
 *
 * Available Methods:
 *    open, close, clear_display, set_normal_display, set_inverse_display,
 *    set_gray_level, set_same_row_wrap_mode, set_set_next_row_wrap_mode, 
 *    set_position, set_contrast_level, put_char, put_string, 
 *    activate_scroll, deactivate_scroll
 *    
 */
typedef py_int grove_oled;

// Device lifetime functions
/* Open an OLED module connected to the specified port with the default 
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
 * grove_oled
 *     The device object if successful
 *     -EIO device not present (raises exception)
 * 
 */
grove_oled grove_oled_open(int grove_id);

/* Release the OLED by closing the device 
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
void grove_oled_close(grove_oled oled);

// Device functions
/* Initialize the OLED with default configurations
 *
 * Unlock the display's MCU, set display off, switch ON
 * the display, set normal display mode, initialize pixels
 * to be brightest white
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
py_void grove_oled_set_default_config(grove_oled oled);

/* Set normal display
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
py_void grove_oled_set_normal_display(grove_oled oled);

/* Set inverse display
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
py_void grove_oled_set_inverse_display(grove_oled oled);

/* Set gray-level
 *
 * Parameters
 * ----------
 * gray level: unsigned char
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_oled_set_gray_level(grove_oled oled, unsigned char gray_level);

/* Set Page mode
 * In this mode, if the string is long (> 16 characters), it will overwrite the string
 * In other words, the line wraps around in the same line
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
py_void grove_oled_set_same_row_wrap_mode(grove_oled oled);

/* Set Horizontal mode
 * In this mode, if the string is long (> 16 characters), it will continue on the next line
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
py_void grove_oled_set_next_row_wrap_mode(grove_oled oled);

/* Set row,column position
 * The valid row range is 0 to 7. The valid column range is 0 to 15
 *
 * Parameters
 * ----------
 * row, column: int
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_oled_set_position(grove_oled oled, int row, int column);

/* Clear the display
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
py_void grove_oled_clear_display(grove_oled oled);

/* Set contrast level
 *
 * Parameters
 * ----------
 * level
 *    Contrast level from 0 to 255
 *
 */
py_void grove_oled_set_contrast_level(grove_oled oled, unsigned char contrast_level);

/* Output an ASCII character
 *
 * Parameters
 * ----------
 * Character: unsigned char
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_oled_put_char(grove_oled oled, unsigned char character);

/* Output an ASCII string
 *
 * Parameters
 * ----------
 * ASCII string: char pointer to the string
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_oled_put_string(grove_oled oled, const char *s);

/* Activate scrolling
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
py_void grove_oled_activate_scroll(grove_oled oled);

/* Deactivate scrolling
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
py_void grove_oled_deactivate_scroll(grove_oled oled);
