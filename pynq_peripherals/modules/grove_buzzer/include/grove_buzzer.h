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

/* Buzzer class
 *
 * Available Methods:
 *    open, close, play_tone, play_note, demo
 *    
 */
typedef py_int grove_buzzer;

// Device lifetime functions
/* Open a Buzzer device connected to the specified port 
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
 * grove_buzzer
 *     The device object if successful
 *     -ENOMEM otherwise
 * 
 */
grove_buzzer grove_buzzer_open(int grove_id);

/* Release the Buzzer by closing the device 
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
void grove_buzzer_close(grove_buzzer buzzer);

// Device functions
/* Play a specified tone for a period of time
 * 
 * Parameters
 * ----------
 * tone: int
 *     The frequency of tone in Hz
 * duration: int
 *     Duration in millisecond
 * 
 * Returns
 * -------
 *     None
 *
 */
py_void grove_buzzer_play_tone(grove_buzzer buzzer, int tone, int duration);

/* Play the tone corresponding to the note for a period of time
 * 
 * The parameter note specifies the tone for playing. The available notes
 * are c, d, e, f, g, a, b, C, D, representing the pitch from the middle C
 * to the high D.
 * 
 * Parameters
 * ----------
 * note: const char*
 *     Note of a certain tone
 * duration: int
 *     Duration in millisecond 
 * 
 * Returns
 * -------
 *     None
 *
 */
py_void grove_buzzer_play_note(grove_buzzer buzzer, const char* note, int duration);
