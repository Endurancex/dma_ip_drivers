#!/bin/bash
# 64KB
transferSz=0x10000
# repeat time
transferCount=1

tool_path=../tools

$tool_path/dma_from_device -d /dev/xdma0_c2h_0 -s $transferSz -f data/c2h_data.bin -c $transferCount
wait

$tool_path/dma_to_device -d /dev/xdma0_h2c_0 -s $transferSz -f data/c2h_data.bin -c $transferCount
wait

exit 0
