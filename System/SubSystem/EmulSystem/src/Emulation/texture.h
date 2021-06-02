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
// Project : Emulation
// DATE    : 8/27/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef TEXTURE_H
#define TEXTURE_H
#define MAX_TEXTURE			1000

typedef struct _texture{
	int height;
	int width;
	int colorbit;	
	unsigned int* colordata32; 
}_texture;

typedef struct _texture_index{
	int total_texture_num;
	int sizeX[MAX_TEXTURE];
	int sizeY[MAX_TEXTURE];
	int address[MAX_TEXTURE];	
}_texture_index;

#endif //TEXTURE_H