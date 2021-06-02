// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore Driver v1.0
// Date : 

//+-----------------------------------------------------------------------
//|	including files
//+-----------------------------------------------------------------------
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/slab.h> // kmalloc, kfree
#include <asm/io.h>
#include <asm/atomic.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>

#include "driver.h"

#define VERSION_STRING  MAKE_VERSION_STRING(raycore_drv)

//+-----------------------------------------------------------------------------
//| kernel functions
//+-----------------------------------------------------------------------------
//static void*        __knl_malloc   (unsigned int size)                        { return kmalloc(size, GFP_KERNEL); }
static void*        __knl_zmalloc  (unsigned int size)                        { return kzalloc(size, GFP_KERNEL); }
static void         __knl_free     (void * ptr)                               { kfree(ptr); }
//static void         __knl_memcpy   (void * dest, const void * src, u32 count) { memcpy(dest, src, count);}
//static void         __knl_memset   (void * dest, u32 c, u32 count)            { memset(dest,   c, count);}
static unsigned int __knl_getpid   (void)                                     { return (unsigned int)task_tgid_vnr(current); }
//static unsigned int __knl_gettid   (void)                                     { return (unsigned int)current->pid ; }
//static unsigned int __knl_ioread32 (unsigned int base, unsigned int addr )                       { return ioread32(base+addr); }
//static void         __knl_iowrite32(unsigned int base, unsigned int addr,unsigned int data )     { iowrite32(data,base+addr); }

//+-----------------------------------------------------------------------------
//| local
//+-----------------------------------------------------------------------------
static RC_CONTEXT*	__pContextHead = 0;

//----------------------------------------------------------------------------
// Context
//----------------------------------------------------------------------------
RC_CONTEXT * __drv_create_context (void){
	RC_CONTEXT* pContext = (RC_CONTEXT *)__knl_zmalloc(sizeof(RC_CONTEXT));
	if(pContext)
	{
		pContext->m_pNext	= (void*)__pContextHead;
		__pContextHead		= pContext;
		pContext->m_PID		= __knl_getpid();
		return pContext;
	}
	return 0;
}

static void __drv_cleanup_context(RC_CONTEXT *pContext)
{
	if(!pContext || !pContext->m_PID) return;
	pContext->m_PID = 0;
}

int __drv_destroy_context  (RC_CONTEXT *pContext)
{
	// 자신이 생성한 컨텍스만 제거한다.
	if(!pContext) return -1;
	{
		RC_CONTEXT* pCur;
		__drv_cleanup_context( pContext );
		pCur = __pContextHead;
		if(pCur==pContext){
			__pContextHead		= pCur->m_pNext;
		}else{
			while(pCur){
				if(pCur->m_pNext == pContext){
					pCur->m_pNext	= pContext->m_pNext;
					break;
				}
				pCur = pCur->m_pNext;
			}
		}
		if(pCur){
			__knl_free(pContext);
			return 0;
		}
	}
	return -1;
}

void __drv_close_proc( void )
{
	int closed_pid = __knl_getpid();
	RC_CONTEXT* pEntry = __pContextHead;
	while( pEntry )
	{
		if(pEntry->m_PID==closed_pid)
		{
			__drv_destroy_context( pEntry );
		}
		pEntry = pEntry->m_pNext;
	}	
}

MODULE_LICENSE("Dual BSD/GPL");
