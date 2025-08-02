#!/bin/bash

# 512MB
transferSz=0x1FE00000
# repeat time
transferCount=1


tool_path=../tools


# Write to all enabled h2cChannels in parallel
addrOffset=0x000000000
#addrOffset=0x200000000

$tool_path/dma_to_device -d /dev/xdma0_h2c_0\
	-s $transferSz -f data/tx_data_510MB.bin\
	-a $addrOffset -c $transferCount &

# Wait for the last transaction to complete.
wait

exit 0
