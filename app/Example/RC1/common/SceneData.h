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
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "RCFramework.h"

struct Color {
	float r, g, b;
};

struct Pos {
	float x, y, z, w;
};

struct Vec {
	float x, y, z;
};

struct TexC {
	float u, v;
};

struct Matrix16f {
	float	m[16];
};

struct SubMaterialList
{
	int             startPrimitiveIndex;
	int             primitiveCount;
	int             materialIndex;
	SubMaterialList *pPrev;
	SubMaterialList *pNext;
};

struct BaseDrawObject
{
    int			           vertexCount;
	struct SubMaterialList *pMaterialList;
    struct Pos             *vertex;
    struct Vec             *normalVector;
    struct TexC            *texCoord;
};

struct MaterialObject
{
	RCuint	     bindName;
    int          texIndex;
	int          tex_repeat[2];
	int          norIndex;
	int          nor_repeat[2];
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
	float        level;
    float        exponent;
    float        reflection;
    float        transmittance;
    float        refractionIndex;
    float        selfIllum;

	// roughness
	float		 roughness;
	// roughness
};

struct LightAniObject
{
	struct Pos   pos;
	struct Vec   direction;
	struct Color diffuse;
};

struct LightObject
{
    struct Pos   pos;
    struct Vec   direction;
    float        Spot[3];
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
    float        Atten[5];

    unsigned int type;

	struct LightAniObject* frameLight;
};

typedef struct RGBImageRec {
	unsigned int type;
	int sizeX, sizeY;
	unsigned char *data;
} RGBImageRec;

typedef struct TextureObject {
	int nouse;			// 0 : use, 1 : no use
    RCuint bindName;
	RCenum target;
	RCuint type;		// RGB or RGBA
    RGBImageRec *pImage;
} TextureObject;

struct SceneObject
{
    RCint           screenWidth;
    RCint           screenHeight;
    
    struct Color   clearColor;
};

struct CameraObject
{
    struct Pos  pos;
    struct Pos  look;
    struct Vec  up;
    RCfloat     distance;
    RCfloat     width;
    RCfloat     left, right, bottom, top;
    RCfloat     fov, aspect;

};

struct KeyDynamicObject
{
	char                  sName[256];
	struct BaseDrawObject  object;
	struct Matrix16f      *frameMatrix;
};

struct FrameObject
{
	int frameCount;
	int cameraAniFlag;
	int lightAniFlag;
};

struct BoneObjecct
{
	char              sName[256];
	struct Matrix16f  *frameMatrix;
};

struct SkinInfo {
	int   weightCount;
	int   boneIndex[10];
	float weight[10];
};

struct SkinObject
{
	int              skinCount;
	struct SkinInfo  *skinInfo;
};

struct BoneDrawObject
{
	int			           vertexCount;
	struct SubMaterialList *pMaterialList;
	int                    *skinIndex;
	struct Pos             *vertex;
	struct Vec             *normalVector;
	struct TexC            *texCoord;

	int				useBoneCount;
	int				*boneIDs;
	RCuint			*matrixIndex;
	float			*weight;
};

struct BoneDynamicObject
{
	char		            sName[256];
	struct BoneDrawObject   object;
	struct SkinObject		skin;
};

struct MeshDynamicObject
{
	char		            sName[256];
	int						primitiveCount;
	int						startFrameIndex;
	int						frameCount;
	struct BaseDrawObject	*object;
};

class MagneticFloat{
public:
	MagneticFloat(void){
		Initialize(0.f, 0.f, 1.f);
		m_fAcceleration	= 0.3f;
	}
	~MagneticFloat(void){}
	void Initialize(float fInitialValue, float fMin, float fMax){
		m_fData			=
		m_fDestination	= fInitialValue;
		m_fMin			= fMin;
		m_fMax			= fMax;
		Set(fInitialValue);
	}
	float Get(void){
		if(!m_bLocked){
			m_bLocked	= (m_fData == m_fDestination);
			m_fData		+= (m_fDestination - m_fData)*m_fAcceleration;
		}
		return m_fData;
	}
	inline float GetDest(void)		{return m_fDestination;}
	inline void Set(float fData){
		if(fData<m_fMin) fData	= m_fMin;
		else
		if(fData>m_fMax) fData	= m_fMax;
		m_fDestination	= fData;
		m_bLocked		= (m_fData == m_fDestination);
	}
	inline void Add(float fData){
		Set(m_fDestination + fData);
	}
	inline void SetAcceleration(float fAccel)	{m_fAcceleration	= fAccel;}
	inline BOOL IsLocked(void)		{return m_bLocked;}

private:
	float	m_fMin, m_fMax;
	float	m_fData;
	float	m_fDestination;
	float	m_fAcceleration;
	BOOL	m_bLocked;
};

class SceneData
{
public:
    SceneData(){
		memset(m_folderName, 0, 1024);
		memset(filename, 0, 1024);
		m_scene.screenWidth = 800;
		m_scene.screenHeight = 480;
		memset(&m_scene.clearColor, 0, sizeof(m_scene.clearColor));
		memset(&m_Frame, 0, sizeof(FrameObject));

        m_cameraCount         = 0;
        m_objectCount         = 0;
        m_materialCount       = 0;
        m_lightCount          = 0;
        m_textureCount           = 0;
		m_textureNormalCount     = 0;
		m_textureFlatNormalCount = 0;
		m_textureBGSubCount      = 0;
		m_materialIndexBG        = -1;
		m_textureBGSubIndex	     = -1;
		m_bFlatNormal            = FALSE;
		m_keyDynamicObjectCount  = 0;
		m_boneCount			     = 0;
		m_boneDynamicObjectCount = 0;
		m_meshDynamicObjectCount = 0;

		m_enableLightCount       = 0;
		m_selectMaterialIndex    = -1;
		m_reflectionWeight       = 0.0f;
		m_transmittanceWeight    = 0.0f;
		m_refractionWeight       = 0.0f;

        m_camera            = NULL;
        m_texture           = NULL;
		m_textureNormal     = NULL;
		m_textureFlatNormal = NULL;
		m_textureBGSub      = NULL;
        m_light         = NULL;
        m_material      = NULL;
        m_object        = NULL;
		m_keyDynamicObject  = NULL;
		m_bone              = NULL;
		m_boneDynamicObject = NULL;
		m_meshDynamicObject = NULL;

#define PI_DIV_2	(3.141592f/2.f)
		m_User.zoom.Initialize(1.f, 0.02f, 10.f);
		m_User.pan.Initialize(0.f, -PI_DIV_2, PI_DIV_2);
		m_User.tilt.Initialize(0.f, -PI_DIV_2, PI_DIV_2);
    }

    ~SceneData(void);
	void SetFolderName(char* folderName) { strcpy(m_folderName, folderName); }
	void FreeList(SubMaterialList *pList);
    bool SceneDataLoad(int screenWidth, int screenHeight);
	bool SceneDataLoadx(int screenWidth, int screenHeight);
	bool KeyAnimationDataLoad(void);
	bool KeyAnimationDataLoadx(void);
	bool BoneAnimationDataLoad(void);
	bool BoneAnimationDataLoadx(void);
	bool MeshAnimationDataLoad(void);
	bool MeshAnimationDataLoadx(void);

	char						m_folderName[1024];
    char                        filename[1024];

    int                         m_cameraCount;
    int                         m_objectCount;
    int                         m_materialCount;
    int                         m_lightCount;
    int                         m_textureCount;
	int	                        m_textureNormalCount;
	int	                        m_textureFlatNormalCount;
	int							m_textureBGSubCount;
	int							m_materialIndexBG;
	int							m_textureBGSubIndex;
	BOOL						m_bFlatNormal;
	
	int                         m_keyDynamicObjectCount;
	int                         m_boneCount;
	int                         m_boneDynamicObjectCount;
	int                         m_meshDynamicObjectCount;

	int							m_enableLightCount;
	int							m_selectMaterialIndex;
	float						m_reflectionWeight;
	float						m_transmittanceWeight;
	float						m_refractionWeight;

	struct SceneObject          m_scene;
    struct CameraObject         *m_camera;
	struct FrameObject			m_Frame;

    TextureObject               *m_texture;
	TextureObject               *m_textureNormal;
	TextureObject               *m_textureFlatNormal;
	TextureObject               *m_textureBGSub;
    struct LightObject          *m_light;
    struct MaterialObject       *m_material;
    struct BaseDrawObject       *m_object;
	struct KeyDynamicObject     *m_keyDynamicObject;
	struct BoneObjecct			*m_bone;
	struct BoneDynamicObject	*m_boneDynamicObject;
	struct MeshDynamicObject	*m_meshDynamicObject;

	struct{
		MagneticFloat		zoom;
		MagneticFloat		tilt;
		MagneticFloat		pan;
	}m_User;

    void InitScene(int iFrame = 0);
	void DrawBaseObject(struct BaseDrawObject *pObject);
	void DrawBoneObject(struct BoneDrawObject *pObject);	
	void DrawScene(void);
    void DrawScene(int i);

	void KeyAnimationDrawScene(int iFrame);
	void KeyAnimationDrawScene(int i, int iFrame);
	void BoneAnimationDrawScene(int iFrame);
	void BoneAnimationDrawScene(int i, int iFrame);
	void MeshAnimationDrawScene(int iFrame);
	void MeshAnimationDrawScene(int i, int iFrame);
    void InitMaterials(void);
    void InitMaterials(int index);
    void InitLights(void);
    void InitLights(int index);
	void AnimationLights(int iFrame);
	void AnimationLights(int index, int iFrame);
	void InitTexture(int index);
	void InitTextureNormal(int index);
	void InitTextureFlatNormal(int index);
	void InitTextureBGSub(int index);
	void GenerateTexture(TextureObject *pTexObj);

	RCint ScreenWidth() {return m_scene.screenWidth;}
	RCint ScreenHeight() {return m_scene.screenHeight;}

	RCint FrameCount() {return m_Frame.frameCount;}
	RCint CameraAniFlag() {return m_Frame.cameraAniFlag;}
	RCint LightAniFlag() {return m_Frame.lightAniFlag;}
	
	void SelectMaterial(int index) {m_selectMaterialIndex=index;}
	void OnKeyDown(char cKey);
	void OnMouseEvent(float x, float y, DWORD dwBtn = MOUSE_BTN_NULL);
	void RefreshBGMaterial();
	void RefreshNormalmaps();
	void ResetMaterials();
	void UpdateMaterialReflection(int index);
	void UpdateMaterialTransmittance(int index);
	void UpdateMaterialRefraction(int index);

private:
};

