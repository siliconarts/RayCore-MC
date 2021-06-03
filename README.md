## Introduction


## Requirement
- OS: Ubuntu 18.04.5
- Compiler: GCC 8.4.0
- Hardware device: Intel® Arria® 10 FPGA or Intel® Arria® 10 PAC

## Build
1. Clone the repository.
2. Install the X11 library.
3. Make a directory named linux_x64 in the app/lib directory
4. Run the build.sh script.
```
git clone https://github.com/siliconarts/RCMC.git
mkdir ./app/lib/linux_x64
sh build.sh
```

## Usage
1. Follow the instruction in the ".xxx.pdf" file in the "doc" directory to set up the FPGA hardware.
2. Download [scene.zip](https://drive.google.com/file/d/1T0YQFlnPm5GfwsBhCuofElV-MbpZj3ZF/view?usp=sharing), and uncompress the zip file to the "bin" directory.
2. Run demos		
	```
	# At the root of the repository
	cd bin
	./rc_museum
	```	
	
3. If you want to create your own samples, you can refer to other documents in the "doc" directory.

## License
See [License](License)