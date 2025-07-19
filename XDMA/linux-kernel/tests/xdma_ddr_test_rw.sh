#!/bin/bash
display_help() {
	echo "$0 <io size> <io count>"
	echo
       
	exit 1
}

if [ $# -eq 0 ]; then
	display_help
fi

transferSz=$1
transferCount=$2


tool_path=../tools


# Write to all enabled h2cChannels in parallel

addrOffset=0x200000000

$tool_path/dma_to_device -d /dev/xdma0_h2c_0\
	-s $transferSz \
	-a $addrOffset -c $transferCount &

addrOffset=0x600000000

$tool_path/dma_from_device -d /dev/xdma0_c2h_0 \
	-s $transferSz \
	-a $addrOffset -c $transferCount &

# Wait for the last transaction to complete.
wait

exit 0
