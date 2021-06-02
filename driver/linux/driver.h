// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore Driver v1.0
// Date : 

#ifndef __RAYCORE_H__
#define __RAYCORE_H__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "driver_raycore.h"

// device hardware information
#define RC_VENDOR_ID		0x1172
#define RC_DEVICE_ID		0x1000
#define RC_CLASS_ID			0x0380
#define RC_MAJOR_NR			240

extern const char* __raycore_pcie_VERSION_STRING__;

// PCI bar information
typedef struct PCI_BAR_t {
	unsigned long long		addr, length, flag;
	volatile void __iomem*	pData;
}PCI_BAR;

// PCI device
typedef struct RC_DEVICE_t{
	struct pci_dev			*pDev;
	PCI_BAR					bar0, bar2, bar4;
}RC_DEVICE;

// Driver (global)
typedef struct RC_DRIVER_t {
	RC_DRIVER_INFO			info;
	RC_DEVICE*				pDevList[RC_MAX_DEVICE_COUNT];
	void*					pTemp;
}RC_DRIVER;

// DMA
typedef struct DMA_MEMORY_t {
	DMA_MEMORY_USER			user;
	dma_addr_t				hDMA;
	RC_DEVICE*				pDevice;
	void*					vir_kernel;
	struct DMA_MEMORY_t*	pNext;
}DMA_MEMORY;

// Context
typedef struct RC_CONTEXT_t {
	struct RC_CONTEXT_t*	m_pNext;
	int						m_PID;
	DMA_MEMORY*				m_pDMAHead;
}RC_CONTEXT;

//FUNCTION
int __drv_destroy_context (RC_CONTEXT *pContext);
RC_CONTEXT * __drv_create_context (void);
void __drv_close_proc( void );

#define LOGI(fmt, args...)	printk("[RayCore] " fmt, ## args)
#define LOGE(fmt, args...)	printk("[RayCore *E] " fmt, ## args)

#endif //__RAYCORE_H__
