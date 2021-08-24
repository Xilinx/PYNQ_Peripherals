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
#include <grove_gesture.h>
#include <grove_gesture_hw.h>
#include <xparameters.h>
#include <xtmrctr.h>
#include <timer.h>
#include <i2c.h>

#define DEVICE_MAX 4

#define I2C_ADDRESS 0x73


#define INIT_REG_ARRAY_SIZE \
    (sizeof(initRegisterArray)/sizeof(initRegisterArray[0]))

typedef enum bank{
	BANK0 = 0,
	BANK1
}bank_e;

const unsigned char initRegisterArray[][2] = {	// Initial Gesture
    {0xEF,0x00},{0x32,0x29},{0x33,0x01},{0x34,0x00},{0x35,0x01},{0x36,0x00},
	{0x37,0x07},{0x38,0x17},{0x39,0x06},{0x3A,0x12},{0x3F,0x00},{0x40,0x02},
	{0x41,0xFF},{0x42,0x01},{0x46,0x2D},{0x47,0x0F},{0x48,0x3C},{0x49,0x00},
    {0x4A,0x1E},{0x4B,0x00},{0x4C,0x20},{0x4D,0x00},{0x4E,0x1A},{0x4F,0x14},
	{0x50,0x00},{0x51,0x10},{0x52,0x00},{0x5C,0x02},{0x5D,0x00},{0x5E,0x10},
	{0x5F,0x3F},{0x60,0x27},{0x61,0x28},{0x62,0x00},{0x63,0x03},{0x64,0xF7},
	{0x65,0x03},{0x66,0xD9},{0x67,0x03},{0x68,0x01},{0x69,0xC8},{0x6A,0x40},
    {0x6D,0x04},{0x6E,0x00},{0x6F,0x00},{0x70,0x80},{0x71,0x00},{0x72,0x00},
	{0x73,0x00},{0x74,0xF0},{0x75,0x00},{0x80,0x42},{0x81,0x44},{0x82,0x04},
	{0x83,0x20},{0x84,0x20},{0x85,0x00},{0x86,0x10},{0x87,0x00},{0x88,0x05},
	{0x89,0x18},{0x8A,0x10},{0x8B,0x01},{0x8C,0x37},{0x8D,0x00},{0x8E,0xF0},
	{0x8F,0x81},{0x90,0x06},{0x91,0x06},{0x92,0x1E},{0x93,0x0D},{0x94,0x0A},
	{0x95,0x0A},{0x96,0x0C},{0x97,0x05},{0x98,0x0A},{0x99,0x41},{0x9A,0x14},
	{0x9B,0x0A},{0x9C,0x3F},{0x9D,0x33},{0x9E,0xAE},{0x9F,0xF9},{0xA0,0x48},
	{0xA1,0x13},{0xA2,0x10},{0xA3,0x08},{0xA4,0x30},{0xA5,0x19},{0xA6,0x10},
	{0xA7,0x08},{0xA8,0x24},{0xA9,0x04},{0xAA,0x1E},{0xAB,0x1E},{0xCC,0x19},
	{0xCD,0x0B},{0xCE,0x13},{0xCF,0x64},{0xD0,0x21},{0xD1,0x0F},{0xD2,0x88},
	{0xE0,0x01},{0xE1,0x04},{0xE2,0x41},{0xE3,0xD6},{0xE4,0x00},{0xE5,0x0C},
	{0xE6,0x0A},{0xE7,0x00},{0xE8,0x00},{0xE9,0x00},{0xEE,0x07},{0xEF,0x01},
    {0x00,0x1E},{0x01,0x1E},{0x02,0x0F},{0x03,0x10},{0x04,0x02},{0x05,0x00},
	{0x06,0xB0},{0x07,0x04},{0x08,0x0D},{0x09,0x0E},{0x0A,0x9C},{0x0B,0x04},
	{0x0C,0x05},{0x0D,0x0F},{0x0E,0x02},{0x0F,0x12},{0x10,0x02},{0x11,0x02},
	{0x12,0x00},{0x13,0x01},{0x14,0x05},{0x15,0x07},{0x16,0x05},{0x17,0x07},
	{0x18,0x01},{0x19,0x04},{0x1A,0x05},{0x1B,0x0C},{0x1C,0x2A},{0x1D,0x01},
	{0x1E,0x00},{0x21,0x00},{0x22,0x00},{0x23,0x00},{0x25,0x01},{0x26,0x00},
    {0x27,0x39},{0x28,0x7F},{0x29,0x08},{0x30,0x03},{0x31,0x00},{0x32,0x1A},
	{0x33,0x1A},{0x34,0x07},{0x35,0x07},{0x36,0x01},{0x37,0xFF},{0x38,0x36},
	{0x39,0x07},{0x3A,0x00},{0x3E,0xFF},{0x3F,0x00},{0x40,0x77},{0x41,0x40},
	{0x42,0x00},{0x43,0x30},{0x44,0xA0},{0x45,0x5C},{0x46,0x00},{0x47,0x00},
	{0x48,0x58},{0x4A,0x1E},{0x4B,0x1E},{0x4C,0x00},{0x4D,0x00},{0x4E,0xA0},
    {0x4F,0x80},{0x50,0x00},{0x51,0x00},{0x52,0x00},{0x53,0x00},{0x54,0x00},
	{0x57,0x80},{0x59,0x10},{0x5A,0x08},{0x5B,0x94},{0x5C,0xE8},{0x5D,0x08},
	{0x5E,0x3D},{0x5F,0x99},{0x60,0x45},{0x61,0x40},{0x63,0x2D},{0x64,0x02},
	{0x65,0x96},{0x66,0x00},{0x67,0x97},{0x68,0x01},{0x69,0xCD},{0x6A,0x01},
	{0x6B,0xB0},{0x6C,0x04},{0x6D,0x2C},{0x6E,0x01},{0x6F,0x32},{0x71,0x00},
	{0x72,0x01},{0x73,0x35},{0x74,0x00},{0x75,0x33},{0x76,0x31},{0x77,0x01},
	{0x7C,0x84},{0x7D,0x03},{0x7E,0x01},
};

struct info {
    i2c i2c_dev;
    unsigned char address;
    int count;
};

static struct info info[DEVICE_MAX];

static int next_index() {
    for (int i = 0; i < DEVICE_MAX; ++i) {
        if (info[i].count == 0) return i;
    }
    return -1;
}

static int paj7620WriteReg(grove_gesture p, uint8_t addr, uint8_t cmd) {
    /* Write to register
    
    Parameters
    ----------
    addr: uint8_t
        8-bit address
    amd: uint8_t
        8-bit write data

    */
    i2c i2c_dev = info[p].i2c_dev;
    uint8_t temp[2];
    temp[0] = addr;
    temp[1] = cmd;
    if(i2c_write(i2c_dev, PAJ7620_ID, temp, 2) != 2) return -EIO;
    else return 0;
}

static int paj7620ReadReg(grove_gesture p, uint8_t addr, uint8_t qty, uint8_t data[]) {
    /* Read form register

    Parameters
    ----------
    addr: uint8_t
        8-bit address
    qty: uint8_t
        Number of bytes to read
    data[]: uint8_t
        Array of read data in bytes 
    
    */
    i2c i2c_dev = info[p].i2c_dev;
    if (i2c_write(i2c_dev, PAJ7620_ID, &addr, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, PAJ7620_ID, data, qty) != qty) return -EIO;
    return 0;
}

static int paj7620SelectBank(grove_gesture p, bank_e bank) {
    /* Select register bank
    
    Parameters
    ----------
    bank: int
        bank number

    */
    i2c i2c_dev = info[p].i2c_dev;
    switch(bank){
        case BANK0:
            if (paj7620WriteReg(i2c_dev, PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK0) == -EIO) return -EIO;
            break;
        case BANK1:
            if (paj7620WriteReg(i2c_dev, PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK1) == -EIO) return -EIO;
            break;
        default:
            break;
    }
    return 0;
}

static int set_default_config(grove_gesture p) {
    /* Default configuration for the device

    */
    i2c i2c_dev = info[p].i2c_dev;
    int i = 0;
    uint8_t data0 = 0, data1 = 0;
    uint8_t error;

    delay_us(700);


    if (paj7620SelectBank(i2c_dev, BANK0) == -EIO) return -EIO;

    error = paj7620ReadReg(i2c_dev, 0, 1, &data0);
    if (error != 0)
    {
        return -1;
    }
    error = paj7620ReadReg(i2c_dev, 1, 1, &data1);
    if (error != 0)
    {
        return -1;
    }
    if ( (data0 != 0x20 ) || (data1 != 0x76) )
    {
        return -1;
    }

    for (i = 0; i < INIT_REG_ARRAY_SIZE; i++)
    {
        if (paj7620WriteReg(i2c_dev, initRegisterArray[i][0], initRegisterArray[i][1]) == -EIO) return -EIO;
    }

    if (paj7620SelectBank(i2c_dev, BANK1) == -EIO) return -EIO;  //gesture flag reg in Bank1
    if (paj7620WriteReg(i2c_dev, 0x65, 0x12) == -EIO) return -EIO;  // near mode 240 fps

    if (paj7620SelectBank(i2c_dev, BANK0) == -EIO) return -1;  //gesture flag reg in Bank0

    return 0;
}

grove_gesture grove_gesture_open(int grove_id) {
    return grove_gesture_open_at_address(grove_id, I2C_ADDRESS);
}

grove_gesture grove_gesture_open_at_address(int grove_id, int address) {
    grove_gesture dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].i2c_dev = i2c_open_grove(grove_id);
        info[dev_id].address = address;
        if (set_default_config(dev_id) == -EIO) {
            info[dev_id].count--;
            i2c_close(info[dev_id].i2c_dev);
            return -EIO;
        }
    }
    return dev_id;
}

void grove_gesture_close(grove_gesture p) {
    if (--info[p].count != 0) return;
    i2c i2c_dev = info[p].i2c_dev;
    i2c_close(i2c_dev);
}

py_int grove_gesture_gesture(grove_gesture p) {
    i2c i2c_dev = info[p].i2c_dev;
    uint8_t data = 0, data1 = 0, error;

    // Read Bank_0_Reg_0x43/0x44 for gesture result.
    error = paj7620ReadReg(i2c_dev, 0x43, 1, &data);
    if (!error)
    {
        switch (data)
        {
            case GES_RIGHT_FLAG:
                delay_ms(GES_ENTRY_TIME);
                if (paj7620ReadReg(i2c_dev, 0x43, 1, &data) == -EIO) return -EIO;
                if(data == GES_FORWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return FORWORD;
                }
                else if(data == GES_BACKWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return BACKWORD;
                }
                else
                {
                    return RIGHT;
                }
            case GES_LEFT_FLAG:
                delay_ms(GES_ENTRY_TIME);
                if (paj7620ReadReg(i2c_dev, 0x43, 1, &data) == -EIO) return -EIO;
                if(data == GES_FORWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return FORWORD;
                }
                else if(data == GES_BACKWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return BACKWORD;
                }
                else
                {
                    return LEFT;
                }
            case GES_UP_FLAG:
                delay_ms(GES_ENTRY_TIME);
                if (paj7620ReadReg(i2c_dev, 0x43, 1, &data) == -EIO) return -EIO;
                if(data == GES_FORWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return FORWORD;
                }
                else if(data == GES_BACKWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return BACKWORD;
                }
                else
                {
                    return UP;
                }
            case GES_DOWN_FLAG:
                delay_ms(GES_ENTRY_TIME);
                if (paj7620ReadReg(i2c_dev, 0x43, 1, &data) == -EIO) return -EIO;
                if(data == GES_FORWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return FORWORD;
                }
                else if(data == GES_BACKWARD_FLAG)
                {
                    delay_ms(GES_QUIT_TIME);
                    return BACKWORD;
                }
                else
                {
                    return DOWN;
                }
            case GES_FORWARD_FLAG:
                delay_ms(GES_QUIT_TIME);
                return FORWORD;
            case GES_BACKWARD_FLAG:
                delay_ms(GES_QUIT_TIME);
                return BACKWORD;
            case GES_CLOCKWISE_FLAG:
                return CLK_WISE;
            case GES_COUNT_CLOCKWISE_FLAG:
                return AN_CLK_WISE;
            default:
                if (paj7620ReadReg(i2c_dev, 0x44, 1, &data1) == -EIO) return -EIO;
                if (data1 == GES_WAVE_FLAG)
                {
                    return WAVE;
                }
        }
    }

   return 0;
}