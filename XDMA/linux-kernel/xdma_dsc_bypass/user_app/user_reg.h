/*
 * This file is part of the Xilinx DMA IP Core driver tools for Linux
 *
 * Copyright (c) 2016-present,  Xilinx, Inc.
 * All rights reserved.
 *
 * This source code is licensed under BSD-style license (found in the
 * LICENSE file in the root directory of this source tree)
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <byteswap.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/mman.h>


#define C2H_GPIO_ADDR                   0x0000
#define H2C_GPIO_ADDR                   0x4000
#define DSC_SRC_ADDR_LOW                0x00    // slv_reg0
#define DSC_SRC_ADDR_HIGH               0x04    // slv_reg1
#define DSC_DEST_ADDR_LOW               0x08    // slv_reg2
#define DSC_DEST_ADDR_HIGH              0x0C    // slv_reg3
#define DSC_LEN_LOW                     0x10    // slv_reg4
#define DSC_LEN_HIGH                    0x14    // slv_reg5
#define DSC_START                       0x18    // slv_reg6
#define DATA_GEN_LEN_LOW                0x1C    // slv_reg7
#define DATA_GEN_LEN_HIGH               0x20    // slv_reg8
#define DATA_GEN_START                  0x24    // slv_reg9


int set_user_c2h_dsc(uint64_t src_addr, uint64_t dest_addr, uint64_t len);
int arm_user_c2h_dsc();
int set_user_data_gen(uint64_t len);
int arm_user_data_gen();
int set_user_h2c_dsc(uint64_t src_addr, uint64_t dest_addr, uint64_t len);
int arm_user_h2c_dsc();
