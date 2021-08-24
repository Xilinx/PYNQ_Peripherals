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
#include <grove_oled.h>
#include <xil_types.h>
#include <cstring>
#include <stdio.h>
#include <grove_oled_hw.h>

#define I2C_ADDRESS 0x3c
#define DEVICE_MAX 4

struct info {
    i2c i2c_dev;
    int count;
};

static struct info info[DEVICE_MAX];
static int oleds_used, num_oleds;
static unsigned char grayH;
static unsigned char grayL;

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


/* Write a command
 * 
 * Parameters
 * ----------
 *
 * command: u8
 *     Command byte to be written.
 * 
 * Return
 * ------
 * int
 *     Return 0 if successful, -5 (EIO) if errors on write.
 * 
 */
static int send_command(grove_oled oled, u8 command){
    i2c i2c_dev = info[oled].i2c_dev;
    u8 buffer[2];
    buffer[0] = OLED_Command_Mode;
    buffer[1] = command;
    if (i2c_write(i2c_dev, OLED_Address, buffer, 2) != 2) return -EIO;
    return 0;
}

/* Write a byte of data
 * 
 * Parameters
 * ----------
 *
 * address: u8
 *     Data byte to be written.
 * 
 * Return
 * ------
 * int
 *     Return 0 if successful, -5 (EIO) if errors on write.
 * 
 */
static int send_data(grove_oled oled, u8 data){
    i2c i2c_dev = info[oled].i2c_dev;
    u8 buffer[2];
    buffer[0] = OLED_Data_Mode;
    buffer[1] = data;
    if (i2c_write(i2c_dev, OLED_Address, buffer, 2) != 2) return -EIO;
    return 0;
}

grove_oled grove_oled_open(int grove_id) {
    grove_oled dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].i2c_dev = i2c_open_grove(grove_id);
        if (grove_oled_set_default_config(dev_id) == -EIO) {
            info[dev_id].count--;
            i2c_close(info[dev_id].i2c_dev);
            return -EIO;
        }
    }
	return dev_id;
}

void grove_oled_close(grove_oled oled) {
    if (--info[oled].count != 0) return;
    i2c i2c_dev = info[oled].i2c_dev;
    i2c_close(i2c_dev);
}

py_void grove_oled_set_contrast_level(grove_oled oled, unsigned char level){
    unsigned char ContrastLevel;
    ContrastLevel = level;
    if (send_command(oled, OLED_Set_ContrastLevel_Cmd) == -EIO) return -EIO;
    if (send_command(oled, ContrastLevel) == -EIO) return -EIO;
    return PY_SUCCESS;
}

py_void grove_oled_set_next_row_wrap_mode(grove_oled oled){
    char addressingMode = HORIZONTAL_MODE;
    if (send_command(oled, 0x20) == -EIO) return -EIO;             //set addressing mode
    if (send_command(oled, 0x00) == -EIO) return -EIO;             //set horizontal addressing mode
    return PY_SUCCESS;
}

py_void grove_oled_set_same_row_wrap_mode(grove_oled oled){
    char addressingMode = PAGE_MODE;
    if (send_command(oled, 0x20) == -EIO) return -EIO;             //set addressing mode
    if (send_command(oled, 0x02) == -EIO) return -EIO;             //set page addressing mode
    return PY_SUCCESS;
}

py_void grove_oled_set_position(grove_oled oled, int Row, int Column){
    if (send_command(oled, 0xB0 + Row) == -EIO) return -EIO;                   //set page address
    if (send_command(oled, 0x00 + (8*Column & 0x0F)) == -EIO) return -EIO;     //set column lower address
    if (send_command(oled, 0x10 + ((8*Column>>4)&0x0F)) == -EIO) return -EIO;  //set column higher address
    return PY_SUCCESS;
}

py_void grove_oled_clear_display(grove_oled oled){
    int i,j;
    if (send_command(oled, OLED_Display_Off_Cmd) == -EIO) return -EIO;     //display off
    for(j=0;j<8;j++)
    {
        grove_oled_set_position(oled, j,0);
        {
            for(i=0;i<16;i++)             //clear all columns
            {
                grove_oled_put_char(oled,' ');
            }
        }
    }
    if (send_command(oled, OLED_Display_On_Cmd) == -EIO) return -EIO;     //display on
    if (grove_oled_set_position(oled, 0,0) == -EIO) return -EIO;
    return PY_SUCCESS;
}

py_void grove_oled_set_gray_level(grove_oled oled, unsigned char level){
    unsigned char grayLevel;
    grayLevel = level;
    grayH = (grayLevel << 4) & 0xF0;
    grayL =  grayLevel & 0x0F;
    return PY_SUCCESS;
}

py_void grove_oled_put_char(grove_oled oled, unsigned char C){
    if(C < 32 || C > 127) //Ignore non-printable ASCII characters.
    {
        C=' '; //Space
    }
    
    int i;
    for(i=0;i<8;i++){
        if (send_data(oled, BasicFont[C-32][i]) == -EIO) return -EIO;
    }
    return PY_SUCCESS;
}

py_void grove_oled_put_string(grove_oled oled, const char *s){
    char ch;

    while(*s !=0) {
        ch = *s++;
        if (grove_oled_put_char(oled, ch) == -EIO) return -EIO;
    }
    return PY_SUCCESS;
}

py_void grove_oled_set_default_config(grove_oled oled){
    // Unlock OLED driver IC MCU interface
    if (send_command(oled, 0xFD) == -EIO) return -EIO;
    if (send_command(oled, 0x12) == -EIO) return -EIO;
    //Set display off
    if (send_command(oled, 0xAE) == -EIO) return -EIO;
    // Switch on display
    if (send_command(oled, 0xAF) == -EIO) return -EIO;
    // Set normal display
    if (send_command(oled, 0xA4) == -EIO) return -EIO;
    // Init gray level for text. Default:Brightest White
    grayH= 0xF0;
    grayL= 0x0F;
    if (grove_oled_clear_display(oled) == -EIO) return -EIO;
    return PY_SUCCESS;
}

py_void grove_oled_activate_scroll(grove_oled oled){
    if (send_command(oled, OLED_Activate_Scroll_Cmd) == -EIO) return -EIO;
    return PY_SUCCESS;
}

py_void grove_oled_deactivate_scroll(grove_oled oled){
    if (send_command(oled, OLED_Dectivate_Scroll_Cmd) == -EIO) return -EIO;
    return PY_SUCCESS;
}

py_void grove_oled_set_normal_display(grove_oled oled){
    if (send_command(oled, OLED_Normal_Display_Cmd) == -EIO) return -EIO;
    return PY_SUCCESS;
}

py_void grove_oled_set_inverse_display(grove_oled oled){
    if (send_command(oled, OLED_Inverse_Display_Cmd) == -EIO) return -EIO;
    return PY_SUCCESS;
}
