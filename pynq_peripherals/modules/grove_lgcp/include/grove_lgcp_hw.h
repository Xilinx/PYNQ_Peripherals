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

#define REG_RAM       	0x00  //~0X7F
#define REG_ENABLE     	0x80
#define REG_ATIME      	0x81
#define REG_WTIME      	0x83
#define REG_ALS_IT     	0x84  //~0X87
#define REG_PROX_IT_L   0x89
#define REG_PROX_IT_H   0x8B
#define REG_IT_PERS    	0x8C
#define REG_CONFIG1   	0x8D
#define REG_PPULSE    	0x8E
#define REG_CONTROL   	0x8F
#define REG_CONFIG2   	0x90
#define REG_REVID     	0x91
#define REG_ID        	0x92
#define REG_STATUS    	0x93
#define REG_RGBC_DATA 	0x94  //~9B
#define REG_PROX_DATA 	0x9C
#define REG_POFFSET_NE	0x9D
#define REG_POFFSET_SW	0x9E
#define REG_CONFIG3   	0x9F

//The following CONFIG registers have different definations under different modes
#define REG_CONFIG_A0  0xA0
#define REG_CONFIG_A1  0xA1
#define REG_CONFIG_A2  0xA2
#define REG_CONFIG_A3  0xA3
#define REG_CONFIG_A4  0xA4
#define REG_CONFIG_A5  0xA5
#define REG_CONFIG_A6  0xA6
#define REG_CONFIG_A7  0xA7
#define REG_CONFIG_A8  0xA8
#define REG_CONFIG_A9  0xA9
#define REG_CONFIG_AA  0xAA
#define REG_CONFIG_AB  0xAB
#define REG_CONFIG_AC  0xAC
#define REG_CONFIG_AE  0xAE
#define REG_CONFIG_AF  0xAF

#define REG_PBCLEAR    0xE3
#define REG_IFORCE     0xE4
#define REG_PICLEAR    0xE5
#define REG_CICLEAR    0xE6
#define REG_AICLEAR    0xE7
#define REG_GFIFO_N    0xFC
#define REG_GFIFO_S    0xFD
#define REG_GFIFO_W    0xFE
#define REG_GFIFO_E    0xFF

//Enable bits
#define ENABLE_PON   0x01
#define ENABLE_AEN   0x02
#define ENABLE_PEN   0x04
#define ENABLE_WEN   0x08
#define ENABLE_AIEN  0x10
#define ENABLE_PIEN  0x20
#define ENABLE_GEN   0x40
#define ENABLE_PBEN  0x80

//Status bits
#define STATUS_AVALID    (1)
#define STATUS_PVALID    (1<<1)
#define STATUS_GINT      (1<<2)
#define STATUS_PBINT     (1<<3)
#define STATUS_AINT      (1<<4)
#define STATUS_PINT      (1<<5)
#define STATUS_PGSAT     (1<<6)
#define STATUS_CPSAT     (1<<7)
