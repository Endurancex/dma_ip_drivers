#include "control_reg.h"

reg_t h2c_ch_reg_list[] = {
    {0x00, "  H2C_CH_ID                   "},
    {0x04, "  H2C_CH_CTRL0                "},
    {0x08, "  H2C_CH_CTRL1                "},
    {0x0C, "  H2C_CH_CTRL2                "},
    {0x40, "  H2C_CH_STATUS0              "},
    {0x44, "  H2C_CH_STATUS1              "},
    {0x48, "  H2C_CH_COMP_DSC_CNT         "},
    {0x4C, "  H2C_CH_ALIGNMENT            "},
    {0x90, "  H2C_CH_INTR_ENABLE0         "},
    {0x94, "  H2C_CH_INTR_ENABLE1         "},
    {0x98, "  H2C_CH_INTR_ENABLE2         "},
};
reg_t c2h_ch_reg_list[] = {
    {0x00, "  C2H_CH_ID                   "},
    {0x04, "  C2H_CH_CTRL0                "},
    {0x08, "  C2H_CH_CTRL1                "},
    {0x0C, "  C2H_CH_CTRL2                "},
    {0x40, "  C2H_CH_STATUS0              "},
    {0x44, "  C2H_CH_STATUS1              "},
    {0x48, "  C2H_CH_COMP_DSC_CNT         "},
    {0x4C, "  C2H_CH_ALIGNMENT            "},
    {0x90, "  C2H_CH_INTR_ENABLE0         "},
    {0x94, "  C2H_CH_INTR_ENABLE1         "},
    {0x98, "  C2H_CH_INTR_ENABLE2         "},
};
reg_t config_reg_list[] = {
    {0x00, "  CONFIG_ID                   "},
    {0x04, "  CONFIG_BUSDEV               "},
    {0x08, "  CONFIG_PCIE_MAXPAYLOAD      "},
    {0x0C, "  CONFIG_PCIE_MAXRDREQ        "},
    {0x10, "  CONFIG_SYSTEM_ID            "},
    {0x14, "  CONFIG_MSI_ENABLE           "},
    {0x18, "  CONFIG_PCIE_DW              "},
    {0x1C, "  CONFIG_PCIE_CONTROL         "},
    {0x40, "  CONFIG_USER_MAXPAYLOAD      "},
    {0x44, "  CONFIG_USER_MAXRDREQ        "},
    {0x60, "  CONFIG_WR_FLUSH_TIMEOUT     "},
};
reg_t h2c_sgdma_reg_list[] = {
    {0x00, "  H2C_SGDMA_ID                "},
    {0x80, "  H2C_SGDMA_DSC_ADDR_LOW      "},
    {0x84, "  H2C_SGDMA_DSC_ADDR_HIGH     "},
    {0x88, "  H2C_SGDMA_DSC_ADJ           "},
    {0x8C, "  H2C_SGDMA_DSC_CREDITS       "},
};
reg_t c2h_sgdma_reg_list[] = {
    {0x00, "  C2H_SGDMA_ID                "},
    {0x80, "  C2H_SGDMA_DSC_ADDR_LOW      "},
    {0x84, "  C2H_SGDMA_DSC_ADDR_HIGH     "},
    {0x88, "  C2H_SGDMA_DSC_ADJ           "},
    {0x8C, "  C2H_SGDMA_DSC_CREDITS       "},
};
reg_t sgdma_com_reg_list[] = {
    {0x00, "  SGDMA_COM_ID                "},
    {0x10, "  SGDMA_COM_DSC_CTRL0         "},
    {0x14, "  SGDMA_COM_DSC_CTRL1         "},
    {0x18, "  SGDMA_COM_DSC_CTRL2         "},
    {0x20, "  SGDMA_COM_DSC_CREDIT_MOD_EN "},
    {0x24, "  SGDMA_COM_DSC_MOD_EN0       "},
    {0x28, "  SGDMA_COM_DSC_MOD_EN1       "},
};


int set_control_run(char* channel_type, uint32_t channel_id){
    int err = 0;
    uint32_t addr;
    uint32_t rd_data;
    if(strcmp(channel_type, "H2C") == 0){
        addr = (CTRL_TAR_H2C_CH<<CTRL_TAR_SHIFT) + (channel_id<<CTRL_CHID_SHIFT) + H2C_CH_CTRL0;
    }
    else if(strcmp(channel_type, "C2H") == 0){
        addr = (CTRL_TAR_C2H_CH<<CTRL_TAR_SHIFT) + (channel_id<<CTRL_CHID_SHIFT) + C2H_CH_CTRL0;
    }
    else{
        err = 1;
        return err;
    }
    reg_read("/dev/xdma0_control", addr, &rd_data);
    reg_write("/dev/xdma0_control", addr, (rd_data|0x1));//run

    return err;
}

int check_control_status(char* channel_type, uint32_t channel_id, uint32_t* status){
    int err = 0;
    uint32_t addr;
    uint32_t rd_data;

    if(strcmp(channel_type, "H2C") == 0){
        addr = (CTRL_TAR_H2C_CH<<CTRL_TAR_SHIFT) + (channel_id<<CTRL_CHID_SHIFT) + H2C_CH_STATUS0;
    }
    else if(strcmp(channel_type, "C2H") == 0){
        addr = (CTRL_TAR_C2H_CH<<CTRL_TAR_SHIFT) + (channel_id<<CTRL_CHID_SHIFT) + C2H_CH_STATUS0;
    }
    else{
        err = 1;
        return err;
    }
    reg_read("/dev/xdma0_control", addr, status);

    return err;
}
int check_control_comp_dsc_cnt(char* channel_type, uint32_t channel_id, uint32_t *comp_dsc_cnt){
    int err = 0;
    uint32_t addr;
    uint32_t rd_data;

    if(strcmp(channel_type, "H2C") == 0){
        addr = (CTRL_TAR_H2C_CH<<CTRL_TAR_SHIFT) + (channel_id<<CTRL_CHID_SHIFT) + H2C_CH_COMP_DSC_CNT;
    }
    else if(strcmp(channel_type, "C2H") == 0){
        addr = (CTRL_TAR_C2H_CH<<CTRL_TAR_SHIFT) + (channel_id<<CTRL_CHID_SHIFT) + C2H_CH_COMP_DSC_CNT;
    }
    else{
        err = 1;
        return err;
    }
    reg_read("/dev/xdma0_control", addr, comp_dsc_cnt);

    return err;
}
int control_reg_dump(){
    int err = 0;
    uint32_t rd_data;
    uint32_t addr;
    char *device;

    printf("==================== Print Control Info Start =====================\n");
    device = "/dev/xdma0_control";
    for (int i = 0; i < H2C_CH_REG_NUM; i++) {
        addr = CTRL_H2C_CH0_ADDR + h2c_ch_reg_list[i].addr;
        err = reg_read(device, addr, &rd_data);
        printf("%s (0x%04X) = 0x%08X\n", h2c_ch_reg_list[i].name, addr, rd_data);
    }
    printf("\n");
    for (int i = 0; i < C2H_CH_REG_NUM; i++) {
        addr = CTRL_C2H_CH0_ADDR + c2h_ch_reg_list[i].addr;
        err = reg_read(device, addr, &rd_data);
        printf("%s (0x%04X) = 0x%08X\n", c2h_ch_reg_list[i].name, addr, rd_data);
    }
    printf("\n");
    for (int i = 0; i < CONFIG_REG_NUM; i++) {
        addr = CTRL_CONFIG_ADDR + config_reg_list[i].addr;
        err = reg_read(device, addr, &rd_data);
        printf("%s (0x%04X) = 0x%08X\n", config_reg_list[i].name, addr, rd_data);
    }
    printf("\n");
    for (int i = 0; i < H2C_SGDMA_REG_NUM; i++) {
        addr = CTRL_H2C0_SGDMA_ADDR + h2c_sgdma_reg_list[i].addr;
        err = reg_read(device, addr, &rd_data);
        printf("%s (0x%04X) = 0x%08X\n", h2c_sgdma_reg_list[i].name, addr, rd_data);
    }
    printf("\n");
    for (int i = 0; i < H2C_SGDMA_REG_NUM; i++) {
        addr = CTRL_C2H0_SGDMA_ADDR + c2h_sgdma_reg_list[i].addr;
        err = reg_read(device, addr, &rd_data);
        printf("%s (0x%04X) = 0x%08X\n", c2h_sgdma_reg_list[i].name, addr, rd_data);
    }
    printf("\n");
    for (int i = 0; i < SGDMA_COM_REG_NUM; i++) {
        addr = CTRL_SGDMA_COM_ADDR + sgdma_com_reg_list[i].addr;
        err = reg_read(device, addr, &rd_data);
        printf("%s (0x%04X) = 0x%08X\n", sgdma_com_reg_list[i].name, addr, rd_data);
    }
    printf("\n");
    printf("==================== Print Control Info Done =====================\n");
    return err;
}