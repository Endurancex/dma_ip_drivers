#!/bin/bash
# 512MB
transferSz=0x1FE00000
# repeat time
transferCount=1

tool_path=../tools


tx0_addrOffset=0x000000000
$tool_path/dma_to_device -d /dev/xdma0_h2c_0\
	-s $transferSz -f data/tx0_data_510MB.bin\
	-a $addrOffset -c $transferCount

wait

sleep 1

rx0_addrOffset=0x400000000
$tool_path/dma_from_device -d /dev/xdma0_c2h_0 \
	-s $transferSz -f data/rx0_data_510MB.bin\
	-a $addrOffset -c $transferCount


wait

sleep 1

tx1_addrOffset=0x200000000
$tool_path/dma_to_device -d /dev/xdma0_h2c_0\
	-s $transferSz -f data/tx1_data_510MB.bin\
	-a $addrOffset -c $transferCount

wait

sleep 1

rx1_addrOffset=0x600000000
$tool_path/dma_from_device -d /dev/xdma0_c2h_0 \
	-s $transferSz -f data/rx1_data_510MB.bin\
	-a $addrOffset -c $transferCount

wait

exit 0
