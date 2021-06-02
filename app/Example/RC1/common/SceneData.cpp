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
// DATE    : 7/22/2013 Mon
// Author  : Nobody
//
//=======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SceneData.h"

#define M_180_PI        57.29577951308232087684f
#define PI_OVER_360		0.0087266462599716478846184538424431f	// = PI / 360

#define POINT_LIGHT         1
#define DIRENDTIONAL_LIGHT  0
#define SPOT_LIGHT          1

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

RGBImageRec *DIBImageLoad(char* path) {

	RGBImageRec* pImage=NULL;
	FILE *f=NULL;
	unsigned char buf[4];
	int dataSize=0;
	int index=0;
	DWORD x=0;
	DWORD y=0;
	int bpp=0;

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
		if(bpp == 1 || bpp == 3)
		{
			pImage = (RGBImageRec*) malloc(sizeof(RGBImageRec));
			pImage->sizeX = HD2.biWidth;
			pImage->sizeY = HD2.biHeight;

			dataSize = HD2.biWidth*HD2.biHeight*3;
			pImage->data = (unsigned char*) malloc(dataSize);

            pImage->type = 3;
			
			for(y=0; y<HD2.biHeight; y++) {
				for(x=0; x<HD2.biWidth; x++) {
					fread(buf, bpp, 1, f);
					if(bpp == 1) {
						buf[1] = buf[2] = buf[0];
                        pImage->type = 1;
					}

					index = (y*HD2.biWidth + x)*3;
					pImage->data[index]		= buf[2];
					pImage->data[index + 1] = buf[1];
					pImage->data[index + 2] = buf[0];
				}
			}
		}
        else
        if(bpp == 4)
		{
			pImage = (RGBImageRec*) malloc(sizeof(RGBImageRec));
			pImage->sizeX = HD2.biWidth;
			pImage->sizeY = HD2.biHeight;

			dataSize = HD2.biWidth*HD2.biHeight*4;
			pImage->data = (unsigned char*) malloc(dataSize);

            pImage->type = 4;
			
			for(y=0; y<HD2.biHeight; y++) {
				for(x=0; x<HD2.biWidth; x++) {
					fread(buf, bpp, 1, f);

					//index = (HD2.biHeight - 1 - y)*HD2.biWidth*3 + x*3;
					index = (y*HD2.biWidth + x)*4;
					pImage->data[index]		= buf[2];
					pImage->data[index + 1] = buf[1];
					pImage->data[index + 2] = buf[0];
                    pImage->data[index + 3] = buf[3];
				}
			}
		}
		fclose(f);
	}

	return pImage;
}


SceneData::~SceneData(void)
{
	if(m_camera) {
		free(m_camera);
		m_camera = NULL;
	}
	if(m_texture) {
		for(int i=0; i<m_textureCount; i++) {
			if(m_texture[i].pImage)
				if(m_texture[i].pImage->data)
					free(m_texture[i].pImage->data);
		}
		free(m_texture);
		m_texture = NULL;
	}

	if(m_textureNormal) {
		for(int i=0; i<m_textureNormalCount; i++) {
			if(m_textureNormal[i].pImage)
				if(m_textureNormal[i].pImage->data)
					free(m_textureNormal[i].pImage->data);
		}
		free(m_textureNormal);
		m_textureNormal = NULL;
	}

	if(m_textureFlatNormal) {
		for(int i=0; i<m_textureFlatNormalCount; i++) {
			if(m_textureFlatNormal[i].pImage)
				if(m_textureFlatNormal[i].pImage->data)
					free(m_textureFlatNormal[i].pImage->data);
		}
		free(m_textureFlatNormal);
		m_textureFlatNormal = NULL;
	}

	if(m_textureBGSub) {
		for(int i=0; i<m_textureBGSubCount; i++) {
			if(m_textureBGSub[i].pImage)
				if(m_textureBGSub[i].pImage->data)
					free(m_textureBGSub[i].pImage->data);
		}
		free(m_textureBGSub);
		m_textureBGSub = NULL;
	}

	if(m_light) {
		for(int i=0; i<m_lightCount; i++)
			if(m_light[i].frameLight)	free(m_light[i].frameLight);

		free(m_light);
		m_light = NULL;
	}
		
	if(m_material) {
		free(m_material);
		m_material = NULL;
	}

	if(m_object) {
		FreeList(m_object->pMaterialList);
		for(int i=0; i<m_objectCount; i++) {
			if(m_object[i].vertex)			free(m_object[i].vertex);
			if(m_object[i].normalVector)	free(m_object[i].normalVector);
			if(m_object[i].texCoord)		free(m_object[i].texCoord);
		}
		free(m_object);
		m_object = NULL;
	}

	if(m_keyDynamicObject) {
		struct BaseDrawObject *pObject=NULL;
		for(int i=0; i<m_keyDynamicObjectCount; i++) {
			pObject = &m_keyDynamicObject[i].object;
			FreeList(pObject->pMaterialList);
			if(pObject->vertex)			free(pObject->vertex);
			if(pObject->normalVector)	free(pObject->normalVector);
			if(pObject->texCoord)		free(pObject->texCoord);
			if(m_keyDynamicObject[i].frameMatrix) free(m_keyDynamicObject[i].frameMatrix);
		}
		free(m_keyDynamicObject);
		m_keyDynamicObject = NULL;
	}

	if(m_bone) {
		for(int i=0; i<m_boneCount; i++) {
			if(m_bone[i].frameMatrix) free(m_bone[i].frameMatrix);
		}
		free(m_bone);
		m_bone = NULL;
	}

	if(m_boneDynamicObject) {
		struct BoneDrawObject *pObject=NULL;
		for(int i=0; i<m_boneDynamicObjectCount; i++) {
			if(m_boneDynamicObject[i].skin.skinInfo)	free(m_boneDynamicObject[i].skin.skinInfo);
			
			pObject = &m_boneDynamicObject[i].object;
			FreeList(pObject->pMaterialList);
			if(pObject->skinIndex)		free(pObject->skinIndex);
			if(pObject->vertex)			free(pObject->vertex);
			if(pObject->normalVector)	free(pObject->normalVector);
			if(pObject->texCoord)		free(pObject->texCoord);
			if(pObject->boneIDs)		free(pObject->boneIDs);
			if(pObject->matrixIndex)	free(pObject->matrixIndex);
			if(pObject->weight)			free(pObject->weight);
		}
		free(m_boneDynamicObject);
		m_boneDynamicObject = NULL;
	}

	if(m_meshDynamicObject){
		struct BaseDrawObject *pObject=NULL;
		for(int i=0; i<m_meshDynamicObjectCount; i++) {
			if(m_meshDynamicObject[i].object) {
				for(int j=0; j<m_meshDynamicObject[i].frameCount; j++) {
					pObject = &m_meshDynamicObject[i].object[j];
					FreeList(pObject->pMaterialList);
					if(pObject->vertex)			free(pObject->vertex);
					if(pObject->normalVector)	free(pObject->normalVector);
					if(pObject->texCoord)		free(pObject->texCoord);
				}
				free(m_meshDynamicObject[i].object);
			}
		}
		free(m_meshDynamicObject);
		m_meshDynamicObject = NULL;
	}
}


bool SceneData::SceneDataLoad(int screenWidth, int screenHeight)
{
	FILE    *fp = NULL;
	union {
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int     i = 0;
	int     primitiveCount = 0;
	int		aniLightCount = 0;
	int		bgSubTexCount = 0;
	LightAniObject* pAniLight = NULL;
	struct SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;
	float oldAspect = 1.0;
	float newAspect = 1.0;

	if (screenWidth <= 0 || screenHeight <= 0)
		return false;

	//// API_obj.txt
	{			
		strcpy(filename, m_folderName);
		strcat(filename, "API_obj.txt");

		fp = fopen(filename, "r");
		if (!fp)	return false;
		do {
			fscanf(fp, "%d", &itemp);

			if (feof(fp))
				break;
			index++;
		} while (1);
		fclose(fp);
	}	

	m_objectCount = index;
	m_object = (struct BaseDrawObject*)malloc(sizeof(struct BaseDrawObject)*index);
	memset(m_object, 0, sizeof(struct BaseDrawObject)*index);

	fp = fopen(filename, "r");
	index = 0;
	do {
		fscanf(fp, "%d", &itemp);

		if (feof(fp))
			break;

		m_object[index].vertexCount = itemp * 3;

		m_object[index].vertex = (struct Pos*) malloc(sizeof(struct Pos)* itemp * 3);
		memset(m_object[index].vertex, 0, sizeof(struct Pos)* itemp * 3);
		m_object[index].normalVector = (struct Vec*) malloc(sizeof(struct Vec)* itemp * 3);
		memset(m_object[index].normalVector, 0, sizeof(struct Vec)* itemp * 3);
		m_object[index].texCoord = (struct TexC*) malloc(sizeof(struct TexC)* itemp * 3);
		memset(m_object[index].texCoord, 0, sizeof(struct TexC)* itemp * 3);


		index++;
	} while (1);
	fclose(fp);

	///// f_KD_prim_pool.txt
	{
		index = 0;

		strcpy(filename, m_folderName);
		strcat(filename, "f_KD_prim_pool.txt");

		fp = NULL;
		fp = fopen(filename, "r");
		if (!fp)	return false;

		while (index < m_objectCount) {

			i = 0;
			primitiveCount = 0;
			pObject = &m_object[index];

			pObject->pMaterialList = (struct SubMaterialList*) malloc(sizeof(struct SubMaterialList));
			pMaterialList = pObject->pMaterialList;
			memset(pMaterialList, 0, sizeof(struct SubMaterialList));

			while (i < pObject->vertexCount) {
				fscanf(fp, "%f", &pObject->vertex[i].x);
				fscanf(fp, "%f", &pObject->vertex[i].y);
				fscanf(fp, "%f", &pObject->vertex[i].z);
				fscanf(fp, "%f", &pObject->normalVector[i].x);
				fscanf(fp, "%f", &pObject->normalVector[i].y);
				fscanf(fp, "%f", &pObject->normalVector[i].z);
				fscanf(fp, "%f", &pObject->texCoord[i].u);
				fscanf(fp, "%f", &pObject->texCoord[i].v);
				i++;

				fscanf(fp, "%f", &pObject->vertex[i].x);
				fscanf(fp, "%f", &pObject->vertex[i].y);
				fscanf(fp, "%f", &pObject->vertex[i].z);
				fscanf(fp, "%f", &pObject->normalVector[i].x);
				fscanf(fp, "%f", &pObject->normalVector[i].y);
				fscanf(fp, "%f", &pObject->normalVector[i].z);
				fscanf(fp, "%f", &pObject->texCoord[i].u);
				fscanf(fp, "%f", &pObject->texCoord[i].v);
				i++;

				fscanf(fp, "%f", &pObject->vertex[i].x);
				fscanf(fp, "%f", &pObject->vertex[i].y);
				fscanf(fp, "%f", &pObject->vertex[i].z);
				fscanf(fp, "%f", &pObject->normalVector[i].x);
				fscanf(fp, "%f", &pObject->normalVector[i].y);
				fscanf(fp, "%f", &pObject->normalVector[i].z);
				fscanf(fp, "%f", &pObject->texCoord[i].u);
				fscanf(fp, "%f", &pObject->texCoord[i].v);
				i++;

				fscanf(fp, "%f", &ftemp);
				fscanf(fp, "%d", &itemp);

				if (i == 3) {
					pMaterialList->materialIndex = itemp;
					pMaterialList->startPrimitiveIndex = primitiveCount;
				}
				else if (pMaterialList->materialIndex != itemp) {
					pMaterialList->pNext = (struct SubMaterialList*) malloc(sizeof(struct SubMaterialList));
					pMaterialList = pMaterialList->pNext;
					memset(pMaterialList, 0, sizeof(struct SubMaterialList));

					pMaterialList->materialIndex = itemp;
					pMaterialList->startPrimitiveIndex = primitiveCount;
				}

				primitiveCount++;
				pMaterialList->primitiveCount++;
			}

			index++;
		}
		fclose(fp);
	}


	//// texture_info.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "texture_info.txt");

		fp = fopen(filename, "r");
		if (!fp)	return false;

		fscanf(fp, "%d", &m_textureCount);

		if (m_textureCount) {

			char path[256];

			m_texture = (TextureObject*)malloc(sizeof(TextureObject) * m_textureCount);
			memset(m_texture, 0, sizeof(TextureObject) * m_textureCount);

			i = 0;

			strcpy(path, m_folderName);

			while (i < m_textureCount) {

				char bmpfile[100];
				strcpy(path, m_folderName);
#ifdef WIN32
				strcat(path, "bmp\\");
#else
				strcat(path, "bmp/");
#endif
				fscanf(fp, "%s", bmpfile);
				strcat(path, bmpfile);
				//printf("[App %d] %s\n", i+1, path);

				m_texture[i].target = RC_TEXTURE_2D;
				m_texture[i].pImage = (RGBImageRec *)DIBImageLoad(path);
				if (!m_texture[i].pImage) {
					printf("Can't find image file : %s\n", path);
					exit(1);
				}

				InitTexture(i);
				i++;
		}

	}

		fclose(fp);
	}

	//// texture_normal_info.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "texture_normal_info.txt");

		fp = fopen(filename, "r");
		//}sjpark
		//if(!fp)	return false;
		//}sjpark
		if (fp != NULL) {
			fscanf(fp, "%d", &m_textureNormalCount);
		}
		else {
			m_textureNormalCount = 0;
		}

		if (m_textureNormalCount) {
			char path[256];

			m_textureNormal = (TextureObject*)malloc(sizeof(TextureObject) * m_textureNormalCount);
			memset(m_textureNormal, 0, sizeof(TextureObject) * m_textureNormalCount);

			i = 0;

			strcpy(path, m_folderName);

			while (i < m_textureNormalCount) {

				char bmpfile[100];
				strcpy(path, m_folderName);
#ifdef WIN32
				strcat(path, "bmp\\");
#else
				strcat(path, "bmp/");
#endif
				fscanf(fp, "%s", bmpfile);
				strcat(path, bmpfile);
				//printf("[App %d] %s\n", i+1, path);

				m_textureNormal[i].target = RC_TEXTURE_2D_NORMAL;
				m_textureNormal[i].pImage = (RGBImageRec *)DIBImageLoad(path);

				InitTextureNormal(i);
				i++;
			}

		}
		//}sjpark
		if (fp != NULL) {
			fclose(fp);
		}
		//}sjpark
	}

	//// texture_flatnormal_info.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "texture_flatnormal_info.txt");

		fp = fopen(filename, "r");
		if (fp != NULL && m_textureNormalCount > 0) {
			fscanf(fp, "%d", &m_textureFlatNormalCount);

			if (m_textureFlatNormalCount != m_textureNormalCount) {
				m_textureFlatNormalCount = 0;
			}
			else {
				if (m_textureFlatNormalCount) {

					char path[256];

					m_textureFlatNormal = (TextureObject*)malloc(sizeof(TextureObject) * m_textureFlatNormalCount);
					memset(m_textureFlatNormal, 0, sizeof(TextureObject) * m_textureFlatNormalCount);

					i = 0;

					strcpy(path, m_folderName);

					while (i < m_textureNormalCount) {

						fscanf(fp, "%d", &itemp);
						if (itemp == 0)	m_textureFlatNormal[i].nouse = 1;
						else			m_textureFlatNormal[i].nouse = 0;

						char bmpfile[100];
						strcpy(path, m_folderName);
#ifdef WIN32
						strcat(path, "bmp\\");
#else
						strcat(path, "bmp/");
#endif
						fscanf(fp, "%s", bmpfile);
						strcat(path, bmpfile);

						m_textureFlatNormal[i].target = RC_TEXTURE_2D_NORMAL;
						m_textureFlatNormal[i].pImage = (RGBImageRec *)DIBImageLoad(path);

						InitTextureFlatNormal(i);
						i++;
					}
				}
			}
			fclose(fp);
		}
	}

	/////// f_material.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "f_material.txt");

		fp = fopen(filename, "r");
		if (!fp)	return false;

		fscanf(fp, "%d", &m_materialCount);

		m_material = (struct MaterialObject*) malloc(sizeof(struct MaterialObject)*m_materialCount);
		memset(m_material, 0, sizeof(struct MaterialObject)*m_materialCount);

		fscanf(fp, "%f", &m_scene.clearColor.r);
		fscanf(fp, "%f", &m_scene.clearColor.g);
		fscanf(fp, "%f", &m_scene.clearColor.b);

		for (index = 0; index < m_materialCount; index++) {
			struct MaterialObject* pMat = &m_material[index];
			fscanf(fp, "%d", &pMat->texIndex);
			fscanf(fp, "%d %d", &pMat->tex_repeat[0], &pMat->tex_repeat[1]);
			fscanf(fp, "%d", &pMat->norIndex);
			fscanf(fp, "%d %d", &pMat->nor_repeat[0], &pMat->nor_repeat[1]);
			
			fscanf(fp, "%f %f %f", &pMat->ambient.r, &pMat->ambient.g, &pMat->ambient.b);
			fscanf(fp, "%f %f %f", &pMat->diffuse.r, &pMat->diffuse.g, &pMat->diffuse.b);
			fscanf(fp, "%f %f %f", &pMat->specular.r, &pMat->specular.g, &pMat->specular.b);

			fscanf(fp, "%f", &pMat->level);
			fscanf(fp, "%f", &pMat->exponent);
			fscanf(fp, "%f", &pMat->reflection);
			fscanf(fp, "%f", &pMat->transmittance);
			fscanf(fp, "%f", &pMat->refractionIndex);
			fscanf(fp, "%f", &pMat->selfIllum);

			//pMat->reflection = 0.f;

			// roughness
			fscanf(fp, "%f", &pMat->roughness);
			//pMat->roughness = 0.1f;
			// roughness
		}

		fclose(fp);
	}

	//// texture_bg_sub_info.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "texture_bg_sub_info.txt");

		fp = fopen(filename, "r");
		if (fp != NULL) {
			fscanf(fp, "%d", &m_materialIndexBG);

			if (m_materialIndexBG >= 0 && m_materialCount > 0) {
				fscanf(fp, "%d", &m_textureBGSubCount);

				if (m_textureBGSubCount) {

					char path[256];

					m_textureBGSub = (TextureObject*)malloc(sizeof(TextureObject) * m_textureBGSubCount);
					memset(m_textureBGSub, 0, sizeof(TextureObject) * m_textureBGSubCount);

					i = 0;

					strcpy(path, m_folderName);

					while (i < m_textureBGSubCount) {
						char bmpfile[100];
						strcpy(path, m_folderName);
#ifdef WIN32
						strcat(path, "bmp\\");
#else
						strcat(path, "bmp/");
#endif
						fscanf(fp, "%s", bmpfile);
						strcat(path, bmpfile);

						m_textureBGSub[i].target = RC_TEXTURE_2D;
						m_textureBGSub[i].pImage = (RGBImageRec *)DIBImageLoad(path);

						InitTextureBGSub(i);
						i++;
					}
				}
			}

			fclose(fp);
		}
	}

    /////// f_light.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "f_light.txt");

		fp = NULL;
		fp = fopen(filename, "r");
		if (!fp)	return false;

		fscanf(fp, "%d", &m_lightCount);

		if (m_lightCount) {
			m_enableLightCount = m_lightCount;

			m_light = (struct LightObject*) malloc(sizeof(struct LightObject)*m_lightCount);
			memset(m_light, 0, sizeof(struct LightObject)*m_lightCount);

			for (index = 0; index < m_lightCount; index++) {
				fscanf(fp, "%f", &m_light[index].pos.x);			// Light Position X
				fscanf(fp, "%f", &m_light[index].pos.y);			// Light Position Y
				fscanf(fp, "%f", &m_light[index].pos.z);			// Light Position Z
				fscanf(fp, "%f", &m_light[index].direction.x);	// Light Direction X
				fscanf(fp, "%f", &m_light[index].direction.y);	// Light Direction Y
				fscanf(fp, "%f", &m_light[index].direction.z);	// Light Direction Z
				fscanf(fp, "%f", &m_light[index].Spot[0]);		// Cos(theta) of spot light in-cone
				fscanf(fp, "%f", &m_light[index].Spot[1]);		// Cos(theta) of spot light out-cone
				fscanf(fp, "%f", &m_light[index].Spot[2]);		// Fall-off value of spot light
				fscanf(fp, "%f", &m_light[index].ambient.r);		// Red component of Ambient light color
				fscanf(fp, "%f", &m_light[index].ambient.g);		// Green component of Ambient light color
				fscanf(fp, "%f", &m_light[index].ambient.b);		// Blue component of Ambient light color
				fscanf(fp, "%f", &m_light[index].diffuse.r);		// Red component of Diffuse light color
				fscanf(fp, "%f", &m_light[index].diffuse.g);		// Green component of Diffuse light color
				fscanf(fp, "%f", &m_light[index].diffuse.b);		// Blue component of Diffuse light color
				fscanf(fp, "%f", &m_light[index].specular.r);	// Red component of Specular light color
				fscanf(fp, "%f", &m_light[index].specular.g);	// Green component of Specular light color
				fscanf(fp, "%f", &m_light[index].specular.b);	// Blue component of Specular light color
				fscanf(fp, "%f", &m_light[index].Atten[3]);		// Start of attenuation
				fscanf(fp, "%f", &m_light[index].Atten[4]);		// End of attenuation
				fscanf(fp, "%f", &m_light[index].Atten[0]);		// Attenuation coefficient 0
				fscanf(fp, "%f", &m_light[index].Atten[1]);		// Attenuation coefficient 1
				fscanf(fp, "%f", &m_light[index].Atten[2]);		// Attenuation coefficient 2
				fscanf(fp, "%d", &m_light[index].type);			// Light type
			}
		}
		fclose(fp);
	}

    //// f_light_static.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "f_light_static.txt");

		fp = fopen(filename, "r");
		if (!fp)	return false;

		if (m_light == NULL) {
			m_light = (struct LightObject*) malloc(sizeof(struct LightObject));
			memset(m_light, 0, sizeof(struct LightObject));
		}

		fscanf(fp, "%f", &m_light[0].ambient.r);		// Red component of Ambient light color
		fscanf(fp, "%f", &m_light[0].ambient.g);		// Green component of Ambient light color
		fscanf(fp, "%f", &m_light[0].ambient.b);		// Blue component of Ambient light color

		fclose(fp);
	}

    //// resolution.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "resolution.txt");

		fp = fopen(filename, "r");
		if (!fp)	return false;

		fscanf(fp, "%d %d", &m_scene.screenWidth, &m_scene.screenHeight);

		fclose(fp);
	}

    //// f_camera.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "f_camera.txt");

		fp = fopen(filename, "r");
		if (!fp)	return false;

		fscanf(fp, "%d", &m_cameraCount);//dk

		m_camera = (struct CameraObject*) malloc(sizeof(struct CameraObject)*m_cameraCount);
		memset(m_camera, 0, sizeof(struct CameraObject)*m_cameraCount);

		for (index = 0; index < m_cameraCount; index++) {
			fscanf(fp, "%f", &m_camera[index].pos.x);
			fscanf(fp, "%f", &m_camera[index].pos.y);
			fscanf(fp, "%f", &m_camera[index].pos.z);
			fscanf(fp, "%f", &m_camera[index].look.x);
			fscanf(fp, "%f", &m_camera[index].look.y);
			fscanf(fp, "%f", &m_camera[index].look.z);
			fscanf(fp, "%f", &m_camera[index].up.x);
			fscanf(fp, "%f", &m_camera[index].up.y);
			fscanf(fp, "%f", &m_camera[index].up.z);
			fscanf(fp, "%f", &m_camera[index].distance);
			fscanf(fp, "%f", &m_camera[index].width);


			m_camera[index].right = m_camera[index].width / 2;
			m_camera[index].left = -m_camera[index].right;
			m_camera[index].aspect = (float)m_scene.screenWidth / (float)m_scene.screenHeight;
			m_camera[index].top = m_camera[index].right / m_camera[index].aspect;
			m_camera[index].bottom = -m_camera[index].top;

			m_camera[index].fov = m_camera[index].top - m_camera[index].bottom;
			m_camera[index].fov /= 2.0f*m_camera[index].distance;
			m_camera[index].fov = atan(m_camera[index].fov);
			m_camera[index].fov /= PI_OVER_360;
		}

		fclose(fp);
	}

	// User defined!
	m_scene.screenHeight = screenHeight;
	m_scene.screenWidth = screenWidth;

	//// frames.txt
	{
		strcpy(filename, m_folderName);
		strcat(filename, "frames.txt");

		fp = fopen(filename, "r");
		if (!fp)	return true;

		fscanf(fp, "%d", &m_Frame.frameCount);
		fscanf(fp, "%d", &m_Frame.cameraAniFlag);
		fscanf(fp, "%d", &m_Frame.lightAniFlag);
		fclose(fp);
	}

	//// f_light_ani.txt
	{
		if (!m_Frame.lightAniFlag) return true;

		strcpy(filename, m_folderName);
		strcat(filename, "f_light_ani.txt");

		fp = fopen(filename, "r");
		if (!fp)	return true;

		fscanf(fp, "%d", &aniLightCount);

		if (aniLightCount) {

			for (index = 0; index < aniLightCount; index++) {
				fscanf(fp, "%d", &itemp);
				if (itemp > -1 && itemp < m_lightCount) {
					m_light[itemp].frameLight = (LightAniObject*)malloc(sizeof(struct LightAniObject)*m_Frame.frameCount);
					memset(m_light[itemp].frameLight, 0, sizeof(struct LightAniObject)*m_Frame.frameCount);
				}
			}

			i = 0;
			while (i < m_Frame.frameCount) {

				for (index = 0; index < aniLightCount; index++) {
					fscanf(fp, "%d", &itemp);
					if (itemp > -1 && itemp < m_lightCount) {

						pAniLight = &m_light[itemp].frameLight[i];

						fscanf(fp, "%f", &pAniLight->pos.x);
						fscanf(fp, "%f", &pAniLight->pos.y);
						fscanf(fp, "%f", &pAniLight->pos.z);
						fscanf(fp, "%f", &pAniLight->direction.x);
						fscanf(fp, "%f", &pAniLight->direction.y);
						fscanf(fp, "%f", &pAniLight->direction.z);
						fscanf(fp, "%f", &pAniLight->diffuse.r);
						fscanf(fp, "%f", &pAniLight->diffuse.g);
						fscanf(fp, "%f", &pAniLight->diffuse.b);
					}
					else {
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
						fscanf(fp, "%f", &ftemp);
					}
				}

				i++;
			}

		}

		fclose(fp);
	}

	return true;
}


bool SceneData::SceneDataLoadx(int screenWidth, int screenHeight)
{
	FILE    *fp=NULL;
	union {
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int     i = 0;
	int     j = 0;
	int     primitiveCount = 0;
	int		aniLightCount = 0;
	int		bgSubTexCount = 0;
	LightAniObject* pAniLight = NULL;
	struct SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;
	float oldAspect = 1.0;
	float newAspect = 1.0;

	if( screenWidth <= 0 || screenHeight <= 0)
		return false;

	////
	strcpy(filename, m_folderName);
	strcat(filename, "API_obj.txt");

	fp = fopen(filename, "r");
	if(!fp)	return false;
	do{
		fscanf (fp, "%d", &itemp);

		if (feof(fp))
			break;
		index++;
	}while(1);
	fclose(fp);

	m_objectCount = index;
	m_object = (struct BaseDrawObject*)malloc(sizeof(struct BaseDrawObject)*index);
	memset(m_object, 0, sizeof(struct BaseDrawObject)*index);

	fp = fopen(filename, "r");
	index = 0;
	do{
		fscanf (fp, "%d", &itemp);

		if (feof(fp))
			break;

		m_object[index].vertexCount = itemp*3;

		m_object[index].vertex        = (struct Pos*) malloc (sizeof(struct Pos)* itemp*3);
		memset(m_object[index].vertex, 0, sizeof(struct Pos)* itemp*3);
		m_object[index].normalVector  = (struct Vec*) malloc (sizeof(struct Vec)* itemp*3);
		memset(m_object[index].normalVector, 0, sizeof(struct Vec)* itemp*3);
		m_object[index].texCoord      = (struct TexC*) malloc (sizeof(struct TexC)* itemp*3);
		memset(m_object[index].texCoord, 0, sizeof(struct TexC)* itemp*3);


		index++;
	}while(1);
	fclose(fp);

	/////
	index = 0;

	strcpy(filename, m_folderName);
	strcat(filename, "fx_KD_prim_pool.dat");

	fp = NULL;
	fp = fopen(filename, "rb");
	if(!fp)	return false;

	while(index < m_objectCount){

		i = 0;
		primitiveCount = 0;
		pObject = &m_object[index];

		pObject->pMaterialList = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
		pMaterialList = pObject->pMaterialList;
		memset(pMaterialList, 0, sizeof(struct SubMaterialList));

		while(i < pObject->vertexCount) {

			for(j = 0; j < 3; j++) {
				fread(&pObject->vertex[i].x, 1, sizeof(float)*3, fp);
				fread(&pObject->normalVector[i].x, 1, sizeof(float)*3, fp);
				fread(&pObject->texCoord[i].u, 1, sizeof(float)*2, fp);
				i++;
			}
			fread(&ftemp, 1, sizeof(float), fp);
			fread(&itemp, 1, sizeof(int), fp);

			if(i == 3) {
				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			} else if (pMaterialList->materialIndex != itemp) {
				pMaterialList->pNext = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
				pMaterialList = pMaterialList->pNext;
				memset(pMaterialList, 0, sizeof(struct SubMaterialList));

				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			}

			primitiveCount++;
			pMaterialList->primitiveCount++;
		}

		index++;
	}
	fclose(fp);


	////
	strcpy(filename, m_folderName);
	strcat(filename, "texture_info.txt");

	fp = fopen(filename, "r");
	if(!fp)	return false;

	fscanf(fp,"%d", &m_textureCount);

	if (m_textureCount) {

		char path[64];

		m_texture = (TextureObject*) malloc (sizeof(TextureObject) * m_textureCount);
		memset(m_texture, 0, sizeof(TextureObject) * m_textureCount);

		i = 0;

		strcpy(path, m_folderName);

		while(i<m_textureCount) {

			char bmpfile[100];
			strcpy(path, m_folderName);
#ifdef WIN32
			strcat(path, "bmp\\");
#else
			strcat(path, "bmp/");
#endif
			fscanf(fp,"%s", bmpfile);
			strcat(path, bmpfile);
			//printf("[App %d] %s\n", i+1, path);

			m_texture[i].target = RC_TEXTURE_2D;
			m_texture[i].pImage = (RGBImageRec *)DIBImageLoad(path);	

			InitTexture(i);
			i++;
		}

	}

	fclose(fp);


	////
	strcpy(filename, m_folderName);
	strcat(filename, "texture_normal_info.txt");

	fp = fopen(filename, "r");
	if(!fp)	return false;


	fscanf(fp,"%d", &m_textureNormalCount);

	if (m_textureNormalCount) {
		char path[64];

		m_textureNormal = (TextureObject*) malloc (sizeof(TextureObject) * m_textureNormalCount);
		memset(m_textureNormal, 0, sizeof(TextureObject) * m_textureNormalCount);

		i = 0;

		strcpy(path, m_folderName);

		while(i<m_textureNormalCount) {

			char bmpfile[100];
			strcpy(path, m_folderName);
#ifdef WIN32
			strcat(path, "bmp\\");
#else
			strcat(path, "bmp/");
#endif
			fscanf(fp,"%s", bmpfile);
			strcat(path, bmpfile);
			//printf("[App %d] %s\n", i+1, path);

			m_textureNormal[i].target = RC_TEXTURE_2D_NORMAL;
			m_textureNormal[i].pImage = (RGBImageRec *)DIBImageLoad(path);	

			InitTextureNormal(i);
			i++;
		}

	}

	fclose(fp);


	////
	strcpy(filename, m_folderName);
	strcat(filename, "texture_flatnormal_info.txt");

	fp = fopen(filename, "r");
	if(fp != NULL && m_textureNormalCount > 0) {
		fscanf(fp,"%d", &m_textureFlatNormalCount);

		if(m_textureFlatNormalCount != m_textureNormalCount) {
			m_textureFlatNormalCount = 0;
		} else {
			if (m_textureFlatNormalCount) {

				char path[64];

				m_textureFlatNormal = (TextureObject*) malloc (sizeof(TextureObject) * m_textureFlatNormalCount);
				memset(m_textureFlatNormal, 0, sizeof(TextureObject) * m_textureFlatNormalCount);

				i = 0;

				strcpy(path, m_folderName);

				while(i<m_textureNormalCount) {

					fscanf(fp,"%d", &itemp);
					if(itemp == 0)	m_textureFlatNormal[i].nouse = 1;
					else			m_textureFlatNormal[i].nouse = 0;

					char bmpfile[100];
					strcpy(path, m_folderName);
#ifdef WIN32
					strcat(path, "bmp\\");
#else
					strcat(path, "bmp/");
#endif
					fscanf(fp,"%s", bmpfile);
					strcat(path, bmpfile);

					m_textureFlatNormal[i].target = RC_TEXTURE_2D_NORMAL;
					m_textureFlatNormal[i].pImage = (RGBImageRec *)DIBImageLoad(path);	

					InitTextureFlatNormal(i);
					i++;
				}
			}
		}
		fclose(fp);
	}


	///////
	strcpy(filename, m_folderName);
	strcat(filename, "fx_material.dat");

	fp = fopen(filename, "rb");
	if(!fp)	return false;

	fread(&m_materialCount, 1, sizeof(int), fp);	

	m_material = (struct MaterialObject*) malloc (sizeof(struct MaterialObject)*m_materialCount);
	memset(m_material, 0, sizeof(struct MaterialObject)*m_materialCount);

	fread(&m_scene.clearColor.r, 1, sizeof(float)*3, fp);

	for(index = 0; index < m_materialCount; index++){
		struct MaterialObject* pMat	= &m_material[index];
		fread(&pMat->texIndex, 1, sizeof(int), fp);
		fread(&pMat->tex_repeat[0], 1, sizeof(int)*2, fp);
		fread(&pMat->norIndex, 1, sizeof(int), fp);
		fread(&pMat->nor_repeat[0], 1, sizeof(int)*2, fp);
		fread(&pMat->ambient.r, 1, sizeof(float)*3, fp);
		fread(&pMat->diffuse.r, 1, sizeof(float)*3, fp);
		fread(&pMat->specular.r, 1, sizeof(float)*3, fp);
		fread(&pMat->level, 1, sizeof(float), fp);
		fread(&pMat->exponent, 1, sizeof(float), fp);
		fread(&pMat->reflection, 1, sizeof(float), fp);
		fread(&pMat->transmittance, 1, sizeof(float), fp);
		fread(&pMat->refractionIndex, 1, sizeof(float), fp);		
		fread(&pMat->selfIllum, 1, sizeof(float), fp);
	}

	fclose(fp);


	///
	strcpy(filename, m_folderName);
	strcat(filename, "texture_bg_sub_info.txt");

	fp = fopen(filename, "r");
	if(fp != NULL) {
		fscanf (fp, "%d", &m_materialIndexBG);

		if(m_materialIndexBG >= 0 && m_materialCount > 0) {
			fscanf (fp, "%d", &m_textureBGSubCount);

			if(m_textureBGSubCount) {

				char path[64];

				m_textureBGSub = (TextureObject*) malloc (sizeof(TextureObject) * m_textureBGSubCount);
				memset(m_textureBGSub, 0, sizeof(TextureObject) * m_textureBGSubCount);

				i = 0;

				strcpy(path, m_folderName);

				while(i<m_textureBGSubCount) {
					char bmpfile[100];
					strcpy(path, m_folderName);
#ifdef WIN32
					strcat(path, "bmp\\");
#else
					strcat(path, "bmp/");
#endif
					fscanf(fp,"%s", bmpfile);
					strcat(path, bmpfile);

					m_textureBGSub[i].target = RC_TEXTURE_2D;
					m_textureBGSub[i].pImage = (RGBImageRec *)DIBImageLoad(path);	

					InitTextureBGSub(i);
					i++;
				}
			}
		}

		fclose(fp);
	}


	///////
	strcpy(filename, m_folderName);
	strcat(filename, "fx_light.dat");

	fp = NULL;
	fp = fopen(filename, "rb");
	if(!fp)	return false;

	fread(&m_lightCount, 1, sizeof(int), fp);	

	if (m_lightCount) {
		m_enableLightCount = m_lightCount;

		m_light = (struct LightObject*) malloc (sizeof(struct LightObject)*m_lightCount);
		memset(m_light, 0, sizeof(struct LightObject)*m_lightCount);

		for(index = 0; index < m_lightCount; index++){
			fread(&m_light[index].pos.x, 1, sizeof(float)*3, fp);	      // Light Position X, Y, Z
			fread(&m_light[index].direction.x, 1, sizeof(float)*3, fp); // Light Direction X, Y, Z
			fread(&m_light[index].Spot[0], 1, sizeof(float)*3, fp);   // Cos(theta) of spot light in-cone, Cos(theta) of spot light out-cone, Fall-off value of spot light
			fread(&m_light[index].ambient.r, 1, sizeof(float)*3, fp);	  // RGB component of Ambient light color 
			fread(&m_light[index].diffuse.r, 1, sizeof(float)*3, fp);	  // RGB component of Diffuse light color
			fread(&m_light[index].specular.r, 1, sizeof(float)*3, fp);  // RGB component of Specular light color
			fread(&m_light[index].Atten[3], 1, sizeof(float)*2, fp);  // Start, End of attenuation
			fread(&m_light[index].Atten[0], 1, sizeof(float)*3, fp);  // Attenuation coefficient(0, 1, 2)			
			fread(&m_light[index].type, 1, sizeof(int), fp);	      // Light type
		}
	}
	fclose(fp);


	////
	strcpy(filename, m_folderName);
	strcat(filename, "fx_light_static.dat");

	fp = fopen(filename, "rb");
	if(!fp)	return false;

	if(m_light == NULL) {
		m_light = (struct LightObject*) malloc (sizeof(struct LightObject));
		memset(m_light, 0, sizeof(struct LightObject));
	}

	fread(&m_light[0].ambient.r, 1, sizeof(float)*3, fp);  // RGB component of Ambient light color
	fclose(fp);


	////
	strcpy(filename, m_folderName);
	strcat(filename, "resolution.txt");

	fp = fopen(filename, "r");
	if(!fp)	return false;

	fscanf(fp, "%d %d", &m_scene.screenWidth, &m_scene.screenHeight);

	fclose(fp);


	////
	strcpy(filename, m_folderName);
	strcat(filename, "fx_camera.dat");

	fp = fopen(filename, "rb");
	if(!fp)	return false;    

	fread(&m_cameraCount, 1, sizeof(int), fp);

	m_camera = (struct CameraObject*) malloc (sizeof(struct CameraObject)*m_cameraCount);
	memset(m_camera, 0, sizeof(struct CameraObject)*m_cameraCount);

	for(index=0;index<m_cameraCount;index++){
		fread(&m_camera[index].pos.x, 1, sizeof(float)*3, fp);
		fread(&m_camera[index].look.x, 1, sizeof(float)*3, fp);
		fread(&m_camera[index].up.x, 1, sizeof(float)*3, fp);
		fread(&m_camera[index].distance, 1, sizeof(float), fp);
		fread(&m_camera[index].width, 1, sizeof(float), fp);

		m_camera[index].right = m_camera[index].width / 2;
		m_camera[index].left = - m_camera[index].right;
		m_camera[index].aspect = (float) m_scene.screenWidth / (float) m_scene.screenHeight;
		m_camera[index].top = m_camera[index].right / m_camera[index].aspect;
		m_camera[index].bottom = - m_camera[index].top;

		m_camera[index].fov = m_camera[index].top - m_camera[index].bottom;
		m_camera[index].fov /= 2.0f*m_camera[index].distance;
		m_camera[index].fov = atan(m_camera[index].fov);
		m_camera[index].fov /= PI_OVER_360;
	}

	fclose(fp);

	// User defined!
	m_scene.screenHeight = screenHeight;
	m_scene.screenWidth = screenWidth;


	////
	strcpy(filename, m_folderName);
	strcat(filename, "frames.txt");

	fp = fopen(filename, "r");
	if(!fp)	return true;

	fscanf (fp, "%d", &m_Frame.frameCount);
	fscanf (fp, "%d", &m_Frame.cameraAniFlag);
	fscanf (fp, "%d", &m_Frame.lightAniFlag);
	fclose(fp);


	////
	if(!m_Frame.lightAniFlag) return true;

	strcpy(filename, m_folderName);
	strcat(filename, "fx_light_ani.dat");

	fp = fopen(filename, "rb");
	if(!fp)	return true;

	fread(&aniLightCount, 1, sizeof(int), fp);
	
	if(aniLightCount) {

		for(index = 0; index < aniLightCount; index++){
			fread(&itemp, 1, sizeof(int), fp);
			if(itemp > -1 && itemp < m_lightCount) {
				m_light[itemp].frameLight = (LightAniObject*) malloc(sizeof(struct LightAniObject)*m_Frame.frameCount);
				memset(m_light[itemp].frameLight, 0, sizeof(struct LightAniObject)*m_Frame.frameCount);
			}
		}

		i = 0;
		while(i<m_Frame.frameCount) {

			for(index = 0; index < aniLightCount; index++){
				fread(&itemp, 1, sizeof(int), fp);
				if(itemp > -1 && itemp < m_lightCount) {

					pAniLight = &m_light[itemp].frameLight[i];

					fread(&pAniLight->pos.x, 1, sizeof(float)*3, fp);
					fread(&pAniLight->direction.x, 1, sizeof(float)*3, fp);
					fread(&pAniLight->diffuse.r, 1, sizeof(float)*3, fp);
				} else {
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
					fread(&ftemp, 1, sizeof(float), fp);
				}
			}

			i++;
		}

	}	

	fclose(fp);


	return true;
}


bool SceneData::KeyAnimationDataLoad(void)
{
	FILE    *fp=NULL;
	union {
		char    sChar[256];
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int     i = 0;
	int     primitiveCount = 0;
	struct SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;
	struct Matrix16f tmpMt;
	
	if(m_Frame.frameCount < 2)
		return true;

	////
	strcpy(filename, m_folderName);
	strcat(filename, "FILE.txt");

	fp = fopen(filename, "r");
	if(!fp)	return true;

	fscanf (fp, "%d", &itemp);
	if(itemp==0){
		fclose(fp);
		return true;
	}	

	m_keyDynamicObjectCount = itemp;
	m_keyDynamicObject = (struct KeyDynamicObject*) malloc (sizeof(struct KeyDynamicObject)*itemp);
	memset(m_keyDynamicObject, 0, sizeof(struct KeyDynamicObject)*itemp);

	index = 0;
	do{
		if(index >= m_keyDynamicObjectCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_keyDynamicObject[index].sName, sChar);

		fscanf (fp, "%d", &itemp);
		if(itemp > 0) {			
			m_keyDynamicObject[index].object.vertexCount   = itemp*3;
			m_keyDynamicObject[index].object.vertex        = (struct Pos*) malloc (sizeof(struct Pos)* itemp*3);
			memset(m_keyDynamicObject[index].object.vertex, 0, sizeof(struct Pos)* itemp*3);
			m_keyDynamicObject[index].object.normalVector  = (struct Vec*) malloc (sizeof(struct Vec)* itemp*3);
			memset(m_keyDynamicObject[index].object.normalVector, 0, sizeof(struct Vec)* itemp*3);
			m_keyDynamicObject[index].object.texCoord      = (struct TexC*) malloc (sizeof(struct TexC)* itemp*3);
			memset(m_keyDynamicObject[index].object.texCoord, 0, sizeof(struct TexC)* itemp*3);
		}
		
		if (feof(fp))
			break;
		
		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_keyDynamicObjectCount){

		if(m_keyDynamicObject[index].object.vertexCount == 0) continue;

		////
		strcpy(filename, m_folderName);
		strcat(filename, "f_KD_prim_pool_");
		strcat(filename, m_keyDynamicObject[index].sName);
		strcat(filename, ".txt");

		fp = fopen(filename, "r");
		if(!fp)	return false;

		i = 0;
		primitiveCount = 0;
		pObject = &m_keyDynamicObject[index].object;

		pObject->pMaterialList = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
		pMaterialList = pObject->pMaterialList;
		memset(pMaterialList, 0, sizeof(struct SubMaterialList));

		while(i < pObject->vertexCount) {

			fscanf(fp,"%f", &pObject->vertex[i].x);
			fscanf(fp,"%f", &pObject->vertex[i].y);
			fscanf(fp,"%f", &pObject->vertex[i].z);
			fscanf(fp,"%f", &pObject->normalVector[i].x);
			fscanf(fp,"%f", &pObject->normalVector[i].y);
			fscanf(fp,"%f", &pObject->normalVector[i].z);
			fscanf(fp,"%f", &pObject->texCoord[i].u);
			fscanf(fp,"%f", &pObject->texCoord[i].v);
			i++;

			fscanf(fp,"%f", &pObject->vertex[i].x);
			fscanf(fp,"%f", &pObject->vertex[i].y);
			fscanf(fp,"%f", &pObject->vertex[i].z);
			fscanf(fp,"%f", &pObject->normalVector[i].x);
			fscanf(fp,"%f", &pObject->normalVector[i].y);
			fscanf(fp,"%f", &pObject->normalVector[i].z);
			fscanf(fp,"%f", &pObject->texCoord[i].u);
			fscanf(fp,"%f", &pObject->texCoord[i].v);
			i++;

			fscanf(fp,"%f", &pObject->vertex[i].x);
			fscanf(fp,"%f", &pObject->vertex[i].y);
			fscanf(fp,"%f", &pObject->vertex[i].z);
			fscanf(fp,"%f", &pObject->normalVector[i].x);
			fscanf(fp,"%f", &pObject->normalVector[i].y);
			fscanf(fp,"%f", &pObject->normalVector[i].z);
			fscanf(fp,"%f", &pObject->texCoord[i].u);
			fscanf(fp,"%f", &pObject->texCoord[i].v);
			i++;

			fscanf(fp,"%f", &ftemp);
			fscanf(fp,"%d", &itemp);

			if(i == 3) {
				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			} else if (pMaterialList->materialIndex != itemp) {
				pMaterialList->pNext = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
				pMaterialList = pMaterialList->pNext;
				memset(pMaterialList, 0, sizeof(struct SubMaterialList));

				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			}

			primitiveCount++;
			pMaterialList->primitiveCount++;
		}

		fclose(fp);


		if(m_Frame.frameCount == 0) continue;
		////
		strcpy(filename, m_folderName);
		strcat(filename, "f_matrix_");
		strcat(filename, m_keyDynamicObject[index].sName);
		strcat(filename, ".txt");

		fp = fopen(filename, "r");
		if(!fp)	return false;
		
		i = 0;
		m_keyDynamicObject[index].frameMatrix = (struct Matrix16f*) malloc (sizeof(struct Matrix16f)* m_Frame.frameCount);
		memset(m_keyDynamicObject[index].frameMatrix, 0, sizeof(struct Matrix16f)* m_Frame.frameCount);

		while(i < m_Frame.frameCount) {
			fscanf(fp,"%d", &itemp);
			for(int k=0; k<16; k++)
				fscanf(fp,"%f", &tmpMt.m[k]);

			memcpy(m_keyDynamicObject[index].frameMatrix[itemp].m, tmpMt.m, sizeof(float)*16);

			i++;
		}

		fclose(fp);

		index++;
	}

	return true;
}


bool SceneData::KeyAnimationDataLoadx(void)
{
	FILE    *fp=NULL;
	union {
		char    sChar[256];
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int     i = 0;
	int     j = 0;
	int     primitiveCount = 0;
	struct SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;
	//struct Matrix16f tmpMt;

	if(m_Frame.frameCount < 2)
		return true;

	////
	strcpy(filename, m_folderName);
	strcat(filename, "FILE.txt");

	fp = fopen(filename, "r");
	if(!fp)	return true;

	fscanf (fp, "%d", &itemp);
	if(itemp==0){
		fclose(fp);
		return true;
	}	

	m_keyDynamicObjectCount = itemp;
	m_keyDynamicObject = (struct KeyDynamicObject*) malloc (sizeof(struct KeyDynamicObject)*itemp);
	memset(m_keyDynamicObject, 0, sizeof(struct KeyDynamicObject)*itemp);

	index = 0;
	do{
		if(index >= m_keyDynamicObjectCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_keyDynamicObject[index].sName, sChar);

		fscanf (fp, "%d", &itemp);
		if(itemp > 0) {			
			m_keyDynamicObject[index].object.vertexCount   = itemp*3;
			m_keyDynamicObject[index].object.vertex        = (struct Pos*) malloc (sizeof(struct Pos)* itemp*3);
			memset(m_keyDynamicObject[index].object.vertex, 0, sizeof(struct Pos)* itemp*3);
			m_keyDynamicObject[index].object.normalVector  = (struct Vec*) malloc (sizeof(struct Vec)* itemp*3);
			memset(m_keyDynamicObject[index].object.normalVector, 0, sizeof(struct Vec)* itemp*3);
			m_keyDynamicObject[index].object.texCoord      = (struct TexC*) malloc (sizeof(struct TexC)* itemp*3);
			memset(m_keyDynamicObject[index].object.texCoord, 0, sizeof(struct TexC)* itemp*3);
		}

		if (feof(fp))
			break;

		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_keyDynamicObjectCount){

		if(m_keyDynamicObject[index].object.vertexCount == 0) continue;

		////
		strcpy(filename, m_folderName);
		strcat(filename, "fx_KD_prim_pool_");
		strcat(filename, m_keyDynamicObject[index].sName);
		strcat(filename, ".dat");

		fp = fopen(filename, "rb");
		if(!fp)	return false;

		i = 0;
		primitiveCount = 0;
		pObject = &m_keyDynamicObject[index].object;

		pObject->pMaterialList = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
		pMaterialList = pObject->pMaterialList;
		memset(pMaterialList, 0, sizeof(struct SubMaterialList));

		while(i < pObject->vertexCount) {

			for(j = 0; j < 3; j++) {
				fread(&pObject->vertex[i].x, 1, sizeof(float)*3, fp);
				fread(&pObject->normalVector[i].x, 1, sizeof(float)*3, fp);
				fread(&pObject->texCoord[i].u, 1, sizeof(float)*2, fp);
				i++;
			}
			fread(&ftemp, 1, sizeof(float), fp);
			fread(&itemp, 1, sizeof(int), fp);

			if(i == 3) {
				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			} else if (pMaterialList->materialIndex != itemp) {
				pMaterialList->pNext = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
				pMaterialList = pMaterialList->pNext;
				memset(pMaterialList, 0, sizeof(struct SubMaterialList));

				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			}

			primitiveCount++;
			pMaterialList->primitiveCount++;
		}

		fclose(fp);


		if(m_Frame.frameCount == 0) continue;
		////
		strcpy(filename, m_folderName);
		strcat(filename, "fx_matrix_");
		strcat(filename, m_keyDynamicObject[index].sName);
		strcat(filename, ".dat");

		fp = fopen(filename, "rb");
		if(!fp)	return false;

		i = 0;
		m_keyDynamicObject[index].frameMatrix = (struct Matrix16f*) malloc (sizeof(struct Matrix16f)* m_Frame.frameCount);
		memset(m_keyDynamicObject[index].frameMatrix, 0, sizeof(struct Matrix16f)* m_Frame.frameCount);

		while(i < m_Frame.frameCount) {
			fread(&itemp, 1, sizeof(int), fp);
			
			//fread(&tmpMt.m[0], 1, sizeof(float)*16, fp);
			//memcpy(m_keyDynamicObject[index].frameMatrix[itemp].m, tmpMt.m, sizeof(float)*16);
			fread(&m_keyDynamicObject[index].frameMatrix[itemp].m[0], 1, sizeof(float)*16, fp);

			i++;
		}

		fclose(fp);

		index++;
	}

	return true;
}


bool SceneData::BoneAnimationDataLoad(void)
{
	FILE    *fp=NULL;
	union {
		char    sChar[256];
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int     i = 0;
	int     primitiveCount = 0;
	struct SubMaterialList *pMaterialList = NULL;
	struct BoneDrawObject *pObject = NULL;
	struct SkinObject *pSkin = NULL;
	struct SkinInfo *pSkinInfo = NULL;
	struct Matrix16f tmpMt;
	int		maxWeightCount = 0;
	int		boneIDs[128];
	
	if(m_Frame.frameCount < 2)
		return true;
		
	////
	strcpy(filename, m_folderName);
	strcat(filename, "bFILE.txt");

	fp = fopen(filename, "r");
	if(!fp)	return true;

	fscanf (fp, "%d", &itemp);
	if(itemp==0) {
		fclose(fp);
		return true;
	}

	m_boneCount = itemp;
	m_bone = (struct BoneObjecct *) malloc (sizeof(struct BoneObjecct)*itemp);	
	memset(m_bone, 0, sizeof(struct BoneObjecct)*itemp);
	
	index = 0;
	do{
		if(index >= m_boneCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_bone[index].sName, sChar);
		fscanf (fp, "%d", &itemp);
		
		if (feof(fp))
			break;

		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_boneCount){
		////
		strcpy(filename, m_folderName);
		strcat(filename, "b_matrix_");
		strcat(filename, m_bone[index].sName);
		strcat(filename, ".txt");

		fp = fopen(filename, "r");
		if(!fp)	return false;

		i = 0;
		m_bone[index].frameMatrix = (struct Matrix16f*) malloc (sizeof(struct Matrix16f)*m_Frame.frameCount);
		memset(m_bone[index].frameMatrix, 0, sizeof(struct Matrix16f)*m_Frame.frameCount);

		while(i < m_Frame.frameCount) {
			fscanf(fp,"%d", &itemp);
			for(int k=0; k<16; k++)
				fscanf(fp,"%f", &tmpMt.m[k]);

			memcpy(m_bone[index].frameMatrix[i].m, tmpMt.m, sizeof(float)*16);

			i++;
		}

		fclose(fp);

		index++;
	}
	

	////
	strcpy(filename, m_folderName);
	strcat(filename, "skin.txt");

	fp = fopen(filename, "r");
	if(!fp)	return false;

	fscanf (fp, "%d", &itemp);
	if(itemp == 0) {
		fclose(fp);
		return true;
	}

	m_boneDynamicObjectCount = itemp;
	m_boneDynamicObject = (struct BoneDynamicObject *) malloc (sizeof(struct BoneDynamicObject)*itemp);
	memset(m_boneDynamicObject, 0, sizeof(struct BoneDynamicObject)*itemp);

	index = 0;
	do{
		if(index >= m_boneDynamicObjectCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_boneDynamicObject[index].sName, sChar);
		fscanf (fp, "%d", &itemp);
		
		if(itemp > 0) {			
			m_boneDynamicObject[index].object.vertexCount   = itemp*3;
			m_boneDynamicObject[index].object.skinIndex     = (int*) malloc (sizeof(int)* itemp*3);
			memset(m_boneDynamicObject[index].object.skinIndex, 0, sizeof(int)* itemp*3);
			m_boneDynamicObject[index].object.vertex        = (struct Pos*) malloc (sizeof(struct Pos)* itemp*3);
			memset(m_boneDynamicObject[index].object.vertex, 0, sizeof(struct Pos)* itemp*3);
			m_boneDynamicObject[index].object.normalVector  = (struct Vec*) malloc (sizeof(struct Vec)* itemp*3);
			memset(m_boneDynamicObject[index].object.normalVector, 0, sizeof(struct Vec)* itemp*3);
			m_boneDynamicObject[index].object.texCoord      = (struct TexC*) malloc (sizeof(struct TexC)* itemp*3);
			memset(m_boneDynamicObject[index].object.texCoord, 0, sizeof(struct TexC)* itemp*3);
		}

		fscanf (fp, "%d", &itemp);
		if(itemp > 0) {	
			m_boneDynamicObject[index].skin.skinCount = itemp;
			m_boneDynamicObject[index].skin.skinInfo  = (struct SkinInfo *) malloc (sizeof(struct SkinInfo) * itemp);
			memset(m_boneDynamicObject[index].skin.skinInfo, 0, sizeof(struct SkinInfo) * itemp);
		}

		if (feof(fp))
			break;

		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_boneDynamicObjectCount){

		if(m_boneDynamicObject[index].object.vertexCount == 0
			&& m_boneDynamicObject[index].skin.skinCount == 0) continue;

		////
		strcpy(filename, m_folderName);
		strcat(filename, "b_KD_prim_pool_");
		strcat(filename, m_boneDynamicObject[index].sName);
		strcat(filename, ".txt");

		fp = fopen(filename, "r");
		if(!fp)	return false;

		i = 0;
		primitiveCount = 0;
		pObject = &m_boneDynamicObject[index].object;

		pObject->pMaterialList = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
		pMaterialList = pObject->pMaterialList;
		memset(pMaterialList, 0, sizeof(struct SubMaterialList));

		while(i < pObject->vertexCount) {
			fscanf (fp, "%d", &itemp);
			pObject->skinIndex[i] = itemp;
			fscanf (fp, "%d", &itemp);
			pObject->skinIndex[i+1] = itemp;
			fscanf (fp, "%d", &itemp);
			pObject->skinIndex[i+2] = itemp;

			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].x = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].y = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].z = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].x = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].y = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].z = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->texCoord[i].u = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->texCoord[i].v = ftemp;
			i++;

			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].x = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].y = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].z = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].x = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].y = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].z = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->texCoord[i].u = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->texCoord[i].v = ftemp;
			i++;

			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].x = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].y = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->vertex[i].z = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].x = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].y = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->normalVector[i].z = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->texCoord[i].u = ftemp;
			fscanf(fp,"%f", &ftemp);
			pObject->texCoord[i].v = ftemp;
			i++;

			fscanf(fp,"%f", &ftemp);
			fscanf(fp,"%d", &itemp);

			if(i == 3) {
				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			} else if (pMaterialList->materialIndex != itemp) {
				pMaterialList->pNext = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
				pMaterialList = pMaterialList->pNext;
				memset(pMaterialList, 0, sizeof(struct SubMaterialList));

				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			}

			primitiveCount++;
			pMaterialList->primitiveCount++;
		}

		fclose(fp);
		
		
		if(m_boneDynamicObject[index].skin.skinCount == 0) continue;
		
		////
		strcpy(filename, m_folderName);
		strcat(filename, m_boneDynamicObject[index].sName);
		strcat(filename, ".txt");

		fp = fopen(filename, "r");
		if(!fp)	return false;

		bool isSame = false;
		float sum = 0.0f;

		for(i=0; i<128; i++)
			boneIDs[i] = -1;

		i = 0;
		maxWeightCount = 0;
		pObject = &m_boneDynamicObject[index].object;
		pSkin = &m_boneDynamicObject[index].skin;

		while(i < pSkin->skinCount) {
			fscanf (fp, "%d", &itemp);
			fscanf (fp, "%d", &itemp);
			pSkin->skinInfo[i].weightCount = itemp;
			for(int j=0; j<10; j++){
				pSkin->skinInfo[i].boneIndex[j] = -1;
				pSkin->skinInfo[i].weight[j] = -1;
			}
			
			sum = 0.0f;
			for(int j=0; j<pSkin->skinInfo[i].weightCount; j++){
				fscanf (fp, "%d", &itemp);
				pSkin->skinInfo[i].boneIndex[j] = itemp;
				fscanf(fp,"%f", &ftemp);
				pSkin->skinInfo[i].weight[j] = ftemp;
				if(j == (pSkin->skinInfo[i].weightCount - 1))
					pSkin->skinInfo[i].weight[j] = 1.0f - sum;
				sum += pSkin->skinInfo[i].weight[j];

				//
				isSame = false;					
				for(int k=0; k<maxWeightCount+1; k++){
					if(boneIDs[k] == -1) {
						if(!isSame) {
							boneIDs[k] = pSkin->skinInfo[i].boneIndex[j];
							maxWeightCount++;
							break;
						}
					} else if(boneIDs[k] == pSkin->skinInfo[i].boneIndex[j]) {
						isSame = true;
						break;
					}
				}
			}

			i++;
		}

		////
		pObject = &m_boneDynamicObject[index].object;
		pObject->useBoneCount = maxWeightCount;
		if(maxWeightCount > 0) {
			pObject->boneIDs   = (int *) malloc (sizeof(int)*maxWeightCount);
			pObject->matrixIndex = (RCuint *) malloc (sizeof(RCuint)*maxWeightCount*pObject->vertexCount);
			pObject->weight      = (float *) malloc (sizeof(float)*maxWeightCount*pObject->vertexCount);
			memset(pObject->matrixIndex, 0, sizeof(RCuint)*maxWeightCount*pObject->vertexCount);
			memset(pObject->weight, 0, sizeof(float)*maxWeightCount*pObject->vertexCount);

			for(i=0; i<maxWeightCount; i++)
				pObject->boneIDs[i] = -1;
			memcpy(pObject->boneIDs, boneIDs, sizeof(int)*maxWeightCount);

			i = 0;
			while(i < pObject->vertexCount) {
				for(int k=0; k<maxWeightCount; k++)
					pObject->matrixIndex[i*maxWeightCount + k] = k;
				i++;
			}

			////
			i = 0;
			pSkin = &m_boneDynamicObject[index].skin;
			int nWeight = 0;
			while(i < pObject->vertexCount) {
				pSkinInfo = &pSkin->skinInfo[pObject->skinIndex[i]];
				nWeight = pSkinInfo->weightCount;
				for(int j=0; j<pObject->useBoneCount; j++){
					for(int k=0; k<nWeight; k++){
						if(pObject->boneIDs[j] == pSkinInfo->boneIndex[k]){
							pObject->weight[i*maxWeightCount + j] = pSkinInfo->weight[k];
						}
					}
				}
				
				i++;
			}
		}

		index++;
	}

	return true;
}


bool SceneData::BoneAnimationDataLoadx(void)
{
	FILE    *fp=NULL;
	union {
		char    sChar[256];
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int     i = 0;
	int     j = 0;
	int     primitiveCount = 0;
	struct SubMaterialList *pMaterialList = NULL;
	struct BoneDrawObject *pObject = NULL;
	struct SkinObject *pSkin = NULL;
	struct SkinInfo *pSkinInfo = NULL;
	//struct Matrix16f tmpMt;
	int		maxWeightCount = 0;
	int		boneIDs[128];

	if(m_Frame.frameCount < 2)
		return true;

	////
	strcpy(filename, m_folderName);
	strcat(filename, "bFILE.txt");

	fp = fopen(filename, "r");
	if(!fp)	return true;

	fscanf (fp, "%d", &itemp);
	if(itemp==0) {
		fclose(fp);
		return true;
	}

	m_boneCount = itemp;
	m_bone = (struct BoneObjecct *) malloc (sizeof(struct BoneObjecct)*itemp);	
	memset(m_bone, 0, sizeof(struct BoneObjecct)*itemp);

	index = 0;
	do{
		if(index >= m_boneCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_bone[index].sName, sChar);
		fscanf (fp, "%d", &itemp);

		if (feof(fp))
			break;

		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_boneCount){
		////
		strcpy(filename, m_folderName);
		strcat(filename, "bx_matrix_");
		strcat(filename, m_bone[index].sName);
		strcat(filename, ".dat");

		fp = fopen(filename, "rb");
		if(!fp)	return false;

		i = 0;
		m_bone[index].frameMatrix = (struct Matrix16f*) malloc (sizeof(struct Matrix16f)*m_Frame.frameCount);
		memset(m_bone[index].frameMatrix, 0, sizeof(struct Matrix16f)*m_Frame.frameCount);

		while(i < m_Frame.frameCount) {
			fread(&itemp, 1, sizeof(int), fp);			

			//fread(&tmpMt.m[0], 1, sizeof(float)*16, fp);
			//memcpy(m_bone[index].frameMatrix[i].m, tmpMt.m, sizeof(float)*16);
			fread(&m_bone[index].frameMatrix[i].m[0], 1, sizeof(float)*16, fp);

			i++;
		}

		fclose(fp);

		index++;
	}


	////
	strcpy(filename, m_folderName);
	strcat(filename, "skin.txt");

	fp = fopen(filename, "r");
	if(!fp)	return false;

	fscanf (fp, "%d", &itemp);
	if(itemp == 0) {
		fclose(fp);
		return true;
	}

	m_boneDynamicObjectCount = itemp;
	m_boneDynamicObject = (struct BoneDynamicObject *) malloc (sizeof(struct BoneDynamicObject)*itemp);
	memset(m_boneDynamicObject, 0, sizeof(struct BoneDynamicObject)*itemp);

	index = 0;
	do{
		if(index >= m_boneDynamicObjectCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_boneDynamicObject[index].sName, sChar);
		fscanf (fp, "%d", &itemp);

		if(itemp > 0) {			
			m_boneDynamicObject[index].object.vertexCount   = itemp*3;
			m_boneDynamicObject[index].object.skinIndex     = (int*) malloc (sizeof(int)* itemp*3);
			memset(m_boneDynamicObject[index].object.skinIndex, 0, sizeof(int)* itemp*3);
			m_boneDynamicObject[index].object.vertex        = (struct Pos*) malloc (sizeof(struct Pos)* itemp*3);
			memset(m_boneDynamicObject[index].object.vertex, 0, sizeof(struct Pos)* itemp*3);
			m_boneDynamicObject[index].object.normalVector  = (struct Vec*) malloc (sizeof(struct Vec)* itemp*3);
			memset(m_boneDynamicObject[index].object.normalVector, 0, sizeof(struct Vec)* itemp*3);
			m_boneDynamicObject[index].object.texCoord      = (struct TexC*) malloc (sizeof(struct TexC)* itemp*3);
			memset(m_boneDynamicObject[index].object.texCoord, 0, sizeof(struct TexC)* itemp*3);
		}

		fscanf (fp, "%d", &itemp);
		if(itemp > 0) {	
			m_boneDynamicObject[index].skin.skinCount = itemp;
			m_boneDynamicObject[index].skin.skinInfo  = (struct SkinInfo *) malloc (sizeof(struct SkinInfo) * itemp);
			memset(m_boneDynamicObject[index].skin.skinInfo, 0, sizeof(struct SkinInfo) * itemp);
		}

		if (feof(fp))
			break;

		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_boneDynamicObjectCount){

		if(m_boneDynamicObject[index].object.vertexCount == 0
			&& m_boneDynamicObject[index].skin.skinCount == 0) continue;

		////
		strcpy(filename, m_folderName);
		strcat(filename, "bx_KD_prim_pool_");
		strcat(filename, m_boneDynamicObject[index].sName);
		strcat(filename, ".dat");

		fp = fopen(filename, "rb");
		if(!fp)	return false;

		i = 0;
		primitiveCount = 0;
		pObject = &m_boneDynamicObject[index].object;

		pObject->pMaterialList = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
		pMaterialList = pObject->pMaterialList;
		memset(pMaterialList, 0, sizeof(struct SubMaterialList));

		while(i < pObject->vertexCount) {
			
			fread(&pObject->skinIndex[i], 1, sizeof(int)*3, fp);
			for(j = 0; j < 3; j++) {				
				fread(&pObject->vertex[i].x, 1, sizeof(float)*3, fp);
				fread(&pObject->normalVector[i].x, 1, sizeof(float)*3, fp);
				fread(&pObject->texCoord[i].u, 1, sizeof(float)*2, fp);
				i++;
			}
			fread(&ftemp, 1, sizeof(float), fp);
			fread(&itemp, 1, sizeof(int), fp);

			if(i == 3) {
				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			} else if (pMaterialList->materialIndex != itemp) {
				pMaterialList->pNext = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
				pMaterialList = pMaterialList->pNext;
				memset(pMaterialList, 0, sizeof(struct SubMaterialList));

				pMaterialList->materialIndex = itemp;
				pMaterialList->startPrimitiveIndex = primitiveCount;
			}

			primitiveCount++;
			pMaterialList->primitiveCount++;
		}

		fclose(fp);


		if(m_boneDynamicObject[index].skin.skinCount == 0) continue;

		////
		strcpy(filename, m_folderName);
		strcat(filename, m_boneDynamicObject[index].sName);
		strcat(filename, ".txt");

		fp = fopen(filename, "r");
		if(!fp)	return false;

		bool isSame = false;
		float sum = 0.0f;

		for(i=0; i<128; i++)
			boneIDs[i] = -1;

		i = 0;
		maxWeightCount = 0;
		pObject = &m_boneDynamicObject[index].object;
		pSkin = &m_boneDynamicObject[index].skin;

		while(i < pSkin->skinCount) {
			fscanf (fp, "%d", &itemp);
			fscanf (fp, "%d", &itemp);
			pSkin->skinInfo[i].weightCount = itemp;
			for(int j=0; j<10; j++){
				pSkin->skinInfo[i].boneIndex[j] = -1;
				pSkin->skinInfo[i].weight[j] = -1;
			}

			sum = 0.0f;
			for(int j=0; j<pSkin->skinInfo[i].weightCount; j++){
				fscanf (fp, "%d", &itemp);
				pSkin->skinInfo[i].boneIndex[j] = itemp;
				fscanf(fp,"%f", &ftemp);
				pSkin->skinInfo[i].weight[j] = ftemp;
				if(j == (pSkin->skinInfo[i].weightCount - 1))
					pSkin->skinInfo[i].weight[j] = 1.0f - sum;
				sum += pSkin->skinInfo[i].weight[j];

				//
				isSame = false;					
				for(int k=0; k<maxWeightCount+1; k++){
					if(boneIDs[k] == -1) {
						if(!isSame) {
							boneIDs[k] = pSkin->skinInfo[i].boneIndex[j];
							maxWeightCount++;
							break;
						}
					} else if(boneIDs[k] == pSkin->skinInfo[i].boneIndex[j]) {
						isSame = true;
						break;
					}
				}
			}

			i++;
		}

		////
		pObject = &m_boneDynamicObject[index].object;
		pObject->useBoneCount = maxWeightCount;
		if(maxWeightCount > 0) {
			pObject->boneIDs   = (int *) malloc (sizeof(int)*maxWeightCount);
			pObject->matrixIndex = (RCuint *) malloc (sizeof(RCuint)*maxWeightCount*pObject->vertexCount);
			pObject->weight      = (float *) malloc (sizeof(float)*maxWeightCount*pObject->vertexCount);
			memset(pObject->matrixIndex, 0, sizeof(RCuint)*maxWeightCount*pObject->vertexCount);
			memset(pObject->weight, 0, sizeof(float)*maxWeightCount*pObject->vertexCount);

			for(i=0; i<maxWeightCount; i++)
				pObject->boneIDs[i] = -1;
			memcpy(pObject->boneIDs, boneIDs, sizeof(int)*maxWeightCount);

			i = 0;
			while(i < pObject->vertexCount) {
				for(int k=0; k<maxWeightCount; k++)
					pObject->matrixIndex[i*maxWeightCount + k] = k;
				i++;
			}

			////
			i = 0;
			pSkin = &m_boneDynamicObject[index].skin;
			int nWeight = 0;
			while(i < pObject->vertexCount) {
				pSkinInfo = &pSkin->skinInfo[pObject->skinIndex[i]];
				nWeight = pSkinInfo->weightCount;
				for(int j=0; j<pObject->useBoneCount; j++){
					for(int k=0; k<nWeight; k++){
						if(pObject->boneIDs[j] == pSkinInfo->boneIndex[k]){
							pObject->weight[i*maxWeightCount + j] = pSkinInfo->weight[k];
						}
					}
				}

				i++;
			}
		}

		index++;
	}

	return true;
}


bool SceneData::MeshAnimationDataLoad(void)
{
	FILE    *fp=NULL;
	union {
		char    sChar[256];
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int		index2 = 0;
	int     i = 0;
	int     primitiveCount = 0;
	int		frameCount = 0;
	struct SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;
	
	if(m_Frame.frameCount < 2)
		return true;
	
	////
	strcpy(filename, m_folderName);
	strcat(filename, "mFILE.txt");

	fp = fopen(filename, "r");
	if(!fp)	return true;

	fscanf (fp, "%d", &itemp);
	if(itemp==0) {
		fclose(fp);
		return true;
	}

	m_meshDynamicObjectCount = itemp;
	m_meshDynamicObject = (struct MeshDynamicObject*) malloc (sizeof(struct MeshDynamicObject)*itemp);
	memset(m_meshDynamicObject, 0, sizeof(struct MeshDynamicObject)*itemp);
	
	index = 0;
	do{
		if(index >= m_meshDynamicObjectCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_meshDynamicObject[index].sName, sChar);
		fscanf (fp, "%d", &itemp);
		m_meshDynamicObject[index].primitiveCount = itemp;
		primitiveCount = itemp;
		fscanf (fp, "%d", &itemp);
		m_meshDynamicObject[index].startFrameIndex = itemp;
		fscanf (fp, "%d", &itemp);
		m_meshDynamicObject[index].frameCount = itemp+1;
		frameCount = itemp+1;

		if(primitiveCount > 0 && frameCount > 1) {
			m_meshDynamicObject[index].object = (struct BaseDrawObject*) malloc (sizeof(struct BaseDrawObject)*frameCount);
			memset(m_meshDynamicObject[index].object, 0, sizeof(struct BaseDrawObject)*frameCount);
			
			i = 0;
			while(i < frameCount) {
				pObject = &m_meshDynamicObject[index].object[i];
				pObject->vertexCount   = primitiveCount*3;
				pObject->vertex        = (struct Pos*) malloc (sizeof(struct Pos)* primitiveCount*3);
				memset(pObject->vertex, 0, sizeof(struct Pos)* primitiveCount*3);
				pObject->normalVector  = (struct Vec*) malloc (sizeof(struct Vec)* primitiveCount*3);
				memset(pObject->normalVector, 0, sizeof(struct Vec)* primitiveCount*3);
				pObject->texCoord      = (struct TexC*) malloc (sizeof(struct TexC)* primitiveCount*3);
				memset(pObject->texCoord, 0, sizeof(struct TexC)* primitiveCount*3);

				i++;
			}
		}

		if (feof(fp))
			break;

		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_meshDynamicObjectCount){

		index2 = 0;
		while(index2 < m_meshDynamicObject[index].frameCount){

			pObject = &m_meshDynamicObject[index].object[index2];

			if(pObject->vertexCount == 0) continue;

			////
			strcpy(filename, m_folderName);
			strcat(filename, "m_KD_prim_pool_");
			strcat(filename, m_meshDynamicObject[index].sName);
			strcat(filename, "_");
			sprintf(sChar, "%d", index2);
			strcat(filename, sChar);
			strcat(filename, ".txt");

			fp = fopen(filename, "r");
			if(!fp)	return false;

			i = 0;
			primitiveCount = 0;

			pObject->pMaterialList = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
			pMaterialList = pObject->pMaterialList;
			memset(pMaterialList, 0, sizeof(struct SubMaterialList));

			while(i < pObject->vertexCount) {

				fscanf(fp,"%f", &pObject->vertex[i].x);
				fscanf(fp,"%f", &pObject->vertex[i].y);
				fscanf(fp,"%f", &pObject->vertex[i].z);
				fscanf(fp,"%f", &pObject->normalVector[i].x);
				fscanf(fp,"%f", &pObject->normalVector[i].y);
				fscanf(fp,"%f", &pObject->normalVector[i].z);
				fscanf(fp,"%f", &pObject->texCoord[i].u);
				fscanf(fp,"%f", &pObject->texCoord[i].v);
				i++;

				fscanf(fp,"%f", &pObject->vertex[i].x);
				fscanf(fp,"%f", &pObject->vertex[i].y);
				fscanf(fp,"%f", &pObject->vertex[i].z);
				fscanf(fp,"%f", &pObject->normalVector[i].x);
				fscanf(fp,"%f", &pObject->normalVector[i].y);
				fscanf(fp,"%f", &pObject->normalVector[i].z);
				fscanf(fp,"%f", &pObject->texCoord[i].u);
				fscanf(fp,"%f", &pObject->texCoord[i].v);
				i++;

				fscanf(fp,"%f", &pObject->vertex[i].x);
				fscanf(fp,"%f", &pObject->vertex[i].y);
				fscanf(fp,"%f", &pObject->vertex[i].z);
				fscanf(fp,"%f", &pObject->normalVector[i].x);
				fscanf(fp,"%f", &pObject->normalVector[i].y);
				fscanf(fp,"%f", &pObject->normalVector[i].z);
				fscanf(fp,"%f", &pObject->texCoord[i].u);
				fscanf(fp,"%f", &pObject->texCoord[i].v);
				i++;

				fscanf(fp,"%f", &ftemp);
				fscanf(fp,"%d", &itemp);

				if(i == 3) {
					pMaterialList->materialIndex = itemp;
					pMaterialList->startPrimitiveIndex = primitiveCount;
				} else if (pMaterialList->materialIndex != itemp) {
					pMaterialList->pNext = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
					pMaterialList = pMaterialList->pNext;
					memset(pMaterialList, 0, sizeof(struct SubMaterialList));

					pMaterialList->materialIndex = itemp;
					pMaterialList->startPrimitiveIndex = primitiveCount;
				}

				primitiveCount++;
				pMaterialList->primitiveCount++;
			}

			fclose(fp);

			index2++;
		}

		index++;
	}

	return true;
}


bool SceneData::MeshAnimationDataLoadx(void)
{
	FILE    *fp=NULL;
	union {
		char    sChar[256];
		int     itemp;
		float   ftemp;
	};
	int     index = 0;
	int		index2 = 0;
	int     i = 0;
	int     j = 0;
	int     primitiveCount = 0;
	int		frameCount = 0;
	struct SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;
	
	if(m_Frame.frameCount < 2)
		return true;
	
	////
	strcpy(filename, m_folderName);
	strcat(filename, "mFILE.txt");

	fp = fopen(filename, "r");
	if(!fp)	return true;

	fscanf (fp, "%d", &itemp);
	if(itemp==0) {
		fclose(fp);
		return true;
	}

	m_meshDynamicObjectCount = itemp;
	m_meshDynamicObject = (struct MeshDynamicObject*) malloc (sizeof(struct MeshDynamicObject)*itemp);
	memset(m_meshDynamicObject, 0, sizeof(struct MeshDynamicObject)*itemp);
	
	index = 0;
	do{
		if(index >= m_meshDynamicObjectCount) break;

		memset(&sChar[0], 0, 256);
		fscanf (fp, "%s", &sChar[0]);
		strcpy(m_meshDynamicObject[index].sName, sChar);
		fscanf (fp, "%d", &itemp);
		m_meshDynamicObject[index].primitiveCount = itemp;
		primitiveCount = itemp;
		fscanf (fp, "%d", &itemp);
		m_meshDynamicObject[index].startFrameIndex = itemp;
		fscanf (fp, "%d", &itemp);
		m_meshDynamicObject[index].frameCount = itemp+1;
		frameCount = itemp+1;

		if(primitiveCount > 0 && frameCount > 1) {
			m_meshDynamicObject[index].object = (struct BaseDrawObject*) malloc (sizeof(struct BaseDrawObject)*frameCount);
			memset(m_meshDynamicObject[index].object, 0, sizeof(struct BaseDrawObject)*frameCount);
			
			i = 0;
			while(i < frameCount) {
				pObject = &m_meshDynamicObject[index].object[i];
				pObject->vertexCount   = primitiveCount*3;
				pObject->vertex        = (struct Pos*) malloc (sizeof(struct Pos)* primitiveCount*3);
				memset(pObject->vertex, 0, sizeof(struct Pos)* primitiveCount*3);
				pObject->normalVector  = (struct Vec*) malloc (sizeof(struct Vec)* primitiveCount*3);
				memset(pObject->normalVector, 0, sizeof(struct Vec)* primitiveCount*3);
				pObject->texCoord      = (struct TexC*) malloc (sizeof(struct TexC)* primitiveCount*3);
				memset(pObject->texCoord, 0, sizeof(struct TexC)* primitiveCount*3);

				i++;
			}
		}

		if (feof(fp))
			break;

		index++;
	}while(1);
	fclose(fp);


	////
	index = 0;

	while(index < m_meshDynamicObjectCount){

		index2 = 0;
		while(index2 < m_meshDynamicObject[index].frameCount){

			pObject = &m_meshDynamicObject[index].object[index2];

			if(pObject->vertexCount == 0) continue;

			////
			strcpy(filename, m_folderName);
			strcat(filename, "mx_KD_prim_pool_");
			strcat(filename, m_meshDynamicObject[index].sName);
			strcat(filename, "_");
			sprintf(sChar, "%d", index2);
			strcat(filename, sChar);
			strcat(filename, ".dat");

			fp = fopen(filename, "rb");
			if(!fp)	return false;

			i = 0;
			primitiveCount = 0;

			pObject->pMaterialList = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
			pMaterialList = pObject->pMaterialList;
			memset(pMaterialList, 0, sizeof(struct SubMaterialList));

			while(i < pObject->vertexCount) {

				for(j = 0; j < 3; j++) {
					fread(&pObject->vertex[i].x, 1, sizeof(float)*3, fp);
					fread(&pObject->normalVector[i].x, 1, sizeof(float)*3, fp);
					fread(&pObject->texCoord[i].u, 1, sizeof(float)*2, fp);
					i++;
				}
				fread(&ftemp, 1, sizeof(float), fp);
				fread(&itemp, 1, sizeof(int), fp);

				if(i == 3) {
					pMaterialList->materialIndex = itemp;
					pMaterialList->startPrimitiveIndex = primitiveCount;
				} else if (pMaterialList->materialIndex != itemp) {
					pMaterialList->pNext = (struct SubMaterialList*) malloc (sizeof(struct SubMaterialList));
					pMaterialList = pMaterialList->pNext;
					memset(pMaterialList, 0, sizeof(struct SubMaterialList));

					pMaterialList->materialIndex = itemp;
					pMaterialList->startPrimitiveIndex = primitiveCount;
				}

				primitiveCount++;
				pMaterialList->primitiveCount++;
			}

			fclose(fp);

			index2++;
		}

		index++;
	}

	return true;
}

void SceneData::FreeList(SubMaterialList *pList)
{
	if(pList == NULL) return;
	if(pList->pNext) FreeList(pList->pNext);
	free(pList);
}

void SceneData::InitScene(int iFrame){
	if(!m_Frame.cameraAniFlag) iFrame	= 0;
	else
	if(iFrame >= m_Frame.frameCount) iFrame	%= m_Frame.frameCount;

    rcClearColor( m_scene.clearColor.r, m_scene.clearColor.g, m_scene.clearColor.b, 1.0f );
	rcViewport(0, 0, m_scene.screenWidth, m_scene.screenHeight);

	rcMatrixMode( RC_PROJECTION );
	rcLoadIdentity();

	rcuPerspective(m_camera[iFrame].fov, (float)m_scene.screenWidth/(float)m_scene.screenHeight, m_camera[iFrame].distance, 10000);

	{
		float zoom	= m_User.zoom.Get();
		float tilt	= m_User.tilt.Get();
		float pan	= m_User.pan.Get();

		struct{float x, y, z;}	v_pos, up;

		v_pos.x	= m_camera[iFrame].pos.x;
		v_pos.y	= m_camera[iFrame].pos.y;
		v_pos.z	= m_camera[iFrame].pos.z;
		up.x	= m_camera[iFrame].up.x;
		up.y	= m_camera[iFrame].up.y;
		up.z	= m_camera[iFrame].up.z;

		if(zoom != 1.f){
			v_pos.x	*= zoom;
			v_pos.y	*= zoom;
			v_pos.z	*= zoom;
		}

		if(pan || tilt){
			float inv;
			// normalize up vector
			inv = 1.f / sqrt(up.x*up.x + up.y*up.y + up.z+up.z);
			up.x *= inv;
			up.y *= inv;
			up.z *= inv;

			if(pan){
				struct{float x, y, z;}	a, c, d;
				float a_len, c_len;
				a.x	= v_pos.x - m_camera[iFrame].look.x;
				a.y	= v_pos.y - m_camera[iFrame].look.y;
				a.z	= v_pos.z - m_camera[iFrame].look.z;
				a_len	= sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
				inv	= 1.f / a_len;
				d.x = a.x * inv;
				d.y = a.y * inv;
				d.z = a.z * inv;

				// cross product
				c.x	= (d.y*up.z - d.z*up.y);
				c.y	= (d.z*up.x - d.x*up.z);
				c.z	= (d.x*up.y - d.y*up.x);

				c_len	= tan(pan) * a_len / sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
				c.x	= a.x + c.x * c_len;
				c.y	= a.y + c.y * c_len;
				c.z	= a.z + c.z * c_len;

				c_len	= a_len / sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
				v_pos.x	= m_camera[iFrame].look.x + c.x * c_len;
				v_pos.y	= m_camera[iFrame].look.y + c.y * c_len;
				v_pos.z	= m_camera[iFrame].look.z + c.z * c_len;
			}

			if(tilt){
				struct{float x, y, z;}	a, c;
				float a_len, c_len, inv;
				a.x	= v_pos.x - m_camera[iFrame].look.x;
				a.y	= v_pos.y - m_camera[iFrame].look.y;
				a.z	= v_pos.z - m_camera[iFrame].look.z;
				a_len	= sqrt(a.x*a.x + a.y*a.y + a.z*a.z);

				c_len	= tan(tilt) * a_len;
				c.x	= a.x + up.x * c_len;
				c.y	= a.y + up.y * c_len;
				c.z	= a.z + up.z * c_len;
				inv	= 1.f / sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
				c.x *= inv;
				c.y *= inv;
				c.z *= inv;

				BOOL bMinus = tilt < 0;
				c_len	= cos(tilt) * a_len;
				if(bMinus){
					up.x = a.x - c.x * c_len;
					up.y = a.y - c.y * c_len;
					up.z = a.z - c.z * c_len;
				}else{
					up.x = c.x * c_len - a.x;
					up.y = c.y * c_len - a.y;
					up.z = c.z * c_len - a.z;
				}
				
				inv	= 1.f / sqrt(up.x*up.x + up.y*up.y + up.z*up.z);
				if(inv > 1000.f){
					up.x	= m_camera[iFrame].up.x;
					up.y	= m_camera[iFrame].up.y;
					up.z	= m_camera[iFrame].up.z;
				}else{
					up.x *= inv;
					up.y *= inv;
					up.z *= inv;
				}

				v_pos.x	= m_camera[iFrame].look.x + c.x * a_len;
				v_pos.y	= m_camera[iFrame].look.y + c.y * a_len;
				v_pos.z	= m_camera[iFrame].look.z + c.z * a_len;
			}
		}

		rcuLookAt(	v_pos.x,    v_pos.y,  v_pos.z,
					m_camera[iFrame].look.x,   m_camera[iFrame].look.y, m_camera[iFrame].look.z, 
					up.x,     up.y,   up.z);
	}
}


void SceneData::InitTexture(int index)
{
	GenerateTexture(&m_texture[index]);
}   

void SceneData::InitTextureNormal(int index)
{
	GenerateTexture(&m_textureNormal[index]);
}   

void SceneData::InitTextureFlatNormal(int index)
{
	GenerateTexture(&m_textureFlatNormal[index]);
}

void SceneData::InitTextureBGSub(int index)
{
	GenerateTexture(&m_textureBGSub[index]);
}

void SceneData::GenerateTexture(TextureObject *pTexObj)
{
	if(pTexObj == NULL) return;
	
	rcGenTextures(1, &pTexObj->bindName);
	rcBindTexture(pTexObj->target, pTexObj->bindName);

	pTexObj->type = RC_RGB;
	if(pTexObj->pImage) {
		if(pTexObj->pImage->type == 3){			
			rcTextureAlpha(0x10);
			rcTexImage2D(pTexObj->target, 0, RC_RGB, pTexObj->pImage->sizeX, pTexObj->pImage->sizeY, 0, RC_RGB, RC_UNSIGNED_BYTE, pTexObj->pImage->data);
			pTexObj->type = RC_RGB;
		}
		else if(pTexObj->pImage->type == 4){
			rcTexImage2D(pTexObj->target, 0, RC_RGBA, pTexObj->pImage->sizeX, pTexObj->pImage->sizeY, 0, RC_RGBA, RC_UNSIGNED_BYTE, pTexObj->pImage->data);
			pTexObj->type = RC_RGBA;
		}

		if(pTexObj->pImage->data)
			delete [] pTexObj->pImage->data;
		pTexObj->pImage->data = NULL;
	}
}

void SceneData::InitMaterials(void)
{
    int i = 0;

    while(i < m_materialCount) {
        InitMaterials(i);
        i++;
    }

}

void SceneData::InitMaterials(int index)
{
	int i = index;
	MaterialObject *pMtlObj = &m_material[i];

	if( i < m_materialCount) {
				
		rcGenMaterials(1, &pMtlObj->bindName);
		rcBindMaterial(pMtlObj->bindName);

		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT,         &pMtlObj->ambient.r);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE,         &pMtlObj->diffuse.r);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR,        &pMtlObj->specular.r);
		rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL,   pMtlObj->level);
		rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS,        pMtlObj->exponent);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFLECTION,       pMtlObj->reflection);
		rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE,    pMtlObj->transmittance);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, pMtlObj->refractionIndex);
		
		// roughness
		//rcMaterialf(RC_FRONT_AND_BACK, RC_ROUGHNESS,		 pMtlObj->roughness);
		// roughness

		if(pMtlObj->texIndex >= 0) {
			rcEnable(RC_TEXTURE_2D);
			rcBindTexture(RC_TEXTURE_2D, m_texture[pMtlObj->texIndex].bindName);
		}
		else
			rcDisable(RC_TEXTURE_2D);

		if((pMtlObj->norIndex) >= 0 && m_textureNormal != NULL) {
			rcEnable(RC_TEXTURE_2D_NORMAL);
			rcBindTexture(RC_TEXTURE_2D_NORMAL, m_textureNormal[pMtlObj->norIndex].bindName);
		}
		else
			rcDisable(RC_TEXTURE_2D_NORMAL);

		rcTexParameteri(RC_TEXTURE_2D, RC_TEXTURE_WRAP_S, (pMtlObj->tex_repeat[0] == 1) ? RC_REPEAT:RC_CLAMP_TO_EDGE);
		rcTexParameteri(RC_TEXTURE_2D, RC_TEXTURE_WRAP_T, (pMtlObj->tex_repeat[1] == 1) ? RC_REPEAT:RC_CLAMP_TO_EDGE);
		rcTexParameteri(RC_TEXTURE_2D_NORMAL, RC_TEXTURE_WRAP_S, (pMtlObj->nor_repeat[0] == 1) ? RC_REPEAT:RC_CLAMP_TO_EDGE);
		rcTexParameteri(RC_TEXTURE_2D_NORMAL, RC_TEXTURE_WRAP_T, (pMtlObj->nor_repeat[1] == 1) ? RC_REPEAT:RC_CLAMP_TO_EDGE);
	}
}

void SceneData::InitLights(void)
{
    int i = 0;

    rcEnable(RC_LIGHTING);

    while(i < m_lightCount) {
		InitLights(i);
        i++;
    }
}

void SceneData::InitLights(int index)
{
	int i = index;
	RCenum lightNumber = RC_LIGHT0 + i;
	
	if(i < m_lightCount) {
		rcEnable(lightNumber);

		switch(m_light[i].type)
		{
		case 0:	case 4:	//Point
			m_light[i].pos.w = 1;
			break;
		case 1:	case 5:	//Directional
			m_light[i].pos.w = 0;
			rcLightfv(lightNumber, RC_SPOT_DIRECTION, &m_light[i].direction.x);
			break;
		case 2:	case 6:	//Spot
			m_light[i].pos.w = 1;
			rcLightfv(lightNumber, RC_SPOT_DIRECTION, &m_light[i].direction.x);
			break;
		}

		/*
		RC_API void RC_APIENTRY rcLightfv (RCenum light, RCenum pname, const RCfloat *params){
			CURRENT_RC_CONTEXT->Lightv (light, pname, params);   
		}
		*/
		rcLightfv(lightNumber, RC_POSITION, &m_light[i].pos.x);		
		rcLightfv(lightNumber, RC_AMBIENT,  &m_light[i].ambient.r);
		rcLightfv(lightNumber, RC_DIFFUSE,  &m_light[i].diffuse.r);
		rcLightfv(lightNumber, RC_SPECULAR, &m_light[i].specular.r);

		//Spot
		if(m_light[i].type == 2 || m_light[i].type == 6) {
			rcLightf (lightNumber, RC_SPOT_EXPONENT,    m_light[i].Spot[2]);
			rcLightf (lightNumber, RC_SPOT_INNER_CONE,  M_180_PI*acos(m_light[i].Spot[0]));
			rcLightf (lightNumber, RC_SPOT_OUTER_CONE,  M_180_PI*acos(m_light[i].Spot[1]));
		}

		//Attenuation
		if(m_light[i].type == 4 || m_light[i].type == 5 || m_light[i].type == 6) {
			rcLightf (lightNumber, RC_START_ATTENUATION,     m_light[i].Atten[3]);
			rcLightf (lightNumber, RC_END_ATTENUATION,       m_light[i].Atten[4]);
			rcLightf (lightNumber, RC_CONSTANT_ATTENUATION,  m_light[i].Atten[0]);
			rcLightf (lightNumber, RC_LINEAR_ATTENUATION,    m_light[i].Atten[1]);
			rcLightf (lightNumber, RC_QUADRATIC_ATTENUATION, m_light[i].Atten[2]);
		}
	}
}

void SceneData::AnimationLights(int iFrame){
	if(!m_Frame.lightAniFlag) return;
	if(iFrame >= m_Frame.frameCount) return;

	for(int i=0;i<m_lightCount;i++) AnimationLights(i, iFrame);
}

void SceneData::AnimationLights(int index, int iFrame)
{
	int i = index;
	RCenum lightNumber = RC_LIGHT0 + i;
	LightAniObject* pAniLight = NULL;

	if(!m_Frame.lightAniFlag) return;
	if(iFrame >= m_Frame.frameCount) return;

	if(i < m_lightCount && m_light[i].frameLight) {		
		
		pAniLight = &m_light[i].frameLight[iFrame];
		switch(m_light[i].type)
		{
		case 0:	case 4:	//Point
			pAniLight->pos.w = 1;
			break;
		case 1:	case 5:	//Directional
			pAniLight->pos.w = 0;
	//		rcLightfv(lightNumber, RC_SPOT_DIRECTION, &pAniLight->direction.x);
			break;
		case 2:	case 6:	//Spot
			pAniLight->pos.w = 1;
	//		rcLightfv(lightNumber, RC_SPOT_DIRECTION, &pAniLight->direction.x);
			break;
		}

		rcLightfv(lightNumber, RC_POSITION, &pAniLight->pos.x);	
	//	rcLightfv(lightNumber, RC_DIFFUSE,  &pAniLight->diffuse.r);
	}
}

void SceneData::DrawBaseObject(struct BaseDrawObject *pObject)
{
	struct SubMaterialList *pMaterialList = NULL;
	int startIndex = 0;
	int vertexCount = 0;
	int materialIndex = 0;
	
	pMaterialList = pObject->pMaterialList;
	while(pMaterialList) {
		startIndex = pMaterialList->startPrimitiveIndex*3;
		vertexCount = pMaterialList->primitiveCount*3;
		materialIndex = pMaterialList->materialIndex;

		
		if(vertexCount == 0)
		{
			pMaterialList = pMaterialList->pNext;
			continue;
		}

		rcVertexPointer(3, RC_FLOAT, sizeof(struct Pos), &pObject->vertex[startIndex]);
		rcNormalPointer(RC_FLOAT, 0, &pObject->normalVector[startIndex]);
		rcTexCoordPointer(2, RC_FLOAT, 0, &pObject->texCoord[startIndex]);

		rcEnable(RC_USE_TRANSMITTANCE_SHADOW);
		if(m_material[materialIndex].texIndex >= 0
			&& m_texture[m_material[materialIndex].texIndex].type == RC_RGBA)
			rcEnable(RC_USE_TEXTURE_ALPHA_SHADOW);
		if(m_material[materialIndex].selfIllum == 1.0)
			rcEnable(RC_USE_TEXTURE_ONLY);

		rcBindMaterial(m_material[materialIndex].bindName);

		rcDrawArrays( RC_TRIANGLES, 0, vertexCount );

		rcDisable(RC_USE_TRANSMITTANCE_SHADOW);
		rcDisable(RC_USE_TEXTURE_ALPHA_SHADOW);
		rcDisable(RC_USE_TEXTURE_ONLY);

		pMaterialList = pMaterialList->pNext;
	}
}

void SceneData::DrawBoneObject(struct BoneDrawObject *pObject)
{
	struct SubMaterialList *pMaterialList = NULL;
	int startIndex = 0;
	int vertexCount = 0;
	int materialIndex = 0;

	pMaterialList = pObject->pMaterialList;
	while(pMaterialList) {
		startIndex = pMaterialList->startPrimitiveIndex*3;
		vertexCount = pMaterialList->primitiveCount*3;
		materialIndex = pMaterialList->materialIndex;

		if(vertexCount == 0)
		{
			pMaterialList = pMaterialList->pNext;
			continue;
		}

		//Bone Animation Info
		rcMatrixIndexPointerOES(pObject->useBoneCount, RC_UNSIGNED_INT, 0, (RCvoid *)&pObject->matrixIndex[startIndex*pObject->useBoneCount]);
		rcWeightPointerOES(pObject->useBoneCount, RC_FLOAT, 0, (RCvoid *)&pObject->weight[startIndex*pObject->useBoneCount]);

		rcVertexPointer(3, RC_FLOAT, sizeof(struct Pos), &pObject->vertex[startIndex]);
		rcNormalPointer(RC_FLOAT, 0, &pObject->normalVector[startIndex]);
		rcTexCoordPointer(2, RC_FLOAT, 0, &pObject->texCoord[startIndex]);

		rcEnable(RC_USE_TRANSMITTANCE_SHADOW);
		if(m_material[materialIndex].texIndex >= 0
			&& m_texture[m_material[materialIndex].texIndex].type == RC_RGBA)
			rcEnable(RC_USE_TEXTURE_ALPHA_SHADOW);
		if(m_material[materialIndex].selfIllum == 1.0)
			rcEnable(RC_USE_TEXTURE_ONLY);

		rcBindMaterial(m_material[materialIndex].bindName);
		rcDrawArrays( RC_TRIANGLES, 0, vertexCount );

		rcDisable(RC_USE_TRANSMITTANCE_SHADOW);
		rcDisable(RC_USE_TEXTURE_ALPHA_SHADOW);
		rcDisable(RC_USE_TEXTURE_ONLY);

		pMaterialList = pMaterialList->pNext;
	}
}

void SceneData::DrawScene(void)
{
    int i = 0;

    rcClear(RC_COLOR_BUFFER_BIT | RC_DEPTH_BUFFER_BIT);

    rcMatrixMode( RC_MODELVIEW );
	rcLoadIdentity();

	rcEnableClientState(RC_VERTEX_ARRAY);
    rcEnableClientState(RC_NORMAL_ARRAY);
	rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

	while(i < m_objectCount) {
		DrawBaseObject(&m_object[i]);
        i++;
    }
		
    rcDisableClientState(RC_VERTEX_ARRAY);
    rcDisableClientState(RC_NORMAL_ARRAY);
	rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
}

void SceneData::DrawScene(int i)
{
	if(i < m_objectCount) {
		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();

		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_NORMAL_ARRAY);
		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		DrawBaseObject(&m_object[i]);
		
		rcDisableClientState(RC_VERTEX_ARRAY);
		rcDisableClientState(RC_NORMAL_ARRAY);
		rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
	}
}

void SceneData::KeyAnimationDrawScene(int iFrame)
{
	int i = 0;
	SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;

	if(iFrame >= m_Frame.frameCount) return;

	rcClear(RC_COLOR_BUFFER_BIT | RC_DEPTH_BUFFER_BIT);

	rcMatrixMode( RC_MODELVIEW );
	rcLoadIdentity();

	rcEnableClientState(RC_VERTEX_ARRAY);
	rcEnableClientState(RC_NORMAL_ARRAY);
	rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

	while(i < m_keyDynamicObjectCount) {
		rcLoadIdentity();
		rcLoadMatrixf(m_keyDynamicObject[i].frameMatrix[iFrame].m);
		DrawBaseObject(&m_keyDynamicObject[i].object);
		i++;
	}

	rcDisableClientState(RC_VERTEX_ARRAY);
	rcDisableClientState(RC_NORMAL_ARRAY);
	rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
}

void SceneData::KeyAnimationDrawScene(int i, int iFrame)
{
	if(iFrame >= m_Frame.frameCount) return;

	if(i < m_objectCount) {
		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();

		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_NORMAL_ARRAY);
		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);
		
		rcLoadMatrixf(m_keyDynamicObject[i].frameMatrix[iFrame].m);
		DrawBaseObject(&m_keyDynamicObject[i].object);

		rcDisableClientState(RC_VERTEX_ARRAY);
		rcDisableClientState(RC_NORMAL_ARRAY);
		rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
	}
}

void SceneData::BoneAnimationDrawScene(int iFrame)
{
	int i = 0;
	struct BoneDrawObject *pObject = NULL;

	if(iFrame >= m_Frame.frameCount) return;

	rcClear(RC_COLOR_BUFFER_BIT | RC_DEPTH_BUFFER_BIT);

	rcMatrixMode( RC_MODELVIEW );
	rcLoadIdentity();

	rcEnable(RC_MATRIX_PALETTE_OES);

	rcEnableClientState(RC_WEIGHT_ARRAY_OES);
	rcEnableClientState(RC_MATRIX_INDEX_ARRAY_OES);
	rcEnableClientState(RC_VERTEX_ARRAY);
	rcEnableClientState(RC_NORMAL_ARRAY);
	rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

	while(i < m_boneDynamicObjectCount) {
		
		pObject = &m_boneDynamicObject[i].object;

		//Set the Matrix Palette
		rcMatrixMode( RC_MATRIX_PALETTE_OES );
		for(int k=0; k<pObject->useBoneCount; k++){
			rcCurrentPaletteMatrixOES(k);
			rcLoadMatrixf(m_bone[pObject->boneIDs[k]].frameMatrix[iFrame].m);
		}

		rcMatrixMode(RC_MODELVIEW);
		rcLoadIdentity();
		
		DrawBoneObject(pObject);
		i++;
	}

	rcDisableClientState(RC_VERTEX_ARRAY);
	rcDisableClientState(RC_NORMAL_ARRAY);
	rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
	rcDisableClientState(RC_WEIGHT_ARRAY_OES);
	rcDisableClientState(RC_MATRIX_INDEX_ARRAY_OES);

	rcDisable(RC_MATRIX_PALETTE_OES);
}

void SceneData::BoneAnimationDrawScene(int i, int iFrame)
{
	struct BoneDrawObject *pObject = NULL;

	if(iFrame >= m_Frame.frameCount) return;

	if(i < m_objectCount) {
		pObject = &m_boneDynamicObject[i].object;

		//Set the Matrix Palette
		rcMatrixMode( RC_MATRIX_PALETTE_OES );
		for(int k=0; k<pObject->useBoneCount; k++){
			rcCurrentPaletteMatrixOES(k);
			rcLoadMatrixf(m_bone[pObject->boneIDs[k]].frameMatrix[iFrame].m);
		}

		rcMatrixMode(RC_MODELVIEW);
		rcLoadIdentity();

		rcEnable(RC_MATRIX_PALETTE_OES);

		rcEnableClientState(RC_WEIGHT_ARRAY_OES);
		rcEnableClientState(RC_MATRIX_INDEX_ARRAY_OES);
		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_NORMAL_ARRAY);
		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		DrawBoneObject(pObject);

		rcDisableClientState(RC_VERTEX_ARRAY);
		rcDisableClientState(RC_NORMAL_ARRAY);
		rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
		rcDisableClientState(RC_WEIGHT_ARRAY_OES);
		rcDisableClientState(RC_MATRIX_INDEX_ARRAY_OES);

		rcDisable(RC_MATRIX_PALETTE_OES);
	}
}

void SceneData::MeshAnimationDrawScene(int iFrame)
{
	int i = 0;
	SubMaterialList *pMaterialList = NULL;
	struct BaseDrawObject *pObject = NULL;

	if(iFrame >= m_Frame.frameCount) return;

	rcClear(RC_COLOR_BUFFER_BIT | RC_DEPTH_BUFFER_BIT);

	rcMatrixMode( RC_MODELVIEW );
	rcLoadIdentity();

	rcEnableClientState(RC_VERTEX_ARRAY);
	rcEnableClientState(RC_NORMAL_ARRAY);
	rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

	while(i < m_meshDynamicObjectCount) {		
		if(iFrame < m_meshDynamicObject[i].startFrameIndex)
			DrawBaseObject(&m_meshDynamicObject[i].object[0]);
		else if(iFrame >= m_meshDynamicObject[i].startFrameIndex
			&& iFrame < m_meshDynamicObject[i].startFrameIndex + m_meshDynamicObject[i].frameCount)
			DrawBaseObject(&m_meshDynamicObject[i].object[iFrame - m_meshDynamicObject[i].startFrameIndex]);
		else if(iFrame < m_meshDynamicObject[i].startFrameIndex + m_meshDynamicObject[i].frameCount)
			DrawBaseObject(&m_meshDynamicObject[i].object[m_meshDynamicObject[i].frameCount]);
		
		i++;
	}

	rcDisableClientState(RC_VERTEX_ARRAY);
	rcDisableClientState(RC_NORMAL_ARRAY);
	rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
}

void SceneData::MeshAnimationDrawScene(int i, int iFrame)
{
	if(iFrame >= m_Frame.frameCount) return;

	if(i < m_objectCount) {
		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();

		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_NORMAL_ARRAY);
		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		if(iFrame < m_meshDynamicObject[i].startFrameIndex)
			DrawBaseObject(&m_meshDynamicObject[i].object[0]);
		else if(iFrame >= m_meshDynamicObject[i].startFrameIndex
			&& iFrame < m_meshDynamicObject[i].startFrameIndex + m_meshDynamicObject[i].frameCount)
			DrawBaseObject(&m_meshDynamicObject[i].object[iFrame - m_meshDynamicObject[i].startFrameIndex]);
		else if(iFrame < m_meshDynamicObject[i].startFrameIndex + m_meshDynamicObject[i].frameCount)
			DrawBaseObject(&m_meshDynamicObject[i].object[m_meshDynamicObject[i].frameCount]);

		rcDisableClientState(RC_VERTEX_ARRAY);
		rcDisableClientState(RC_NORMAL_ARRAY);
		rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
	}
}

void SceneData::OnKeyDown(char cKey)
{
	switch(cKey){
	case 'B':	//Change Background Texture
		{
			if(m_materialIndexBG >= 0 && m_textureBGSubCount > 0) {
				m_textureBGSubIndex++;
				if(m_textureBGSubIndex == m_textureBGSubCount)
					m_textureBGSubIndex = -1;
			}
			RefreshBGMaterial();
		}
		break;
	case 'N':	//Change Material Normal Map
		{
			m_bFlatNormal = !m_bFlatNormal;
			RefreshNormalmaps();
		}
		break;
	case 'I':	//Initialize
		{
			for(int i=0; i<m_lightCount; i++){
				rcEnable(RC_LIGHT0 + i);
			}

			m_enableLightCount       = m_lightCount;
			m_selectMaterialIndex    = -1;
			m_reflectionWeight       = 0.0f;
			m_transmittanceWeight    = 0.0f;
			m_refractionWeight       = 0.0f;
			ResetMaterials();

			//
			m_textureBGSubIndex = -1;
			m_bFlatNormal = FALSE;
			RefreshBGMaterial();
			RefreshNormalmaps();
		}
		break;
	case 'O':	//Each Light On In Order
		{
			if(m_enableLightCount < m_lightCount){
				rcEnable(RC_LIGHT0 + m_enableLightCount);
				m_enableLightCount++;
			}
		}
		break;
	case 'L':	//Each Light Off In Order
		{
			if(m_enableLightCount > 0){
				m_enableLightCount--;
				if(m_enableLightCount < m_lightCount)
					rcDisable(RC_LIGHT0 + m_enableLightCount);
			}
		}
		break;
	case 'M':	//Initialize Material Property
		{
			m_reflectionWeight       = 0.0f;
			m_transmittanceWeight    = 0.0f;
			m_refractionWeight       = 0.0f;
			ResetMaterials();
		}
		break;
	case 'R':	//Material Reflection Increase
		{
			m_reflectionWeight += 0.2f;
			if(m_reflectionWeight > 1.0f)
				m_reflectionWeight = 1.0f;

			UpdateMaterialReflection(m_selectMaterialIndex);
		}	
		break;
	case 'F':	//Material Reflection Decrease
		{
			m_reflectionWeight -= 0.2f;
			if(m_reflectionWeight < -1.0f)
				m_reflectionWeight = -1.0f;

			UpdateMaterialReflection(m_selectMaterialIndex);
		}		
		break;
	case 'T':	//Material Transmittance Increase
		{
			m_transmittanceWeight += 0.2f;
			if(m_transmittanceWeight > 1.0f)
				m_transmittanceWeight = 1.0f;

			UpdateMaterialTransmittance(m_selectMaterialIndex);
		}	
		break;
	case 'G':	//Material Transmittance Decrease
		{
			m_transmittanceWeight -= 0.2f;
			if(m_transmittanceWeight < -1.0f)
				m_transmittanceWeight = -1.0f;

			UpdateMaterialTransmittance(m_selectMaterialIndex);
		}		
		break;
	case 'Y':	//Material Refraction Increase
		{
			m_refractionWeight += 0.2f;
			if(m_refractionWeight > 2.0f)
				m_refractionWeight = 2.0f;

			UpdateMaterialRefraction(m_selectMaterialIndex);
		}
		break;
	case 'H':	//Material Refraction Decrease
		{
			m_refractionWeight -= 0.2f;
			if(m_refractionWeight < -2.0f)
				m_refractionWeight = -2.0f;

			UpdateMaterialRefraction(m_selectMaterialIndex);
		}
		break;
	}
}

void SceneData::OnMouseEvent(float x, float y, DWORD dwBtn){
	{
		static BOOL bBtnDown	= FALSE;
		static float prev_y		= 0;
		static float prev_x		= 0;
		if(!(dwBtn & MOUSE_BTN_LEFT)){
			m_User.pan.Set(0.f);
			m_User.tilt.Set(0.f);
			bBtnDown	= FALSE;
		}else{
			if(bBtnDown){
				m_User.pan.Add((x-prev_x) * 2.f);
				m_User.tilt.Add((y-prev_y) * 2.f);
			}
			prev_x		= x;
			prev_y		= y;
			bBtnDown	= TRUE;
		}
	}
	{
		static BOOL bBtnDown	= FALSE;
		static float prev_y		= 0;
		if(!(dwBtn & MOUSE_BTN_RIGHT)){
			m_User.zoom.Set(1.f);
			bBtnDown	= FALSE;
		}else{
			if(bBtnDown && !(dwBtn & MOUSE_BTN_LEFT)){
				m_User.zoom.Add((y-prev_y) * 2.f);
			}
			prev_y		= y;
			bBtnDown	= TRUE;
		}
	}
}

void SceneData::RefreshBGMaterial() {
	if(m_material == NULL) return;
	if(m_texture == NULL) return;
	if(m_materialIndexBG < 0) return;
	if(m_textureBGSub == NULL) return;

	MaterialObject *pMtlObj = &m_material[m_materialIndexBG];
	int curMaterialName = 0;

	rcGetIntegerv(RC_MATERIAL_BINDING, &curMaterialName);
	rcBindMaterial(pMtlObj->bindName);
	if(pMtlObj->texIndex >= 0) {
		rcEnable(RC_TEXTURE_2D);

		if(m_textureBGSubIndex < 0)
			rcBindTexture(RC_TEXTURE_2D, m_texture[pMtlObj->texIndex].bindName);
		else if(m_textureBGSubCount > 0)
			rcBindTexture(RC_TEXTURE_2D, m_textureBGSub[m_textureBGSubIndex].bindName);
	}
	rcBindMaterial(curMaterialName);
}

void SceneData::RefreshNormalmaps() {
	if(m_material == NULL) return;
	if(m_textureNormal == NULL) return;
	if(m_textureFlatNormalCount == 0) return;
	if(m_textureFlatNormalCount != m_textureNormalCount) return;	
	if(m_textureFlatNormal == NULL) return;

	int i = 0;
	int curMaterialName = 0;
	MaterialObject *pMtlObj = NULL;

	rcGetIntegerv(RC_MATERIAL_BINDING, &curMaterialName);
	for(int i=0; i<m_materialCount; i++){
		pMtlObj = &m_material[i];
		rcBindMaterial(pMtlObj->bindName);
		if((pMtlObj->norIndex) >= 0) {
			if(!m_textureFlatNormal[pMtlObj->norIndex].nouse) {
				if(m_bFlatNormal) 
					rcBindTexture(RC_TEXTURE_2D_NORMAL, m_textureFlatNormal[pMtlObj->norIndex].bindName);
				else
					rcBindTexture(RC_TEXTURE_2D_NORMAL, m_textureNormal[pMtlObj->norIndex].bindName);
			}
		}
	}
	rcBindMaterial(curMaterialName);
}

void SceneData::ResetMaterials()
{
	int i = 0;

	while(i < m_materialCount) {
		rcBindMaterial(m_material[i].bindName);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFLECTION,       m_material[i].reflection);
		rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE,    m_material[i].transmittance);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, m_material[i].refractionIndex);

		i++;
	}

	rcBindMaterial(0);
}

void SceneData::UpdateMaterialReflection(int index)
{
	if(index >= 0 && index < m_materialCount){
		float reflection;
		reflection	= m_material[index].reflection + m_reflectionWeight;

		if(reflection < 0.0f)	reflection = 0.0f;
		if(reflection > 1.0f)	reflection = 1.0f;
		
		rcBindMaterial(m_material[index].bindName);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFLECTION,       reflection);
		rcBindMaterial(0);
	}
}

void SceneData::UpdateMaterialTransmittance(int index)
{
	if(index >= 0 && index < m_materialCount){
		float transmittance = m_material[index].transmittance + m_transmittanceWeight;

		if(transmittance < 0.0f)	transmittance = 0.0f;
		if(transmittance > 1.0f)	transmittance = 1.0f;

		rcBindMaterial(m_material[index].bindName);
		rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE,    transmittance);
		rcBindMaterial(0);
	}
}

void SceneData::UpdateMaterialRefraction(int index)
{
	if(index >= 0 && index < m_materialCount){
		float refraction=m_material[index].refractionIndex;

		refraction += m_refractionWeight;
		if(refraction < 0.0f) refraction = 0.0f;
		if(refraction > 2.0f) refraction = 2.0f;

		rcBindMaterial(m_material[m_selectMaterialIndex].bindName);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, refraction);
		rcBindMaterial(0);
	}
}
