//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name			:  MDINTYPE.H
// Description			:  This file contains typedefine for the driver files	
// Ref. Docment			: 
// Revision History 	:

#ifndef		__MDINTYPE_H__	
#define		__MDINTYPE_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------
typedef char				CHAR, *PCHAR;		// signed 8bit
typedef short				SHORT, *PSHORT;		// signed 16bit
typedef long				LONG, *PLONG;		// signed 32bit

typedef unsigned char		BYTE, *PBYTE;		// unsigned 8bit
typedef unsigned short		WORD, *PWORD;		// unsigned 16bit
typedef unsigned long		DWORD, *PDWORD;		// unsigned 32bit
typedef	float				FLOAT, *PFLOAT;		// floating number

//typedef	bit					BIT;				// 1-bit

#define	ROMDATA				code				// program(read-only) memory

#define	ON					1
#define	OFF					0

#define	HIGH				1
#define	LOW					0

#define	NACK				1
#define	ACK					0

//#define SYSTEM_USE_MDIN220			// for use MDIN220
#define SYSTEM_USE_MDIN240			// for use MDIN240
//#define SYSTEM_USE_MDIN245			// for use MDIN245

#define SYSTEM_USE_HDMICEC	 0		// "1" for CEC, "0" for no-CEC of MDIN240
#define SYSTEM_USE_HDMIHDCP	 0		// "1" for HDCP, "0" for no-HDCP of MDIN240

#ifdef SYSTEM_USE_MDIN240
#define	SUPPROT_HDMI_SDINTR	 1		// "1" for 480i/576i format of HDMI-OUT
#endif

#define	I2C_MDINAPI_ADDR	0xDC	// UpScaler slave address
//#ifdef SYSTEM_USE_MDIN245
#define	I2C_MDVI1ST_ADDR	0x72	// DVI-1st slave address
#define	I2C_MDVI2ND_ADDR	0x7A	// DVI-2nd slave address
//#endif

#define	CPU_ALIGN_BIG_ENDIAN	0	// "1" for big-endian, "0" for little-endian

#define DIFF(a,b)			((a)>(b)? (a-b):(b-a))

#define MIN(a,b)			((a)>(b)? (b):(a))
#define MAX(a,b)			((a)>(b)? (a):(b))

#define HI4BIT(a)			((BYTE)((a)>>4))
#define LO4BIT(a)			((BYTE)((a)&0x0f))

#define CLIP12(a)			((WORD)((((a)>2047)? 2047:((a)<-2048)? -2048:(a))))


#endif	/* __MDINTYPE_H__ */
