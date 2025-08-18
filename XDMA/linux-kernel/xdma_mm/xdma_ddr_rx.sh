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


output_file="data/rx_data_512MB.bin"


for ((i=0; i<$ch; i++)); do
    $tool_path/dma_from_device -d /dev/xdma0_c2h_${i} -s $transferSz -f "$output_file" -c $transferCount &
done


# Wait for the last transaction to complete.
wait

input_file="data/tx_data_512MB.bin"

if cmp -s "$output_file" "$input_file"; then
	echo "data matching"
else
	echo "data not matched"
	echo "display the first 10 lines of differences"
	hexdump -C "$output_file" | head -n 10 > data/rx.hex
	hexdump -C "$input_file" | head -n 10 > data/tx.hex
	diff data/rx.hex data/tx.hex
fi
exit 0
