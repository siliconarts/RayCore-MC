// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore Driver v1.0
// Date : 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "driver_raycore.h"

int main(void)
{
	RC_DRIVER_INFO		device_info;
	int					fd;

	// open driver
	fd	= open(DEV_PATH,  O_RDWR);

	if(fd<0) {
		printf("Device driver was not found!\n");
		return 0;
	}

	{	// check device information
		if(ioctl(fd, IOCTL_RAYCORE_INFOMATION, &device_info)){
			printf("Invalid I/O control operation...\n");
			return 0;
		}
		printf("\n-Device information.\n");
		printf("\tVersion		: 0x%X\n", device_info.version);
		printf("\tCard slots	: %d\n", device_info.card_count);
		//printf("\tCard id	: %d\n", device_info.card_id[1]);

		if(!device_info.card_count){
			printf("There is no RayCore card installation!!!\n\n");
			return 0;
		}
	}
	{	// register read/write test
		RC_TRANSACTION	trans;
		unsigned int	reg[4];	// minimum 2 dword
		memset(&trans, 0, sizeof(RC_TRANSACTION));
		printf("\n\nRegister read/write test...\n");

		// read test
		trans.write_enable	= 0;
		trans.offset		= 0x00000000;			// offset only, not use address
		trans.byte_size		= sizeof(int) * 4;
		trans.pData			= reg;
		ioctl(fd, IOCTL_RAYCORE_TRANSACTION_REG, &trans);

		printf("reg[0] = 0x%X\n", reg[0]);
		printf("reg[1] = 0x%X\n", reg[1]);
		printf("reg[2] = 0x%X\n", reg[2]);
		printf("reg[3] = 0x%X\n", reg[3]);


		// write test
		trans.write_enable	= 1;
//		trans.offset		= 0x00000010;			// offset only, not use address
		trans.offset		= 0x00000020;			// offset only, not use address
		trans.byte_size		= sizeof(int) * 4;
		trans.pData			= reg;
		reg[0]	= 0x1004;
		reg[1]	= 0x4001;
		reg[2]	= 0x4002;
		reg[3]	= 0x4003;
		if (ioctl(fd, IOCTL_RAYCORE_TRANSACTION_REG, &trans)) {
			printf("Invalid I/O control operation...\n");
			return 0;
		}

		reg[0]	= 0x05;
		reg[1]	= 0x05;
		reg[2]	= 0x05;
		reg[3]	= 0x05;

		// read test
		trans.write_enable	= 0;
		ioctl(fd, IOCTL_RAYCORE_TRANSACTION_REG, &trans);

		printf("reg[0] = 0x%X\n", reg[0]);
		printf("reg[1] = 0x%X\n", reg[1]);
		printf("reg[2] = 0x%X\n", reg[2]);
		printf("reg[3] = 0x%X\n", reg[3]);
	}

	{	// memory read/write test
		int i;
		RC_TRANSACTION	trans;
		unsigned int	mem[32];	// minimum 16 dword
		memset(&trans, 0, sizeof(RC_TRANSACTION));
		printf("\n\nMemory read/write test...\n");


		// read test
		trans.write_enable	= 0;
		trans.offset		= 0;
		trans.phy_address	= 0x00FCC980;
		ioctl(fd, IOCTL_RAYCORE_TRANSACTION_MEM, &trans);
		for(i=0;i<32;i++) printf("mem[%d] = 0x%X\n", i, mem[i]);		

		// write test
		trans.write_enable	= 1;
		trans.phy_address	= 0x00010000;
		trans.offset		= sizeof(unsigned int) * 4;
		trans.byte_size		= sizeof(int) * 32;
		trans.pData			= mem;
		for(i=0;i<32;i++) mem[i] = i | 0x12340000;
		ioctl(fd, IOCTL_RAYCORE_TRANSACTION_MEM, &trans);

		for(i=0;i<32;i++) mem[i] = 0;

		// read test
		trans.write_enable	= 0;
		trans.offset		= 0;
		ioctl(fd, IOCTL_RAYCORE_TRANSACTION_MEM, &trans);
		for(i=0;i<32;i++) printf("mem[%d] = 0x%X\n", i, mem[i]);
	}

	{	// dma allocation/free
		
		DMA_MEMORY_USER		dma;
		unsigned int*		pVir;
		dma.byte_size		= 1024*1024;		// \C3ִ\EB 4MB \B4\DC\C0\A7.

		printf("\n\nDMA allocation test...\n");

		if(ioctl(fd, IOCTL_RAYCORE_DMA_ALLOC, &dma)){		// dma allocation
			printf("DMA allocation is failed!\n");
			return 0;
		}

		printf("DMA allocation is succeed!!\n");

		pVir = (unsigned int*)mmap(0, dma.byte_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, dma.phy_addr);
		if(!pVir) {
			printf("mmap is failed!\n");
			return 0;
		}

		{	// dma read test
			int i;
			RC_TRANSACTION	trans;
			memset(&trans, 0, sizeof(RC_TRANSACTION));
			printf("\n\nDMA read test...\n");

			for(i=0;i<32;i++) pVir[i]	= i;
			// write test
			trans.write_enable	= 0;
			trans.phy_address	= 0x00000000;
			trans.offset		= sizeof(int) * 16;
			trans.byte_size		= sizeof(int) * 16;
			trans.pData			= dma.kernel_dma;

			ioctl(fd, IOCTL_RAYCORE_TRANSACTION_DMA, &trans);

			for(i=0;i<32;i++) printf("dma[%d] = 0x%X\n", i, pVir[i]);
		}

		munmap((void*)pVir, dma.byte_size);

		ioctl(fd, IOCTL_RAYCORE_DMA_FREE, &dma);	// dma free
	}

	close(fd);	// close driver

	printf("\nDriver test is done.\n\n");

	return 0;
}
