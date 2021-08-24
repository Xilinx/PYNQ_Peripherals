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
#include <grove_lgcp.h>
#include <timer.h>

#include "grove_lgcp_hw.h"

#define I2C_ADDRESS 0x39
#define DEVICE_MAX 4

struct info {
    i2c i2c_dev;
    int count;
    unsigned int red_raw;
    unsigned int green_raw;
    unsigned int blue_raw;
    unsigned int clear_raw;
};

static struct info info[DEVICE_MAX];

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

/* Get Device ID
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Return
 * ------
 * 	  int
 * 		Device ID.
 * 
 */
static int get_device_id(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer;
    buffer = REG_ID;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return (buffer >> 2);
}

/* Get Control register content for the proximity 
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Return
 * ------
 * 	  int
 * 		Returns LED Drive Strength, Proximity gain control.
 * 
 */
static int get_control_reg(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_CONTROL;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return (int)buffer;
}

/* Set Control register content for the proximity
 * 
 * Parameters
 * ----------
 *    unsigned char
 *		LED Drive Strength[7:6], Proximity gain control[3:2], ALS gain control[1:0]
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 * 
 */
/* static int set_control_reg(grove_lgcp p, unsigned char control) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_CONTROL;
    buffer[1] = control;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
    return 0;
} */

/* Get Configuration Register Two settings
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		PSIEN[7], CPSIEN[6], LEDBOOST[5:4], 0[3:1],1[0]
 *		-1 otherwise
 * 
 */
 static int get_config2(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_CONFIG2;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return (int)buffer;
}

/* Set Configuration Register Two settings
 * 
 * Parameters
 * ----------
 *    unsigned char
 *		PSIEN[7], CPSIEN[6], LEDBOOST[5:4], 0[3:1],1[0]
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 * 
 */
 static int set_config2(grove_lgcp p, unsigned char config) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_CONFIG2;
    buffer[1] = config;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
    return 0;
}

/* Get Configuration Register Three settings
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		0[7:6], PCMP[5], SAI[4], PMSK_N[3], PMSK_S[2], PMSK_W[1], PMSK_E[0]
 *		-1 otherwise
 * 
 */
static int get_config3(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_CONFIG3;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return (int)buffer;
}

/* Set Configuration Register Three settings
 * 
 * Parameters
 * ----------
 *    unsigned char
 *		0[7:6], PCMP[5], SAI[4], PMSK_N[3], PMSK_S[2], PMSK_W[1], PMSK_E[0]
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 * 
 */
 /* static int set_config3(grove_lgcp p, unsigned char config) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_CONFIG3;
    buffer[1] = config;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
    return 0;
} */

/* Read Status Register 
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		CPSAT[7], PGSAT[6], PINT[5], AINT[4], PBINT[3], GINT[2], PVALID[1], AVALID[0]
 *		-1 otherwise
 * 
 */
 static int get_status(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_STATUS;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return (int)buffer;
}

/* Clear pattern burst interrupt
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 * 
 */
/* static int clear_pattern_burst_interrupts(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_PBCLEAR;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return 0;
} */

/* Force interrupt pin to assert
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 * 
 */
 /* static int force_assert_interrupt_pin(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_IFORCE;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return 0;
} */

/* Set Configuration Register Three settings
 * 
 * Parameters
 * ----------
 *    unsigned char
 *		The amount of time in a low power mode between Proximity and/or ALS 
 *		cycles in multiple of 2.7ms 
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 * 
 
 static int grove_lgcp_set_wait_time(grove_lgcp p, unsigned char wtime) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_WTIME;
    buffer[1] = wtime;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
    return 0;
} 

static int grove_lgcp_enable_wait_time_12x_factor(grove_lgcp p, int enable) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_WTIME;
    if (enable) {
        buffer[1] = 0x62;
    } else {
        buffer[1] = 0x60;
    }
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
    return 0;
} 

static int grove_lgcp_get_interrupt_persistance_reg(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_IT_PERS;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return (int)buffer;
} */

/* Set Interrupt Persistance Register 
 * 
 * Parameters
 * ----------
 *    unsigned char
 *		PPERS[7:4], APERS[3:0] 
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 */
static int set_interrupt_persistance_reg(grove_lgcp p, unsigned char pers) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_IT_PERS;
    buffer[1] = pers;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
    return 0;
}

/* Check if device is ready 
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 */
static int is_device_ready(grove_lgcp p) {
    delay_ms(6);
    if (get_device_id(p) != 0x2a) {
    	return -1;
    }
    return 0;
}

/* Set proximity pulse length 
 * 
 * Parameters
 * ----------
 *    unsigned char cnt, and len
 *		PPLEN[7:6], PPULSE[5:0] 
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 */
 static int set_proximity_pulse_len(grove_lgcp p, unsigned char cnt, unsigned char len) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_PPULSE;
    if (cnt > 63) {
        cnt = 63;
    }
    if (len > 3) {
        len = 3;
    }
    buffer[1] = (cnt & 0x3f) | (len & 0xb0); 
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
    return 0;
}

/*
 static int grove_lgcp_set_als_interrupt_threshold(grove_lgcp p, unsigned int low, unsigned int high) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[5];
    buffer[0] = REG_ALS_IT;
    buffer[1] = low & 0xff;
    buffer[2] = low >> 8;
    buffer[3] = high & 0xff;
    buffer[4] = high >> 8;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 5) != 5) return -1;
    return 0;
}

static int grove_lgcp_clear_als_interrupts(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_CICLEAR;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return 0;
}
S
static int grove_lgcp_clear_all_interrupts(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_AICLEAR;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -1;
    return 0;
}
*/

/* Reset the lgcp chip 
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 */
static int lgcp_reset(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
	unsigned char enable_reg, enable_bits;
    buffer[0] = REG_ENABLE;
    buffer[1] = 0x0; // Turn-off all engines
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -1;
	return 0;
}

/* Set recommended settings for proximity 
 * 
 * Parameters
 * ----------
 *    None
 * 
 * Return
 * ------
 * 	  int
 * 		0 if succesful, -1 otherwise
 */
static int setup_recommended_config_for_proximity(grove_lgcp p) {
    if (set_proximity_pulse_len(p, 63, 1) == -1) return -EIO;
    int ret = get_config2(p);
    unsigned char config2;
    if (ret == -1) {
        return -EIO;
    } else {
        config2 = (unsigned char)ret;
    }
    config2 |= (3 << 4); //300% power
    if (set_config2(p, config2) == -1) return -EIO;
    if (set_interrupt_persistance_reg(p, 0xa << 4) == -1) return -EIO; // 10 consecutive proximity values out of range
    return 0;
}

grove_lgcp grove_lgcp_open(int grove_id) {
    grove_lgcp dev_id = next_index();
    if (dev_id >= 0) {
        info[dev_id].count++;
        info[dev_id].i2c_dev = i2c_open_grove(grove_id);
        info[dev_id].red_raw = 0;
        info[dev_id].green_raw = 0;
        info[dev_id].blue_raw = 0;
        info[dev_id].clear_raw = 0;
        lgcp_reset(dev_id);
        if (is_device_ready(dev_id) != 0) {
            info[dev_id].count--;
            i2c_close(info[dev_id].i2c_dev);
            return -EIO;
        }
    }
    return dev_id;
}

void grove_lgcp_close(grove_lgcp p) {
    info[p].red_raw = 0;
    info[p].green_raw = 0;
    info[p].blue_raw = 0;
    info[p].clear_raw = 0;
    if (--info[p].count != 0) return;
    i2c i2c_dev = info[p].i2c_dev;
    i2c_close(i2c_dev);
}

py_int grove_lgcp_get_enable_reg(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_ENABLE;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return (int)buffer;
}

py_void grove_lgcp_select_proximity(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    int pben = 0;
    unsigned char enable_reg;
	int rst = lgcp_reset(p);
	if (rst == -1) return -EIO;
    int ret = grove_lgcp_get_enable_reg(p);
    if (ret == -1) {
        return -EIO;
    } else {
        enable_reg = (unsigned char)ret;
    }
    unsigned char enable_bits = enable_reg | ENABLE_PEN | ENABLE_PIEN;

    unsigned char buffer[2];
    buffer[0] = REG_ENABLE;
    if (enable_bits & ENABLE_PBEN) {
        enable_bits = ENABLE_PBEN;
        pben = 1;
    }
    buffer[1] = ENABLE_PON | enable_bits;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;

    if (!pben) {
        delay_ms(7);
        return PY_SUCCESS;
    } else {
        return -EIO;
    }
	
	setup_recommended_config_for_proximity(p);
}

py_void grove_lgcp_select_als(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    int pben = 0;
    unsigned char enable_reg;
	int rst = lgcp_reset(p);
	if (rst == -1) return -EIO;
    int ret = grove_lgcp_get_enable_reg(p);
    if (ret == -1) {
        return -EIO;
    } else {
        enable_reg = (unsigned char)ret;
    }
    unsigned char enable_bits = enable_reg | ENABLE_AEN | ENABLE_AIEN;

    unsigned char buffer[2];
    buffer[0] = REG_ENABLE;
    if (enable_bits & ENABLE_PBEN) {
        enable_bits = ENABLE_PBEN;
        pben = 1;
    }
    buffer[1] = ENABLE_PON | enable_bits;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;

    if (!pben) {
        delay_ms(7);
        return PY_SUCCESS;
    } else {
        return -EIO;
    }
    buffer[0] = REG_ATIME;
    buffer[1] = 0xDA;	// approximately 100 ms
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;
}

py_void grove_lgcp_select_gesture(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    int pben = 0;
    unsigned char enable_reg;
    int rst = lgcp_reset(p);
    if (rst == -1) return -EIO;
    int ret = grove_lgcp_get_enable_reg(p);
    if (ret == -1) {
        return -EIO;
    } else {
        enable_reg = (unsigned char)ret;
    }
    unsigned char enable_bits = enable_reg | ENABLE_GEN;

    unsigned char buffer[2];
    buffer[0] = REG_ENABLE;
    if (enable_bits & ENABLE_PBEN) {
        enable_bits = ENABLE_PBEN;
        pben = 1;
    }
    buffer[1] = ENABLE_PON | enable_bits;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;

    if (!pben) {
        delay_ms(7);
        buffer[0] = REG_CONFIG_AB;
        buffer[1] = 0x11;
        if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;
        buffer[0] = REG_CONFIG_A2;
        buffer[1] = 0xc0;
        if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;
        buffer[0] = REG_CONFIG_A3;
        buffer[1] = 0x20;
        if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;
        return PY_SUCCESS;
    } else {
        return -EIO;
    }
}

py_int grove_lgcp_get_gesture_north(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer1[2];
    buffer1[0] = REG_CONFIG_AA;
    buffer1[1] = 0x03;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer1, 2) != 2) return -EIO;

    unsigned char buffer = REG_GFIFO_N;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return (int)buffer;
}

py_int grove_lgcp_get_gesture_south(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_GFIFO_S;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return (int)buffer;
}

py_int grove_lgcp_get_gesture_west(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_GFIFO_W;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return (int)buffer;
}

py_int grove_lgcp_get_gesture_east(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_GFIFO_E;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return (int)buffer;
}

/*
py_void grove_lgcp_reset(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_ENABLE;
    buffer[1] = 0x0;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;
    return PY_SUCCESS;
}
*/

py_void grove_lgcp_set_adc_integration_time(grove_lgcp p, unsigned char atime) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_ATIME;
    buffer[1] = atime;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;
    return PY_SUCCESS;
}

//proximity
/*
py_void grove_lgcp_setup_recommended_config_for_proximity(grove_lgcp p) {
    if (set_proximity_pulse_len(p, 63, 1) == -1) return -EIO;
    int ret = get_config2(p);
    unsigned char config2;
    if (ret == -1) {
        return -EIO;
    } else {
        config2 = (unsigned char)ret;
    }
    config2 |= (3 << 4); //300% power
    if (set_config2(p, config2) == -1) return -EIO;
    if (set_interrupt_persistance_reg(p, 0xa << 4) == -1) return -EIO; // 10 consecutive proximity values out of range
    return PY_SUCCESS;
}
*/

py_void grove_lgcp_set_proximity_range(grove_lgcp p, unsigned char low, unsigned char high) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer[2];
    buffer[0] = REG_PROX_IT_L;
    buffer[1] = low;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;

    buffer[0] = REG_PROX_IT_H;
    buffer[1] = high;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 2) != 2) return -EIO;
    return PY_SUCCESS;
}

py_int grove_lgcp_get_proximity_raw(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_PROX_DATA;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return (int)buffer;
}

py_int grove_lgcp_is_proximity_detected(grove_lgcp p) {
    unsigned char status = (unsigned char)get_status(p);
    if ((status & 0x20) == 0x20) {
        return 1;
    } else {
        return PY_SUCCESS;
    }
}

//ALS(light)
py_int grove_lgcp_get_rgbc_raw(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
	unsigned char buffer[8];
    buffer[0] = REG_RGBC_DATA;
    if (i2c_write(i2c_dev, I2C_ADDRESS, buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, buffer, 8) != 8) return -EIO;
    info[p].clear_raw = (buffer[1] << 8) | buffer[0];
    info[p].red_raw = (buffer[3] << 8) | buffer[2];
    info[p].green_raw = (buffer[5] << 8) | buffer[4];
    info[p].blue_raw = (buffer[7] << 8) | buffer[6];
    return PY_SUCCESS;
}

py_int grove_lgcp_red(grove_lgcp p) {
    return info[p].red_raw;
}

py_int grove_lgcp_green(grove_lgcp p) {
    return info[p].green_raw;
}

py_int grove_lgcp_blue(grove_lgcp p) {
    return info[p].blue_raw;
}

py_int grove_lgcp_clear(grove_lgcp p) {
    return info[p].clear_raw;
}

py_int grove_lgcp_get_lux(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    if (grove_lgcp_get_rgbc_raw(p) == -1) return -EIO;
    unsigned int R = info[p].red_raw;
    unsigned int G = info[p].green_raw;
    unsigned int B = info[p].blue_raw;
    unsigned int C = info[p].clear_raw;
    unsigned char buffer = REG_ATIME;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    float ms = (256 - buffer) * 2.78;

    int ret = get_control_reg(p);
    if (ret == -1){
        return -1;
    } else {
        buffer = (unsigned char)ret;
    }
    int gain;
    switch (buffer & 0x3) {
        case 0x0:
            gain = 1;
            break;
        case 0x1:
            gain = 4;
            break;
        case 0x2:
            gain = 16;
            break;
        case 0x3:
            gain = 64;
            break;
        default:
            gain = 1;
    }

    int IR;
    float CPL, Y, L;
    IR = (R + G + B - C) / 2;
    if (IR < 0) {
        IR = 0;
    }
    Y = 0.362 * (R - IR) + 1 * (G - IR) + 0.136 * (B - IR);
    CPL = ms * gain / 412;
    L = Y / CPL;
   
    return int(L); 
}

py_int grove_lgcp_get_cct(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    if (grove_lgcp_get_rgbc_raw(p) == -1) return -EIO;
    unsigned int R = info[p].red_raw;
    unsigned int G = info[p].green_raw;
    unsigned int B = info[p].blue_raw;
    unsigned int C = info[p].clear_raw;
    int IR, minV;
    IR = (R + G + B - C) / 2;
    if (IR < 0) {
        IR = 0;
    }

    if (R <= B) {
        minV = R;
    } else {
        minV = B;
    }
    if (IR < minV) {
        IR = minV - 0.1;
    }

    float rate = (float)(B - R) / (float)(R - IR);

    return 2745 * (int)rate + 2242;
}

py_void grove_lgcp_clear_proximity_interrupts(grove_lgcp p) {
    i2c i2c_dev = info[p].i2c_dev;
    unsigned char buffer = REG_PICLEAR;
    if (i2c_write(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, I2C_ADDRESS, &buffer, 1) != 1) return -EIO;
    return PY_SUCCESS;
}

