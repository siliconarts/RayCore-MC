#!/bin/sh

make

./register_driver.sh

if [ -c /dev/raycore_pcie ]; then
	sudo rmmod -f raycore_pcie
	echo removed!!!
fi

sudo insmod  raycore_pcie.ko
lsmod | grep raycore
