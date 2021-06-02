#!/bin/sh

if [ -c /dev/raycore_pcie ]; then
	echo already driver is registered.
else
	sudo mknod /dev/raycore_pcie c 240 0
	sudo chmod 777 /dev/raycore_pcie
	sudo dmesg -c
fi
