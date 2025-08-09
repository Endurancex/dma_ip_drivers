#!/bin/bash

display_help() {
	echo "$0 <repeat time>"
	echo
       
	exit 1
}

if [ $# -eq 0 ]; then
	display_help
fi

repeat=$1

echo "Repeat DDR TX/RX test for $repeat times"
for ((i=1; i<=repeat; i++)); do
	echo "=== Iteration: $i ==="
	./xdma_ddr_tx.sh 0
	./xdma_ddr_rx.sh 0
	./xdma_ddr_tx.sh 1
	./xdma_ddr_rx.sh 1
done
