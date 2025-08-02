#!/bin/bash

# 512MB
transferSz=0x1FE00000
# repeat time
transferCount=1


tool_path=../tools


addrOffset=0x400000000
#addrOffset=0x600000000

$tool_path/dma_from_device -d /dev/xdma0_c2h_0 \
	-s $transferSz -f data/rx_data_512MB.bin\
	-a $addrOffset -c $transferCount &

# Wait for the last transaction to complete.
wait

exit 0
