// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore DDK v 1.0
// Date : 

#ifndef __PCIE_MEMORY_H__
#define __PCIE_MEMORY_H__
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "driver/SystemHAL.h"
#include "../../../driver/linux/driver_raycore.h"

#include "NativeInterface.h"

class PCIeMemory :
	public NativeMemory
{
	friend NativeMemory* CreateNativeMemory(DWORD dwByteSize);
	
protected:
	~PCIeMemory(void);

public:
	PCIeMemory(void);

	STDMETHOD_(void, Reads)(unsigned long long int dwPhysical, DWORD dwOffset, DWORD dwByteSize);
	STDMETHOD_(void, Wait)(void);

protected:
	STDMETHOD_(BOOL, Alloc)(DWORD dwByteSize);

private:
	int					m_fd;		// raycore driver
	DMA_MEMORY_USER		m_dma;		// dma memory user info.
	RC_TRANSACTION		m_trans;	// transaction
};

#endif//__PCIE_MEMORY_H__
