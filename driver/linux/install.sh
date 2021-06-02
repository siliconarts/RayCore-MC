#!/bin/sh

chmod 755 update_driver.sh
chmod 755 register_driver.sh
./update_driver.sh

if [ `getconf LONG_BIT` = "64" ]
then
echo "ARCH: 64bit"
	cd ../../app/lib/linux_x64 
else
echo "ARCH: 32bit"
	cd ../../app/lib/linux_x86 
fi

sudo rm /usr/lib/libRayCoreAPI.so
sudo rm /usr/lib/libRCDDIArriaV.so
sudo rm /usr/lib/libRCDDK.so
sudo rm /usr/lib/libRCHAL.so

sudo ln -sf $(pwd)/libRayCoreAPI.so /usr/lib/libRayCoreAPI.so
sudo ln -sf $(pwd)/libRCDDIArriaV.so /usr/lib/libRCDDIArriaV.so
sudo ln -sf $(pwd)/libRCDDK.so /usr/lib/libRCDDK.so
sudo ln -sf $(pwd)/libRCHAL.so /usr/lib/libRCHAL.so

cd ../../../driver/linux

echo RayCore library, Application were installed!
