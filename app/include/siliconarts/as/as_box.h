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
// DATE    : 8/27/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#pragma once
#include "driver/RCStructures.h"

// bounding box structure
typedef struct BOUNDING_BOX{
    DWORD			rootNodeIndex;		// root node id of bounding box
    RCVECTOR3f		minimum;			// minimum coordinate of scene [x, y, z]
    RCVECTOR3f		maximum;			// maximum coordinate of scene [x, y, z]    
}BOUNDING_BOX;

typedef struct BOUNDING_BOX24{
    DWORD			rootNodeIndex;		// root node id of bounding box
    RCVECTOR3i		minimum;			// minimum coordinate of scene [x, y, z]
    RCVECTOR3i		maximum;			// maximum coordinate of scene [x, y, z]    
}BOUNDING_BOX24;
