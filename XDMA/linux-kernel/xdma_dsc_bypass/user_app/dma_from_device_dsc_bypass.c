#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "reg_rw.h"
#include "control_reg.h"

#define MEM_SIZE (1*1024*1024) // 1MB

extern reg_t h2c_ch_reg_list[];
extern reg_t c2h_ch_reg_list[];
extern reg_t config_reg_list[];
extern reg_t h2c_sgdma_reg_list[];
extern reg_t c2h_sgdma_reg_list[];
extern reg_t sgdma_com_reg_list[];

int main() {
    uint32_t status;
    uint32_t comp_dsc_cnt;

    control_reg_dump();

    int fd = open("/dev/mem", O_RDWR);
    if (fd < 0) { perror("open"); return -1; }

    void *ptr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); return -1; }

    // printf("mem mapped at %p, 0x%x\n", ptr, (uint64_t)(uintptr_t)ptr);
    printf("\n");

    printf("==================== ARM C2H0 Transfer =====================\n");
    set_control_run("C2H", 0);
    printf("==================== C2H0 Transfer ARMed =====================\n");
    printf("\n");

    printf("==================== Configure Data Stimulus Module =====================\n");
    uint64_t transferSz = 0x80000; // 512KByte
    uint64_t packet_len=transferSz/32; // Cycle

    set_user_c2h_dsc(0, (uint64_t)(uintptr_t)ptr, transferSz);
    set_user_h2c_dsc((uint64_t)(uintptr_t)ptr, 0,transferSz);
    set_user_data_gen(packet_len);
    arm_user_c2h_dsc();
    
    arm_user_data_gen();
    sleep(1);
    arm_user_h2c_dsc();
    printf("==================== Configure Data Stimulus Module Done =====================\n");
    printf("\n");

    printf("==================== Check C2H Channel Status in loop =====================\n");
    check_control_status("C2H", 0, &status);
    check_control_comp_dsc_cnt("C2H", 0, &comp_dsc_cnt);

    return 0;
}
