#!/bin/bash
display_help() {
	echo "$0 <c2h #>"
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

rx_idx=$1

addrOffset=$((0x400000000+0x200000000*rx_idx))
echo "addr $addrOffset"

$tool_path/dma_from_device -d /dev/xdma0_c2h_0 \
	-s $transferSz -f data/rx${rx_idx}_data_510MB.bin\
	-a $addrOffset -c $transferCount &

# Wait for the last transaction to complete.
wait

exit 0
