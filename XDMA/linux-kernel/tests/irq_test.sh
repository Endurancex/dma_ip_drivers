#!/bin/bash


tool_path=../tools



addr=0x3014
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal

# IRQ Block Identifier
addr=0x2000
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
#echo $addr $statusRegVal
#channelId=${statusRegVal:0:3}
#echo $channelId
#irqid=${statusRegVal:3:1}
#echo $irqid
#version=${statusRegVal:5:2}
#echo $version

addr=0x2004
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal

addr=0x2040
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal

addr=0x2048
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal

addr=0x2080
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal

addr=0x8000
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x8004
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x8008
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x800C
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal

addr=0x8010
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x8014
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x8018
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x801C
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal

addr=0x8020
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x8024
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x8028
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
addr=0x802C
statusRegVal=`$tool_path/reg_rw /dev/xdma0_control ${addr} w | grep "Read.*:" | sed 's/Read.*: 0x\([a-z0-9]*\)/\1/'`
echo $addr $statusRegVal
exit 0
