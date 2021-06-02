#!/bin/sh
cd ./app/linux/toolkit
make clean
make
cd ../../Example/RC1
make clean
make
cd ../../../driver/linux
make clean
make
sh install.sh
