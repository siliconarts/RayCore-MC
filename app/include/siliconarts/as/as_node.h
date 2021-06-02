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
// Project : RayCore
// DATE    : 5/28/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#pragma once
#include "STDInterface.h"
// node structure

/************************************************************************/
/* external include data structure */
/************************************************************************/

typedef struct NODE{
    union{
        float		split;						// split at inner node
        int			primimiveNumber;			// number of primitive at leaf node
    };

    union{
        unsigned int     flag;
        unsigned int     rightChild;			// at inner node
        unsigned int     listIndex;				// at leaf node
    }; 
}NODE;

typedef struct NODE_BUFFER{
    unsigned int	buffer[2];
} NODE_BUFFER;

typedef enum{
	RC_NODE_AXIS_X,
	RC_NODE_AXIS_Y,
	RC_NODE_AXIS_Z,
	RC_NODE_AXIS_TERMINAL,
}RC_NODE_AXIS;

typedef union{
	DWORD		data[2];

	struct{
		DWORD		number		: 24;
		DWORD		index_l		: 8;
		DWORD		index_h		: 16;
		DWORD		axis		: 3;
		DWORD		/*dummy*/	: 0;
	};
}DDK_NODE;

/************************************************************************/
/* DDK                                                                  */
/************************************************************************/

// GET NODE DATA
#define GET_NODE_BUFFER_AXIS(name, out)            out = (name.buffer[1] >> 16) & 0x7
#define GET_NODE_BUFFER_SPLIT(name, out)           out = f24f32(name.buffer[0] & 0x00ffffff)
#define GET_NODE_BUFFER_PRIMITIVENUMBER(name, out) out = (name.buffer[0] & 0x00ffffff)
#define GET_NODE_BUFFER_RIGHTCHILD(name, out)      out = ((name.buffer[0] >> 24) | (name.buffer[1] << 8)) & 0x00ffffff
#define GET_NODE_BUFFER_LISTINDEX(name, out)       GET_NODE_BUFFER_RIGHTCHILD(name, out)

#define SET_NODE_AXIS(name, value)  \
    name.listIndex = ((0x000000007 & value) << 24) | (name.listIndex & 0x00ffffff)
#define SET_NODE_LISTINDEX(name, value)  \
    name.listIndex = (name.listIndex & 0x07000000) | (value & 0x00ffffff)
#define SET_NODE_RIGHTCHILD(name, value)  SET_NODE_LISTINDEX(name, value)
    

#define GET_NODE_AXIS(name)    \
    ((name.listIndex >> 24) & 0x7)

#define GET_NODE_RIGHTCHILD(name)    \
    (name.listIndex & 0x00ffffff)
