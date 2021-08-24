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

/* lgcp (light-color-temperature) class
 *
 * Available Methods:
 *    open, close, clear_display, reset, select_proximity, select_als,
 *    select_gesture, set_adc_integration_time, clear_proximity_interrupts, 
 *    set_proximity_range, 
 *    get_proximity_raw, proximity_interrupt,get_rgbc_raw, red, green, blue,
 *    get_clear, get_lux, get_cct, get_gesture_north, get_gesture_south,
 *    get_gesture_east, get_gesture_west
 *    
 */
typedef py_int grove_lgcp;

// Device lifetime functions

/* Open a light-color-temperature (lgcp) module connected to the specified port with the default 
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
 * grove_lgcp
 *     The device object if successful
 *     -EIO device not present (raises exception)
 * 
 */
grove_lgcp grove_lgcp_open(int grove_id);

/* Release the lgcp by closing the device 
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
void grove_lgcp_close(grove_lgcp p);

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
py_int grove_lgcp_get_enable_reg(grove_lgcp p);

/* Select proximity feature
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
py_void grove_lgcp_select_proximity(grove_lgcp p);

/* Select ALS (light) feature
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
py_void grove_lgcp_select_als(grove_lgcp p);

/* Enable gesture feature
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
py_void grove_lgcp_select_gesture(grove_lgcp p);

/* Set on-chip ADC integration time
 *
 * Parameters
 * ----------
 * atime: unsigned char
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_lgcp_set_adc_integration_time(grove_lgcp p, unsigned char atime);

/* Clear source of interrupt for the proximity feature
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
py_void grove_lgcp_clear_proximity_interrupts(grove_lgcp p);

//proximity

/* Configure the proximity feature with the recommended settings
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
// py_void grove_lgcp_setup_recommended_config_for_proximity(grove_lgcp p);

/* Set low (starting) and high (ending) values for the proxiity detection
 *
 * Parameters
 * ----------
 *     low, high: unsigned char
 *
 * Returns
 * -------
 *     None
 *
 */
py_void grove_lgcp_set_proximity_range(grove_lgcp p, unsigned char low, unsigned char high);

/* Get the raw proximity value 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			Raw value
 *			-EIO general operation error (raises exception)
 *
 */
py_int grove_lgcp_get_proximity_raw(grove_lgcp p);

/* Set low (starting) and high (ending) values for the proximity detection
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			1 if object detected
 *			0 otherwise
 *
 */
py_int grove_lgcp_is_proximity_detected(grove_lgcp p);

//ALS(light)

/* Get the raw light intensity values 
 *
 * The raw values are stored in the internal data structure
 * Typically, this will follow with readig R, G, B, Clear values calls
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			-EIO general operation error (raises exception)
 *
 */
py_int grove_lgcp_get_rgbc_raw(grove_lgcp p);

/* Get the raw red light intensity value 
 *
 * Preceed this call with grove_lgcp_get_rgbc_raw
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			Red color raw intensity
 *
 */
py_int grove_lgcp_red(grove_lgcp p);

/* Get the raw green light intensity value 
 *
 * Preceed this call with grove_lgcp_get_rgbc_raw
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			Green color raw intensity
 *
 */
py_int grove_lgcp_green(grove_lgcp p);

/* Get the raw blue light intensity value 
 *
 * Preceed this call with grove_lgcp_get_rgbc_raw
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			Blue color raw intensity
 *
 */
py_int grove_lgcp_blue(grove_lgcp p);

/* Get the raw clear light intensity value 
 *
 * Preceed this call with grove_lgcp_get_rgbc_raw
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			Clear color raw intensity
 *
 */
py_int grove_lgcp_clear(grove_lgcp p);

/* Get the Illuminance value 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			Illuminance(Lux) value
 *
 */
py_int grove_lgcp_get_lux(grove_lgcp p);

/* Get the color corelated temperature (cct) value 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			CCT value
 *
 */
py_int grove_lgcp_get_cct(grove_lgcp p);

//Gesture

/* Get the movement in the North direction 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			North direction movement value
 *			-EIO general operation error (raises exception)
 *
 */
py_int grove_lgcp_get_gesture_north(grove_lgcp p);

/* Get the movement in the South direction 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			South direction movement value
 *			-EIO general operation error (raises exception)
 *
 */
py_int grove_lgcp_get_gesture_south(grove_lgcp p);

/* Get the movement in the West direction 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			West direction movement value
 *			-EIO general operation error (raises exception)
 *
 */
py_int grove_lgcp_get_gesture_west(grove_lgcp p);

/* Get the movement in the East direction 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     int:
 *			East direction movement value
 *			-EIO general operation error (raises exception)
 *
 */
py_int grove_lgcp_get_gesture_east(grove_lgcp p);
