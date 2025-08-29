/*
 * This file is part of the Xilinx DMA IP Core driver tools for Linux
 *
 * Copyright (c) 2016-present,  Xilinx, Inc.
 * All rights reserved.
 *
 * This source code is licensed under BSD-style license (found in the
 * LICENSE file in the root directory of this source tree)
 */
#include "reg_rw.h"
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) \
        do { printf(fmt, ##__VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif


int reg_read(char *device, uint32_t addr, uint32_t *rd_data)
{
	int fd;
	int err = 0;
	void *map;
	off_t target = addr;
	off_t pgsz, target_aligned, offset;

	/* check for target page alignment */
	pgsz = sysconf(_SC_PAGESIZE);
	offset = target & (pgsz - 1);
	target_aligned = target & (~(pgsz - 1));

	DEBUG_PRINT("device: %s, address: 0x%lx (0x%lx+0x%lx), access %s.\n",
		device, target, target_aligned, offset, "read");

	if ((fd = open(device, O_RDWR | O_SYNC)) == -1) {
		DEBUG_PRINT("character device %s opened failed: %s.\n",
			device, strerror(errno));
		return -errno;
	}
	DEBUG_PRINT("character device %s opened.\n", device);

	map = mmap(NULL, offset + 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
		       	target_aligned);
	if (map == (void *)-1) {
		DEBUG_PRINT("Memory 0x%lx mapped failed: %s.\n",
			target, strerror(errno));
		err = 1;
		goto close;
	}
	DEBUG_PRINT("Memory 0x%lx mapped at address %p.\n", target_aligned, map);

	map += offset;
	/* read only */
    *rd_data = *((uint32_t *) map);
    /* swap 32-bit endianess if host is not little-endian */
    *rd_data = ltohl(*rd_data);
    DEBUG_PRINT
        ("Read 32-bit value at address 0x%lx (%p): 0x%08x\n",
            target, map, (unsigned int)*rd_data);

unmap:
	map -= offset;
	if (munmap(map, offset + 4) == -1) {
		DEBUG_PRINT("Memory 0x%lx mapped failed: %s.\n",
			target, strerror(errno));
	}
close:
	close(fd);

	return err;
}
int reg_write(char *device, uint32_t addr, uint32_t wr_data)
{
	int fd;
	int err = 0;
	void *map;
	off_t target = addr;
	off_t pgsz, target_aligned, offset;

	/* check for target page alignment */
	pgsz = sysconf(_SC_PAGESIZE);
	offset = target & (pgsz - 1);
	target_aligned = target & (~(pgsz - 1));

	DEBUG_PRINT("device: %s, address: 0x%lx (0x%lx+0x%lx), access %s.\n",
		device, target, target_aligned, offset, "read");

	if ((fd = open(device, O_RDWR | O_SYNC)) == -1) {
		DEBUG_PRINT("character device %s opened failed: %s.\n",
			device, strerror(errno));
		return -errno;
	}
	DEBUG_PRINT("character device %s opened.\n", device);

	map = mmap(NULL, offset + 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
		       	target_aligned);
	if (map == (void *)-1) {
		DEBUG_PRINT("Memory 0x%lx mapped failed: %s.\n",
			target, strerror(errno));
		err = 1;
		goto close;
	}
	DEBUG_PRINT("Memory 0x%lx mapped at address %p.\n", target_aligned, map);

	map += offset;

    DEBUG_PRINT("Write 32-bits value 0x%08x to 0x%lx (0x%p)\n",
            (unsigned int)wr_data, target, map);
    /* swap 32-bit endianess if host is not little-endian */
    wr_data = htoll(wr_data);
    *((uint32_t *) map) = wr_data;

unmap:
	map -= offset;
	if (munmap(map, offset + 4) == -1) {
		DEBUG_PRINT("Memory 0x%lx mapped failed: %s.\n",
			target, strerror(errno));
	}
close:
	close(fd);

	return err;
}
