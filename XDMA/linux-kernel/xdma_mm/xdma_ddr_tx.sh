#!/bin/bash
display_help() {
	echo "$0 <Channel Count>"
	echo
       
	exit 1
}

if [ $# -eq 0 ]; then
	display_help
fi

# repeat time
ch=$1
# 512MB
transferSz=0x20000000
# repeat time
transferCount=1
tool_path=../tools
addrOffset=0

for ((i=0; i<$ch; i++)); do
    $tool_path/dma_to_device -d /dev/xdma0_h2c_${i} -s $transferSz -f data/tx_data_512MB.bin -c $transferCount &
done

# Wait for the last transaction to complete.
wait

exit 0
