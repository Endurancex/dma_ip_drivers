#!/bin/bash
display_help() {
	echo "$0 <Channel Count>"
	echo
       
	exit 1
}

if [ $# -eq 0 ]; then
	display_help
fi


# 2048MB
transferSz=0x80000000
# repeat time
ch=$1
transferCount=1

tool_path=../tools

for ((i=0; i<$ch; i++)); do
    $tool_path/dma_from_device -d /dev/xdma0_c2h_${i} -s $transferSz -f data/c2h_data.bin -c $transferCount &
done


# AXIS Data Gen
packet_len_addr=0x0
packet_len=$((transferSz/32))
statusRegVal=`$tool_path/reg_rw /dev/xdma0_user ${packet_len_addr} w ${packet_len}| grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
statusRegVal=`$tool_path/reg_rw /dev/xdma0_user ${packet_len_addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $packet_len_addr $statusRegVal

start_addr=0x8
statusRegVal=`$tool_path/reg_rw /dev/xdma0_user ${start_addr} w 1| grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
statusRegVal=`$tool_path/reg_rw /dev/xdma0_user ${start_addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $start_addr $statusRegVal
statusRegVal=`$tool_path/reg_rw /dev/xdma0_user ${start_addr} w 0| grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
statusRegVal=`$tool_path/reg_rw /dev/xdma0_user ${start_addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $start_addr $statusRegVal

wait

for ((i=0; i<$ch; i++)); do
    $tool_path/dma_to_device -d /dev/xdma0_h2c_${i} -s $transferSz -f data/c2h_data.bin -c $transferCount &
done
wait

exit 0
