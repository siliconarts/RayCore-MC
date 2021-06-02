// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore Driver v1.0
// Date : 

#ifndef __DRIVER_RAYCORE_H__
#define __DRIVER_RAYCORE_H__

#define DEV_NAME					"raycore_pcie"
#define DEV_PATH					"/dev/raycore_pcie"
#define RC_MAX_DEVICE_COUNT			32

// I/O command list
typedef enum{
	IOCTL_RAYCORE_INFOMATION		= 0x10000,
	IOCTL_RAYCORE_DMA_ALLOC,
	IOCTL_RAYCORE_DMA_FREE,
	IOCTL_RAYCORE_TRANSACTION_REG,
	IOCTL_RAYCORE_TRANSACTION_MEM,
	IOCTL_RAYCORE_TRANSACTION_DMA,
}IOCTL_RAYCORE;

// driver information
typedef struct RC_DRIVER_INFO_t{
	unsigned int	version;
	unsigned int	card_count;
	unsigned int	card_id[RC_MAX_DEVICE_COUNT];
}RC_DRIVER_INFO;

// DMA struct
typedef struct DMA_MEMORY_USER_t {
	unsigned int	byte_size;
	unsigned long	phy_addr;
	void*			kernel_dma;
}DMA_MEMORY_USER;

// transaction struct
typedef struct RC_TRANSACTION_t {
	unsigned short	write_enable;			// write or read
	unsigned short	device_id;				// device ID
	unsigned int	phy_address, offset;	// inner physical address & offset
	unsigned int	byte_size;				// transaction byte size byte size
	void*			pData;					// user virtual pointer or kernel DMA pointer
} RC_TRANSACTION;

#endif //__DRIVER_RAYCORE_H__
