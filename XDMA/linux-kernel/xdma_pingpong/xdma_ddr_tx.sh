#!/bin/bash
display_help() {
	echo "$0 <h2c #>"
	echo
       
	exit 1
}

if [ $# -eq 0 ]; then
	display_help
fi

# 512MB
transferSz=0x1FE00000
# repeat time
transferCount=1


tool_path=../tools

tx_idx=$1

addrOffset=$((0x200000000*tx_idx))
echo "addr $addrOffset"

$tool_path/dma_to_device -d /dev/xdma0_h2c_0\
	-s $transferSz -f data/tx${tx_idx}_data_510MB.bin\
	-a $addrOffset -c $transferCount

# Wait for the last transaction to complete.
wait

exit 0
