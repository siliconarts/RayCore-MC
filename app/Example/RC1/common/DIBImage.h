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
// Project : Example
// DATE    : 4/10/2013 Wed
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include "STDInterface.h"

typedef struct RGBImageRec {
	int sizeX, sizeY;
	char *data;
} RGBImageRec;

typedef struct bmpBITMAPFILEHEADER{
	WORD   bfType;			//"BM"
	DWORD  bfSize;			//Total Size of Bitmap File
	WORD   bfReserved1;		//Reserved Variable
	WORD   bfReserved2;		//Reserved Variable
	DWORD  bfOffBits;		//Start Position of Bitmap Data in File
}BITMAPFILEHEADER1;

typedef struct bmpBITMAPINFOHEADER{
	DWORD	biSize;				//Structure Size
	DWORD	biWidth;			//Bitmap Width
	DWORD	biHeight;			//Bitmap Height
	WORD 	biPlanes;			//Plane Count (Set 1)
	WORD	biBitCount;			//Bit Count per pixel
	DWORD	biCompression;		//Compression Flag
	DWORD	biSizeImage;		//Image Data Size
	DWORD	biXPelsPerMeter;	//X Meter per pixel
	DWORD	biYPelsPerMeter;	//Y Meter per pixel
	DWORD	biClrUsed;			//Using Color Count
	DWORD	biClrImportant;		//Important Color
}BITMAPFILEHEADER2;

RGBImageRec *DIBImageLoad(char* path, int channel) {
	RGBImageRec* pImage=NULL;
	FILE *f=NULL;
	char *pBuf=NULL;
	int dataSize=0;
	int index=0;
	DWORD x=0;
	DWORD y=0;
	int bpp=0;

	if(channel !=3 && channel !=4) return pImage;
	
	f = fopen(path, "rb");
	if(f != NULL) {
		BITMAPFILEHEADER1 HD1;
		BITMAPFILEHEADER2 HD2;

		fseek(f,0,SEEK_SET);
		fread(&HD1.bfType, sizeof(WORD), 1, f);			// Read the bitmap file header
		fread(&HD1.bfSize,sizeof(DWORD), 1, f);			// Read the bitmap image header
		fseek(f, 10, SEEK_SET);
		fread(&HD1.bfOffBits, sizeof(DWORD),1,  f);		// Read the offset of a bitmap image data(4byte)
	
		fread(&HD2.biSize, sizeof(int), 1, f);			// Read the bitmap file header
		fread(&HD2.biWidth, sizeof(int), 1, f);			// Read the bitmap file header
		fread(&HD2.biHeight, sizeof(int), 1, f);		// Read the bitmap image header
		fread(&HD2.biPlanes, sizeof(WORD), 1, f);		// Bit Plane 1(2byte)
		fread(&HD2.biBitCount, sizeof(WORD), 1, f);		// Bit Count(2byte, 1,4,8,16,24,32)
		fread(&HD2.biCompression, sizeof(int), 1, f);	// Compression Type(4byte, BI_RGB(0))

		fseek(f,HD1.bfOffBits,SEEK_SET);

		bpp = HD2.biBitCount/8;
		if(bpp == 1 || bpp == channel)
		{
			pBuf = (char*) malloc(channel);

			pImage = (RGBImageRec*) malloc(sizeof(RGBImageRec));
			pImage->sizeX = HD2.biWidth;
			pImage->sizeY = HD2.biHeight;

			dataSize = HD2.biWidth*HD2.biHeight*channel;
			pImage->data = (char*) malloc(dataSize);

			for(y=0; y<HD2.biHeight; y++) {
				for(x=0; x<HD2.biWidth; x++) {
					fread(pBuf, bpp, 1, f);
					if(bpp == 1) {
						pBuf[1] = pBuf[2] = pBuf[0];
						if(channel == 4)	pBuf[3] = 0;
					}

					index = (y*HD2.biWidth + x)*channel;
					pImage->data[index]		= pBuf[2];
					pImage->data[index + 1] = pBuf[1];
					pImage->data[index + 2] = pBuf[0];
					if(channel == 4)
						pImage->data[index + 3] = pBuf[3];
				}
			}

			if(pBuf) free(pBuf);
			pBuf = NULL;
		}

		fclose(f);
	}

	return pImage;
}
