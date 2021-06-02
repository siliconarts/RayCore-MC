//=======================================================================
//
// This confidential and proprietary software may be used only as
// authorized by a licensing agreement from Siliconarts Inc.
//
//   (C) Copyright 2010 ~ 2013  Siliconarts Inc. ALL RIGHTS RESERVED
//
// The entire notice above must be reproduced on all authorized
// copies and copies may only be made to the extent permitted
// by a licensing agreement from Siliconarts.
//
// Project : Linux driver
// DATE    : 1/21/2014 Tue
// Author  : ay
//
//=======================================================================

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/pci.h>
#include <linux/pci_regs.h>	// PCI_SUBSYSTEM_VENDOR_ID
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mm.h>		//mmap, remap_pfn_range
#include <linux/printk.h>
#include <asm/types.h>

#include <linux/string.h>  //using memcpy

#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <asm/io.h>			// consistent_alloc, consistent_free

#include "driver.h"

#define DRIVER_NAME			DEV_NAME  "_drv"				// device name
static const char*			__device_name	= DRIVER_NAME;	// device name (const data)
static RC_DRIVER			g_Driver;						// global driver struct
/*
static BLOCK_RW_FIFO		blk;
static BLOCK_RW_FIFO*		blk_rw;
*/
/****************************************************************************
*								driver function								*
****************************************************************************/
static void*		__knl_malloc   (unsigned int size)                        { return kmalloc(size, GFP_KERNEL); }
static void			__knl_free     (void * ptr)                               { kfree(ptr); }
//static unsigned int	__knl_ioread32 (unsigned int base, unsigned int addr )                       { return ioread32((void __iomem *)(base+addr)); }
//static void			__knl_iowrite32(unsigned int base, unsigned int addr,unsigned int data )     { iowrite32((u32)data,(void __iomem *)(base+addr)); }
//static unsigned int	__knl_copy_from_user( void * to, const void __user * from, unsigned long n ) { return (unsigned int)copy_from_user (to, from, n); }
//static unsigned int	__knl_copy_to_user( void __user * to, const void * from, unsigned long n ) { return (unsigned int)copy_to_user (to, from, n); }


//set 64bit read/write
static inline u64 __ioread64(const volatile void __iomem *addr)
{
		return *(const volatile u64 __force *) addr;
}
//#define readq(addr) __le64_to_cpu(__ioread64(addr))

static inline void __iowrite64(u64 b, volatile void __iomem *addr)
{
		*(volatile u64 __force *) addr = b;
}
//#define writeq(b,addr) __iowrite64(__cpu_to_le64(b),addr)

static long ioctl_device_information(RC_CONTEXT* pContext, unsigned long arg){
	if(!arg) return -ENOMEM;
	copy_to_user((void*)arg, &g_Driver.info, sizeof(RC_DRIVER_INFO));
	return 0;
}

static long ioctl_dma_alloc(RC_CONTEXT* pContext, unsigned long arg){
	DMA_MEMORY*			pMem = (DMA_MEMORY*)__knl_malloc(sizeof(DMA_MEMORY));
	
	if(!pMem || copy_from_user((void *)&pMem->user, (const void*)arg, sizeof(DMA_MEMORY_USER))) return 0;

	{	// \C7Ҵ\E7
		RC_DEVICE* pDevice	= g_Driver.pDevList[0];
		// usr->byte_size \B8\B8ŭ \C7Ҵ\E7 \C7ؼ\AD phy, vir ä\BF\EE\B4\D9.
		pMem->vir_kernel	= pci_alloc_consistent(pDevice->pDev, pMem->user.byte_size, &pMem->hDMA);
		if(!pMem->vir_kernel) {
			//LOGI("dma page addr is NULL\n");
			__knl_free(pMem);
			return -ENOMEM;
		}
		pMem->user.phy_addr		= virt_to_phys(pMem->vir_kernel);
		pMem->pDevice			= pDevice;
		pMem->user.kernel_dma	= pMem;
	}

	if(copy_to_user((void*)arg, &pMem->user, sizeof(DMA_MEMORY_USER))) return 0;
	// link
	pMem->pNext				= pContext->m_pDMAHead;
	pContext->m_pDMAHead	= pMem;

	return 0;
}

static long ioctl_dma_free(RC_CONTEXT* pContext, unsigned long arg){
	DMA_MEMORY_USER		user;
	if(copy_from_user((void *)&user, (const void*)arg, sizeof(DMA_MEMORY_USER))) return -EINVAL;

	{
		DMA_MEMORY*		pMem	= (DMA_MEMORY*)(user.kernel_dma);
		DMA_MEMORY*		pNode	= pContext->m_pDMAHead;

		if(pNode == pMem){ // head \B8\A6 \C1\F6\BF\EF \B6\A7...
			pci_free_consistent(pMem->pDevice->pDev, user.byte_size, pMem->vir_kernel, pMem->hDMA);
			pContext->m_pDMAHead	= pMem->pNext;
			__knl_free(pMem);
		}else{
			while(pNode){	// \C1߰\A3\C0\BB \C1\F6\BF\EF \B6\A7...
				if(pNode->pNext == pMem){
					pci_free_consistent(pMem->pDevice->pDev, user.byte_size, pMem->vir_kernel, pMem->hDMA);
					pNode->pNext = pMem->pNext;
					__knl_free(pMem);
					break;
				}
				pNode = pNode->pNext;
			}
		}
	}
	return 0;
}

static long ioctl_transaction_reg(RC_CONTEXT* pContext, unsigned long arg){
	RC_TRANSACTION	transaction;
	if(copy_from_user((void *)&transaction, (const void*)arg, sizeof(RC_TRANSACTION))) return -ENOMEM;
	if(transaction.device_id >= g_Driver.info.card_count) return -ENODEV;
	if(transaction.phy_address) return -ENOMEM;	// address must be zero
	{
		unsigned int	iCount	= (transaction.byte_size >> 2);
		unsigned int	cmd		= (1<<31)|(transaction.write_enable<<30)|(iCount<<23)|(transaction.offset>>2);
		u64*			pTemp	= (u64*)g_Driver.pTemp;
		PCI_BAR*		pBar	= &g_Driver.pDevList[transaction.device_id]->bar0;
		int i;

		iowrite32((__u32)cmd, (void __iomem *)(pBar->pData + 0x2000));
		//LOGI("CMD = %X\n", cmd);

		//if(iCount&1) LOGI("*E reg error\n");

		iCount	>>= 1;
		if(transaction.write_enable){	// write
			copy_from_user(pTemp, transaction.pData, transaction.byte_size);
			for(i=0;i<iCount;i++)
				__iowrite64(pTemp[i], pBar->pData);	// no increase (fifo type)
		}else{	// read
			for(i=0;i<iCount;i++)
				pTemp[i]	= __ioread64(pBar->pData + i*sizeof(u64));

			copy_to_user(transaction.pData, pTemp, transaction.byte_size);
		}
	}
	return 0;
}

static long ioctl_transaction_mem(RC_CONTEXT* pContext, unsigned long arg){
	RC_TRANSACTION	transaction;
	if(copy_from_user((void *)&transaction, (const void*)arg, sizeof(RC_TRANSACTION))) return -ENOMEM;

	if(transaction.device_id >= g_Driver.info.card_count) return -ENODEV;
	{
		unsigned int	iCount	= (transaction.byte_size >> (4+2));
		//unsigned int	cmd		= /*(0<<31)|*/(transaction.write_enable<<30)|(16<<23)|(transaction.phy_address>>(4+2));
		//for 8G Memory
		unsigned int	cmd		= /*(0<<31)|*/(transaction.write_enable<<30)|(16<<23)|(transaction.phy_address);
		u64*			pUser	= transaction.pData;
		u64*			pTemp	= (u64*)g_Driver.pTemp;
		PCI_BAR*		pBar	= &g_Driver.pDevList[transaction.device_id]->bar0;
		
		int i,t;

		if(transaction.write_enable){	// write
			for(i=0;i<iCount;i++){
				iowrite32((__u32)cmd, (void __iomem *)(pBar->pData + 0x2000));cmd++;
				
				copy_from_user(pTemp, pUser, sizeof(unsigned int)*16);pUser += 8;
				for(t=0;t<8;t++)
					__iowrite64(pTemp[t], pBar->pData);	// no increase (fifo type)
			}
		}else{	// read
			for(i=0;i<iCount;i++){
				iowrite32((__u32)cmd, (void __iomem *)(pBar->pData + 0x2000));cmd++;

				for(t=0;t<8;t++)
					pTemp[t]	= __ioread64(pBar->pData + t*sizeof(u64));
				copy_to_user(pUser, pTemp, sizeof(unsigned int)*16);pUser += 8;
			}
		}
	}

	return 0;
}

static long ioctl_transaction_dma(RC_CONTEXT* pContext, unsigned long arg){
	RC_TRANSACTION	transaction;
	if(copy_from_user((void *)&transaction, (const void*)arg, sizeof(RC_TRANSACTION))) return -ENOMEM;

	if(transaction.device_id >= g_Driver.info.card_count) return -ENODEV;
	if(transaction.write_enable) return -ENOMEM;	// write operation is not allowed.
	// do something...
	{
		DMA_MEMORY* pDMA	= (DMA_MEMORY*)(transaction.pData);
		RC_DEVICE*	pDevice	= g_Driver.pDevList[transaction.device_id];
		union{
			__u64			addr;
			struct{
				unsigned int	low, hi;
			};
		}page;

		if(transaction.byte_size){
			{// check byte alignment
				if((transaction.offset&(8-1)) || (transaction.phy_address&(31-1))) return -EFAULT;
			}

			{// ATT(address translation table) setting
				PCI_BAR*	pBar	= &pDevice->bar4;
				page.addr	= pDMA->user.phy_addr;


				{
					int i;
					unsigned int	table_size = (transaction.offset + transaction.byte_size + 1024*1024 + (page.low & 0xFFFFF)) / (1024*1024);
					for(i=0;i<table_size;i++){

						
						iowrite32((__u32)(page.low | (page.hi ? 1 : 0)), (void __iomem *)(pBar->pData + 0x1000 + i*8) );	// low address, check 64bit prefetch												
						iowrite32((__u32)page.hi, (void __iomem *)(pBar->pData + 0x1000 + i*8 + 4));						// high address


						page.addr += 1024*1024;
					}
				}
			}

			{// transfer
				PCI_BAR*		pBar	= &pDevice->bar0;
				unsigned int	cmd		= (1<<31)|(1<<30)|(4<<23)|(0x10000);
				unsigned int	reg[4];
				page.addr	= pDMA->user.phy_addr;
				reg[0]	= (page.low & 0xFFFFF) + transaction.offset;	// CRA start address
				//reg[1]	= transaction.phy_address >> 6;						// ddr address (32 byte(128bit) align)
				//for 8G Memory
				reg[1]	= transaction.phy_address;						// ddr address (32 byte(128bit) align)
				//reg[2]	= transaction.byte_size;		
				reg[2]		= (1<<24)|transaction.byte_size;
				// size

				reg[3]	= 1;	


				// start
				iowrite32((__u32)cmd, (void __iomem *)(pBar->pData + 0x2000));

				__iowrite64(((__u64*)reg)[0], pBar->pData);	// no increase (fifo type)
				__iowrite64(((__u64*)reg)[1], pBar->pData);	// no increase (fifo type)
			}
		}else{	// wait DMA
			PCI_BAR*		pBar	= &pDevice->bar0;
			unsigned int	cmd;
			unsigned int	reg[4];
			// wait done.
			for(;;){
				
				cmd = (1<<31)|(2<<23)|(0x10000);
				iowrite32((__u32)cmd, (void __iomem *)(pBar->pData + 0x2000));

				((__u64*)reg)[0]	= __ioread64(pBar->pData);

				cmd = (1<<31)|(2<<23)|(0x10002);
				iowrite32((__u32)cmd, (void __iomem *)(pBar->pData + 0x2000));

                                ((__u64*)reg)[1]        = __ioread64(pBar->pData);

				if(reg[3] & 2) break;

			}
				
			

			// sync memory
			//wmb();	// memory barrier
			//pci_dma_sync_single_for_cpu(pDMA->pDevice->pDev, pDMA->hDMA, pDMA->user.byte_size, DMA_FROM_DEVICE);
			pci_dma_sync_single_for_cpu(pDevice->pDev, pDMA->hDMA, pDMA->user.byte_size, DMA_FROM_DEVICE);
			//pci_dma_sync_single_for_device 
		}
	}
	return 0;
}

typedef long (*IOCTL_FUNC)(RC_CONTEXT* pContext, unsigned long arg);

IOCTL_FUNC	g_ioctl_functions[6]	= {
	ioctl_device_information,
	ioctl_dma_alloc,
	ioctl_dma_free,
	ioctl_transaction_reg,
	ioctl_transaction_mem,
	ioctl_transaction_dma,
};

#ifdef KERNEL_VERSION_BELOW_2_6_35
static int __driver_ioctl(struct inode *pinode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
static long __driver_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
	RC_CONTEXT*	pContext	= (RC_CONTEXT*)filp->private_data;
	if(cmd<IOCTL_RAYCORE_INFOMATION && cmd >IOCTL_RAYCORE_DMA_FREE) return -1;
	cmd -= IOCTL_RAYCORE_INFOMATION;
	return g_ioctl_functions[cmd](pContext, arg);
}

/****************************************************************************
*								file operation								*
****************************************************************************/
static int __driver_open(struct inode *inode, struct file *filp) 
{
	if(filp->f_flags & O_SYNC) return -ENOMEM;

	if(!(filp->private_data = (void *)__drv_create_context())){
		//LOGI("context creation is failed!\n");
		return -ENOMEM;
	}

	return 0;
}

static int __driver_release (struct inode *inode, struct file *filp)
{
	if(filp->f_flags & O_SYNC) return -ENOMEM;
	__drv_destroy_context((RC_CONTEXT *)filp->private_data);

	return 0;
}


static int __driver_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long size      = vma->vm_end - vma->vm_start;

#ifdef KERNEL_VERSION_BELOW_3_6_0
	vma->vm_flags |= (VM_IO | VM_RESERVED);
#else
	//vma->vm_flags |= (VM_IO | VM_LOCKED | (VM_DONTEXPAND | VM_DONTDUMP));
	vma->vm_flags |= (VM_IO | VM_LOCKED | (VM_DONTEXPAND));
#endif
	if(remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, size, vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

/****************************************************************************
*						driver extension function							*
****************************************************************************/
static int __pci_bar_initialize(struct pci_dev *dev, PCI_BAR* pBar, int bar_num){
	// get I/O information
	pBar->addr		= pci_resource_start	(dev, bar_num);
	pBar->length	= pci_resource_len		(dev, bar_num);
	pBar->flag		= pci_resource_flags	(dev, bar_num);
	if(!pBar->addr || !pBar->length){
		//LOGI("	Can't found bar #%d information.\n", bar_num);
		return -ENOMEM;
	}
	//LOGI("	Bar #%d : ADDR(0x%lx), LENGTH(0x%lx)\n", bar_num, (unsigned long)pBar->addr, (unsigned long)pBar->length);
	// request memory region
	if(!request_mem_region(pBar->addr, pBar->length, __device_name)) return -ENOMEM;
	// I/O remapping
	if(pBar->flag & IORESOURCE_CACHEABLE)
		pBar->pData = ioremap(pBar->addr, pBar->length);
	else
		pBar->pData = ioremap_nocache(pBar->addr, pBar->length);

	if(!pBar->pData) return -ENOMEM;

	return 0;
}

static int __probe(struct pci_dev *dev, const struct pci_device_id *id){
	{	// check device validation
		if(pci_enable_device(dev)<0){
			//LOGI("Invalid device :  %s\n", pci_name(dev));
			return -EIO;
		}
		pci_set_master(dev);
	}

	LOGI("Device(%d) is proved. : %s\n", g_Driver.info.card_count, pci_name(dev));

	{	// add device link
		RC_DEVICE* pDevice		= (RC_DEVICE*)__knl_malloc(sizeof(RC_DEVICE));
		memset((void*)pDevice, 0, sizeof(RC_DEVICE));
		pDevice->pDev			= dev;
		g_Driver.pDevList[g_Driver.info.card_count]	= pDevice;
		dev->sysdata			= (void*)pDevice;


		// read card ID
		{
			union{
				unsigned int		ID;
				struct{
					unsigned short		vid;
					unsigned short		did;
				};

			}card_id;
			pci_read_config_word(dev, PCI_SUBSYSTEM_ID, &card_id.did);
			pci_read_config_word(dev, PCI_SUBSYSTEM_VENDOR_ID, &card_id.vid);
			//LOGI("	Sub system ID = 0x%X\n", card_id.ID);
			g_Driver.info.card_id[g_Driver.info.card_count]	= card_id.ID;
		}

		// bar I/O mapping
		if(__pci_bar_initialize(dev, &pDevice->bar0, 0)) return -ENOMEM;
		if(__pci_bar_initialize(dev, &pDevice->bar2, 2)) return -ENOMEM;	
		if(__pci_bar_initialize(dev, &pDevice->bar4, 4)) return -ENOMEM;	
	}

	g_Driver.info.card_count++;

	return 0;
}

static void __remove(struct pci_dev *dev){
	RC_DEVICE* pDevice	= (RC_DEVICE*)dev->sysdata;

	LOGI("Device(%s) will be removed.\n", pci_name(dev));

	{	// release bar information
		if(pDevice->bar0.pData)	iounmap(pDevice->bar0.pData);
		if(pDevice->bar0.addr)	release_mem_region(pDevice->bar0.addr, pDevice->bar0.length);
		if(pDevice->bar2.pData)	iounmap(pDevice->bar2.pData);
		if(pDevice->bar2.addr) release_mem_region(pDevice->bar2.addr, pDevice->bar2.length);
		if(pDevice->bar4.pData)	iounmap(pDevice->bar4.pData);
		if(pDevice->bar4.addr) release_mem_region(pDevice->bar4.addr, pDevice->bar4.length);
	}
	{	// delete from device link chain
		int i;
		for(i=0;i<RC_MAX_DEVICE_COUNT;i++){
			if(g_Driver.pDevList[i] == pDevice){
				__knl_free(pDevice);
				g_Driver.pDevList[i]	= NULL;
				g_Driver.info.card_count--;
				break;
			}
		}
	}
	
	return;
}

/****************************************************************************
*								driver structure							*
****************************************************************************/
// pci id table
static struct pci_device_id __id_table	= {
	RC_VENDOR_ID,		// vender
	RC_DEVICE_ID,		// device
	PCI_ANY_ID,			// subvender
	PCI_ANY_ID,			// subdevice
};

// pci driver table
static struct pci_driver __driver_module = {
	.name		= DRIVER_NAME,
	.id_table	= &__id_table,
	.probe		= __probe,
	.remove		= __remove,
};

// character file driver table
static struct file_operations __driver_fops = {
	.owner			= THIS_MODULE,
#ifdef KERNEL_VERSION_BELOW_2_6_35
	.ioctl		    = __driver_ioctl,
#else
	.unlocked_ioctl	= __driver_ioctl,
#endif
	.mmap			= __driver_mmap,
	.open			= __driver_open,
	.release		= __driver_release,
};

static int __init __module_init(void){
	LOGI("%s\n", __raycore_pcie_VERSION_STRING__);
	// reset devices struct
	memset(&g_Driver, 0, sizeof(RC_DRIVER));
	g_Driver.info.version	= (RC_VENDOR_ID<<16)|RC_DEVICE_ID;
	// alloc. temporary memory
	g_Driver.pTemp			= __knl_malloc(4096);

	// register character driver
	if(register_chrdev(RC_MAJOR_NR, DEV_NAME, &__driver_fops)<0)
		return -1;

	// register pcie module driver
	// refer to http://wiki.kldp.org/wiki.php/PCI%20Drivers
	pci_register_driver(&__driver_module);

	return 0;
}

static void __exit __module_exit(void){
	// unregister pcie module driver
	pci_unregister_driver(&__driver_module);
	// unregister character driver
	unregister_chrdev(RC_MAJOR_NR, DEV_NAME);

	__knl_free(g_Driver.pTemp);

	LOGI("driver was released\n");
	LOGI("------------------------------------------------------------------\n");
}

module_init(__module_init);
module_exit(__module_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("CloneX");
