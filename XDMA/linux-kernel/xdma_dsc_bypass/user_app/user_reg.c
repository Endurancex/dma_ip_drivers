/*
 * This file is part of the Xilinx DMA IP Core driver tools for Linux
 *
 * Copyright (c) 2016-present,  Xilinx, Inc.
 * All rights reserved.
 *
 * This source code is licensed under BSD-style license (found in the
 * LICENSE file in the root directory of this source tree)
 */
#include "user_reg.h"

int set_user_c2h_dsc(uint64_t src_addr, uint64_t dest_addr, uint64_t len){
    int err = 0;
    uint32_t addr;
    // uint32_t rd_data;
    addr = C2H_GPIO_ADDR + DSC_SRC_ADDR_LOW;
    reg_write("/dev/xdma0_user", addr, (uint32_t)src_addr);
    addr = C2H_GPIO_ADDR + DSC_SRC_ADDR_HIGH;
    reg_write("/dev/xdma0_user", addr, (uint32_t)(src_addr>>32));
    addr = C2H_GPIO_ADDR + DSC_DEST_ADDR_LOW;
    reg_write("/dev/xdma0_user", addr, (uint32_t)dest_addr);
    addr = C2H_GPIO_ADDR + DSC_DEST_ADDR_HIGH;
    reg_write("/dev/xdma0_user", addr, (uint32_t)(dest_addr>>32));
    addr = C2H_GPIO_ADDR + DSC_LEN_LOW;
    reg_write("/dev/xdma0_user", addr, (uint32_t)len);
    addr = C2H_GPIO_ADDR + DSC_LEN_HIGH;
    reg_write("/dev/xdma0_user", addr, (uint32_t)(len>>32));

    return err;
}
int arm_user_c2h_dsc(){
    int err = 0;
    uint32_t addr;
    // uint32_t rd_data;
    addr = C2H_GPIO_ADDR + DSC_START;
    reg_write("/dev/xdma0_user", addr, 1);
    reg_write("/dev/xdma0_user", addr, 0);

    return err;
}
int set_user_data_gen(uint64_t len){
    int err = 0;
    uint32_t addr;
    // uint32_t rd_data;
    addr = C2H_GPIO_ADDR + DATA_GEN_LEN_LOW;
    reg_write("/dev/xdma0_user", addr, (uint32_t)len);
    addr = C2H_GPIO_ADDR + DATA_GEN_LEN_HIGH;
    reg_write("/dev/xdma0_user", addr, (uint32_t)(len>>32));

    return err;
}

int arm_user_data_gen(){
    int err = 0;
    uint32_t addr;
    // uint32_t rd_data;
    addr = C2H_GPIO_ADDR + DATA_GEN_START;
    reg_write("/dev/xdma0_user", addr, 1);
    reg_write("/dev/xdma0_user", addr, 0);

    return err;
}


int set_user_h2c_dsc(uint64_t src_addr, uint64_t dest_addr, uint64_t len){
    int err = 0;
    uint32_t addr;
    // uint32_t rd_data;
    addr = H2C_GPIO_ADDR + DSC_SRC_ADDR_LOW;
    reg_write("/dev/xdma0_user", addr, (uint32_t)src_addr);
    addr = H2C_GPIO_ADDR + DSC_SRC_ADDR_HIGH;
    reg_write("/dev/xdma0_user", addr, (uint32_t)(src_addr>>32));
    addr = H2C_GPIO_ADDR + DSC_DEST_ADDR_LOW;
    reg_write("/dev/xdma0_user", addr, (uint32_t)dest_addr);
    addr = H2C_GPIO_ADDR + DSC_DEST_ADDR_HIGH;
    reg_write("/dev/xdma0_user", addr, (uint32_t)(dest_addr>>32));
    addr = H2C_GPIO_ADDR + DSC_LEN_LOW;
    reg_write("/dev/xdma0_user", addr, (uint32_t)len);
    addr = H2C_GPIO_ADDR + DSC_LEN_HIGH;
    reg_write("/dev/xdma0_user", addr, (uint32_t)(len>>32));

    return err;
}
int arm_user_h2c_dsc(){
    int err = 0;
    uint32_t addr;
    // uint32_t rd_data;
    addr = H2C_GPIO_ADDR + DSC_START;
    reg_write("/dev/xdma0_user", addr, 1);
    reg_write("/dev/xdma0_user", addr, 0);

    return err;
}
