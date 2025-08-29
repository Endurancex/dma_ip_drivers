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

/*
  Control Device Reg List
*/
// Target ID
#define CTRL_TAR_H2C_CH             0x0
#define CTRL_TAR_C2H_CH             0x1
#define CTRL_TAR_IRQ                0x2
#define CTRL_TAR_CONFIG             0x3
#define CTRL_TAR_H2C_SGDMA          0x4
#define CTRL_TAR_C2H_SGDMA          0x5
#define CTRL_TAR_SGDMA_COM          0x6
#define CTRL_TAR_MSIX               0x8
#define CTRL_TAR_SHIFT              12
// Channel ID
#define CTRL_CHID_0                 0x0
#define CTRL_CHID_1                 0x1
#define CTRL_CHID_2                 0x2
#define CTRL_CHID_3                 0x3
#define CTRL_CHID_SHIFT             8
// Target Base Address
#define CTRL_H2C_CH0_ADDR       (CTRL_TAR_H2C_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_0<<CTRL_CHID_SHIFT)
#define CTRL_H2C_CH1_ADDR       (CTRL_TAR_H2C_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_1<<CTRL_CHID_SHIFT)
#define CTRL_H2C_CH2_ADDR       (CTRL_TAR_H2C_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_2<<CTRL_CHID_SHIFT)
#define CTRL_H2C_CH3_ADDR       (CTRL_TAR_H2C_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_3<<CTRL_CHID_SHIFT)
#define CTRL_C2H_CH0_ADDR       (CTRL_TAR_C2H_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_0<<CTRL_CHID_SHIFT)
#define CTRL_C2H_CH1_ADDR       (CTRL_TAR_C2H_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_1<<CTRL_CHID_SHIFT)
#define CTRL_C2H_CH2_ADDR       (CTRL_TAR_C2H_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_2<<CTRL_CHID_SHIFT)
#define CTRL_C2H_CH3_ADDR       (CTRL_TAR_C2H_CH<<CTRL_TAR_SHIFT) + (CTRL_CHID_3<<CTRL_CHID_SHIFT)
#define CTRL_CONFIG_ADDR        (CTRL_TAR_CONFIG<<CTRL_TAR_SHIFT)
#define CTRL_H2C0_SGDMA_ADDR    (CTRL_TAR_H2C_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_0<<CTRL_CHID_SHIFT)
#define CTRL_H2C1_SGDMA_ADDR    (CTRL_TAR_H2C_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_1<<CTRL_CHID_SHIFT)
#define CTRL_H2C2_SGDMA_ADDR    (CTRL_TAR_H2C_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_2<<CTRL_CHID_SHIFT)
#define CTRL_H2C3_SGDMA_ADDR    (CTRL_TAR_H2C_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_3<<CTRL_CHID_SHIFT)
#define CTRL_C2H0_SGDMA_ADDR    (CTRL_TAR_C2H_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_0<<CTRL_CHID_SHIFT)
#define CTRL_C2H1_SGDMA_ADDR    (CTRL_TAR_C2H_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_1<<CTRL_CHID_SHIFT)
#define CTRL_C2H2_SGDMA_ADDR    (CTRL_TAR_C2H_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_2<<CTRL_CHID_SHIFT)
#define CTRL_C2H3_SGDMA_ADDR    (CTRL_TAR_C2H_SGDMA<<CTRL_TAR_SHIFT) + (CTRL_CHID_3<<CTRL_CHID_SHIFT)
#define CTRL_SGDMA_COM_ADDR     (CTRL_TAR_SGDMA_COM<<CTRL_TAR_SHIFT)
    
// Register
#define H2C_CH_ID                   0x00
#define H2C_CH_CTRL0                0x04
#define H2C_CH_CTRL1                0x08 // W1S
#define H2C_CH_CTRL2                0x0C // W1C
#define H2C_CH_STATUS0              0x40
#define H2C_CH_STATUS1              0x44
#define H2C_CH_COMP_DSC_CNT         0x48
#define H2C_CH_ALIGNMENT            0x4C
#define H2C_CH_INTR_ENABLE0         0x90
#define H2C_CH_INTR_ENABLE1         0x94
#define H2C_CH_INTR_ENABLE2         0x98

#define C2H_CH_ID                   0x00
#define C2H_CH_CTRL0                0x04
#define C2H_CH_CTRL1                0x08 // W1S
#define C2H_CH_CTRL2                0x0C // W1C
#define C2H_CH_STATUS0              0x40
#define C2H_CH_STATUS1              0x44
#define C2H_CH_COMP_DSC_CNT         0x48
#define C2H_CH_ALIGNMENT            0x4C
#define C2H_CH_INTR_ENABLE0         0x90
#define C2H_CH_INTR_ENABLE1         0x94
#define C2H_CH_INTR_ENABLE2         0x98

#define CONFIG_ID                   0x00
#define CONFIG_BUSDEV               0x04
#define CONFIG_PCIE_MAXPAYLOAD      0x08
#define CONFIG_PCIE_MAXRDREQ        0x0C
#define CONFIG_SYSTEM_ID            0x10
#define CONFIG_MSI_ENABLE           0x14
#define CONFIG_PCIE_DW              0x18
#define CONFIG_PCIE_CONTROL         0x1C // RW
#define CONFIG_USER_MAXPAYLOAD      0x40
#define CONFIG_USER_MAXRDREQ        0x44
#define CONFIG_WR_FLUSH_TIMEOUT     0x60

#define H2C_SGDMA_ID                0x00
#define H2C_SGDMA_DSC_ADDR_LOW      0x80
#define H2C_SGDMA_DSC_ADDR_HIGH     0x84
#define H2C_SGDMA_DSC_ADJ           0x88
#define H2C_SGDMA_DSC_CREDITS       0x8C

#define C2H_SGDMA_ID                0x00
#define C2H_SGDMA_DSC_ADDR_LOW      0x80
#define C2H_SGDMA_DSC_ADDR_HIGH     0x84
#define C2H_SGDMA_DSC_ADJ           0x88
#define C2H_SGDMA_DSC_CREDITS       0x8C

#define SGDMA_COM_ID                0x00
#define SGDMA_COM_DSC_CTRL0         0x10
#define SGDMA_COM_DSC_CTRL1         0x14
#define SGDMA_COM_DSC_CTRL2         0x18
#define SGDMA_COM_DSC_CREDIT_MOD_EN 0x20
#define SGDMA_COM_DSC_MOD_EN0       0x24
#define SGDMA_COM_DSC_MOD_EN1       0x28


typedef struct {
    uint32_t addr;    
    const char *name; 
} reg_t;

// extern reg_t h2c_ch_reg_list[];
// extern reg_t c2h_ch_reg_list[];
// extern reg_t config_reg_list[];
// extern reg_t h2c_sgdma_reg_list[];
// extern reg_t c2h_sgdma_reg_list[];
// extern reg_t sgdma_com_reg_list[];

#define H2C_CH_REG_NUM      (sizeof(h2c_ch_reg_list)/sizeof(h2c_ch_reg_list[0]))
#define C2H_CH_REG_NUM      (sizeof(c2h_ch_reg_list)/sizeof(c2h_ch_reg_list[0]))
#define CONFIG_REG_NUM      (sizeof(config_reg_list)/sizeof(config_reg_list[0]))
#define H2C_SGDMA_REG_NUM   (sizeof(h2c_sgdma_reg_list)/sizeof(h2c_sgdma_reg_list[0]))
#define C2H_SGDMA_REG_NUM   (sizeof(c2h_sgdma_reg_list)/sizeof(c2h_sgdma_reg_list[0]))
#define SGDMA_COM_REG_NUM   (sizeof(sgdma_com_reg_list)/sizeof(sgdma_com_reg_list[0]))

int set_control_run(char* channel_type, uint32_t channel_id);
int check_control_status(char* channel_type, uint32_t channel_id, uint32_t* status);
int check_control_comp_dsc_cnt(char* channel_type, uint32_t channel_id, uint32_t *comp_dsc_cnt);
int control_reg_dump();