#!/bin/bash
display_help() {
	echo "$0 <io size> <io count> <h2c #> <c2h #>"
	echo
       
	exit 1
}

if [ $# -eq 0 ]; then
	display_help
fi

xid=xdma0
transferSz=$1
transferCount=$2
h2cChannels=$3
c2hChannels=$4

tool_path=../tools

# Run the PCIe DMA memory mapped write read test
echo -e "\ttransfer size:  $transferSz, count: $transferCount"

# Write to all enabled h2cChannels in parallel
if [ $h2cChannels -gt 0 ]; then
	# Loop over four blocks of size $transferSz and write to them
	for ((i=0; i<$h2cChannels; i++)); do
		addrOffset=$((0x200000000 * $i))
		curChannel=$i

		$tool_path/dma_to_device -d /dev/${xid}_h2c_${curChannel} \
			-s $transferSz \
			-a $addrOffset -c $transferCount &
	done
fi

# Wait for the last transaction to complete.
wait

# Read from all enabled c2hChannels in parallel
if [ $c2hChannels -gt 0 ]; then
	# Loop over four blocks of size $transferSz and read from them
	for ((i=0; i<$c2hChannels; i++)); do
		addrOffset=$((0x200000000 * $i + 0x400000000))
		curChannel=$i

		$tool_path/dma_from_device -d /dev/${xid}_c2h_${curChannel} \
		       	-s $transferSz \
		       	-a $addrOffset -c $transferCount &
	done
fi

# Wait for the last transaction to complete.
wait

exit 0
