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
// DATE    : 8/28/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "StdAfx.h"
#include "Emulation.h"
#include "EmulSystem.h"
#include <stdio.h>
#include <assert.h>
#include "Registers.h"
#include "rc_util.h"

Emulation::Emulation(void){
	m_bReset		= FALSE;
}

Emulation::~Emulation(void){
}

BOOL Emulation::DoInit(void){
	return TRUE;
}

BOOL Emulation::DoWork(void){
	Lock();
	// do something...

	return TRUE;
}

void Emulation::DoReset(void){
	m_bReset	= TRUE;
	UnLock();
}

DWORD Emulation::RegRead(DWORD Offset32){
	// do read
	return 0;
}

void Emulation::RegWrite(DWORD Offset32, DWORD Data){
	switch(Offset32){
	case 0:
		break;
	case 1:
		break;
	default:
		break;
	}
}

void Emulation::RegReads(DWORD Offset32, DWORD* pData, int iCount){
	switch(Offset32<<2){
	case RC_REG_OFFSET_INFO:
		break;
	case RC_REG_OFFSET_REPORT:
		pData[1]	= 0x1000;
		for(int i=1;i<iCount;i++)
			pData[i]	= 1;
		break;
	default:
		if(Offset32 >= (RC_REG_OFFSET_CONTROL>>2) && Offset32 < (RC_REG_OFFSET_CONTROL>>2) + REG_CONTROL_SIZE){
			int offset32	= (RC_REG_OFFSET_CONTROL>>2) + (0x3C>>2) - Offset32;
			pData[offset32]	|= 0x4;	// 강제로 end 반환
			break;
		}
		//assert(0);
		return;
	}
}

void Emulation::RegWrites(DWORD Offset32, const DWORD* pData, int iCount){
	int iStride;
	for(;iCount>0;){
		switch(Offset32<<2){
		case RC_REG_OFFSET_CONFIG:
			iStride		= REG_CONFIG_SIZE;
			assert(iCount == iStride);
			memcpy(&g_reg.config, pData, iCount * sizeof(DWORD));
			{
				REG_CONFIG*		pReg			= (REG_CONFIG*)pData;
// 				m_Config.base.node				= (PVOID)GetPointer(pReg->base.node);
// 				m_Config.base.primitive			= (PVOID)GetPointer(pReg->base.primitive);
// 				m_Config.base.list				= (PVOID)GetPointer(pReg->base.list);
// 				m_Config.base.transmit			= (PVOID)GetPointer(pReg->base.transmit);
// 				m_Config.base.primitive_info	= (PVOID)GetPointer(pReg->base.primitive_info);
// 				m_Config.base.texture			= (PVOID)GetPointer(pReg->base.texture);
// 				m_Config.base.framebuffer_0		= (PVOID)GetPointer(pReg->base.framebuffer_0);
// 				m_Config.base.framebuffer_1		= (PVOID)GetPointer(pReg->base.framebuffer_1);

				// set back-buffer
				g_pEmulSystem->Write(REG_DISPLAY_ADDRESS_BACK, pReg->base.framebuffer_0);

				g_mem.pTransmittance			= (RC_TRANSMITTANCE_DATA*)GetPointer(pReg->base.transmit);
				g_mem.pList						= (DDK_LIST*)GetPointer(pReg->base.list);
				g_mem.pNode						= (DDK_NODE*)GetPointer(pReg->base.node);
				g_mem.pPrimitive				= (DDK_PRIMITIVE*)GetPointer(pReg->base.primitive);
				g_mem.pPrimitiveInfo			= (DDK_PRIMITIVE_INFO*)GetPointer(pReg->base.primitive_info);
				g_mem.pTexture					= (BYTE*)GetPointer(pReg->base.texture);
				// 포인터 얻기
				//g_pNode					= (DDK_NODE*)GetPointer(pReg->node_base_addr);
				//kd_list					= (LIST*)GetPointer(pReg->list_base_addr);
			}
			break;
		case RC_REG_OFFSET_CONTROL:
			iStride		= REG_CONTROL_SIZE;
			assert(iCount == iStride);
			memcpy(&g_reg.control, pData, iCount * sizeof(DWORD));
			{
				REG_CONTROL*	pReg			= (REG_CONTROL*)pData;
// 				m_Control.resolution.width		= pReg->resolution.width;
// 				m_Control.resolution.height		= pReg->resolution.height;
// 				m_Control.pixel_size.x			= f24f32(pReg->pixel_size.x);
// 				m_Control.pixel_size.y			= f24f32(pReg->pixel_size.y);
// 				m_Control.start.x				= f24f32(pReg->start.x);
// 				m_Control.start.y				= f24f32(pReg->start.y);
// 				m_Control.background.r			= f24f32(pReg->background.r);
// 				m_Control.background.g			= f24f32(pReg->background.g);
// 				m_Control.background.b			= f24f32(pReg->background.b);
// 				m_Control.fb_stride				= pReg->fb_stride;

				RegWrite(REG_DISPLAY_WIDTH, pReg->resolution.width);
				RegWrite(REG_DISPLAY_HEIGHT, pReg->resolution.height);
				RegWrite(REG_DISPLAY_BYTE_STRIDE, pReg->fb_stride * sizeof(DWORD));
			}
			RenderScene();	// 렌더링 시작
			break;
		case RC_REG_OFFSET_OPTION:
			iStride		= REG_OPTION_SIZE;
			assert(iCount == iStride);
			memcpy(&g_reg.option, pData, iCount * sizeof(DWORD));
			{
// 				REG_OPTION*	pReg				= (REG_OPTION*)pData;
// 				m_Option.set.all[0]				= pReg->set.all[0];
// 				m_Option.set.all[1]				= pReg->set.all[1];
// 
// 				m_Option.threshold_weight		= f24f32(pReg->threshold_weight);
// 
// 				m_Option.camera.eye.x			= f24f32(pReg->camera.eye.x);
// 				m_Option.camera.eye.y			= f24f32(pReg->camera.eye.y);
// 				m_Option.camera.eye.z			= f24f32(pReg->camera.eye.z);
// 				m_Option.camera.basis[0].x		= f24f32(pReg->camera.basis[0].x);
// 				m_Option.camera.basis[0].y		= f24f32(pReg->camera.basis[0].y);
// 				m_Option.camera.basis[0].z		= f24f32(pReg->camera.basis[0].z);
// 				m_Option.camera.basis[1].x		= f24f32(pReg->camera.basis[1].x);
// 				m_Option.camera.basis[1].y		= f24f32(pReg->camera.basis[1].y);
// 				m_Option.camera.basis[1].z		= f24f32(pReg->camera.basis[1].z);
// 				m_Option.camera.basis[2].x		= f24f32(pReg->camera.basis[2].x);
// 				m_Option.camera.basis[2].y		= f24f32(pReg->camera.basis[2].y);
// 				m_Option.camera.basis[2].z		= f24f32(pReg->camera.basis[2].z);
// 				m_Option.camera.distance		= f24f32(pReg->camera.distance);
// 				m_Option.camera.atten.start		= f24f32(pReg->camera.atten.start);
// 				m_Option.camera.atten.end		= f24f32(pReg->camera.atten.end);
// 				m_Option.camera.atten.weight[0]	= f24f32(pReg->camera.atten.weight[0]);
// 				m_Option.camera.atten.weight[1]	= f24f32(pReg->camera.atten.weight[1]);
// 				m_Option.camera.atten.weight[2]	= f24f32(pReg->camera.atten.weight[2]);
			}
			break;
		case RC_REG_OFFSET_AST:
			iStride		= REG_AST_SIZE;
			memcpy(&g_reg.ast, pData, iCount * sizeof(DWORD));
			assert(iCount == iStride);
			{
//				REG_AST*	pReg				= (REG_AST*)pData;
// 
// 				m_AST.def_list_num				= pReg->def_list_num;
// 				m_AST.def_list_addr				= pReg->def_list_addr;
// 				m_AST.bound_box.index			= pReg->bound_box.index;
// 				m_AST.bound_box.min.x			= f24f32(pReg->bound_box.min.x);
// 				m_AST.bound_box.min.y			= f24f32(pReg->bound_box.min.y);
// 				m_AST.bound_box.min.z			= f24f32(pReg->bound_box.min.z);
// 				m_AST.bound_box.max.x			= f24f32(pReg->bound_box.max.x);
// 				m_AST.bound_box.max.y			= f24f32(pReg->bound_box.max.y);
// 				m_AST.bound_box.max.z			= f24f32(pReg->bound_box.max.z);
			}
			break;
		default:
			if(Offset32 < (RC_REG_OFFSET_MATERIAL>>2)){
				// light
				int id				= (Offset32 - (RC_REG_OFFSET_LIGHT>>2)) / REG_LIGHT_SIZE;
				//RC_LIGHT* pLight	= &lights[id];
				REG_LIGHT*	pReg	= (REG_LIGHT*)pData;
				iStride				= REG_LIGHT_SIZE;
				assert(id >= 0 && id < 8);
				assert(iCount == iStride);
				memcpy(&g_reg.light[id], pData, iCount * sizeof(DWORD));

// 				pLight->position.x		= f24f32(pReg->position.x);
// 				pLight->position.y		= f24f32(pReg->position.y);
// 				pLight->position.z		= f24f32(pReg->position.z);
// 				pLight->direction.x		= f24f32(pReg->direction.x);
// 				pLight->direction.y		= f24f32(pReg->direction.y);
// 				pLight->direction.z		= f24f32(pReg->direction.z);
// 				pLight->spot[0]			= f24f32(pReg->spot.cos_in);
// 				pLight->spot[1]			= f24f32(pReg->spot.cos_out);
// 				pLight->spot[2]			= f24f32(pReg->spot.falloff);
// 
// 				pLight->ambient.r		= f24f32(pReg->ambient.r);
// 				pLight->ambient.g		= f24f32(pReg->ambient.g);
// 				pLight->ambient.b		= f24f32(pReg->ambient.b);
// 
// 				pLight->diffuse.r		= f24f32(pReg->diffuse.r);
// 				pLight->diffuse.g		= f24f32(pReg->diffuse.g);
// 				pLight->diffuse.b		= f24f32(pReg->diffuse.b);
// 
// 				pLight->specular.r		= f24f32(pReg->specular.r);
// 				pLight->specular.g		= f24f32(pReg->specular.g);
// 				pLight->specular.b		= f24f32(pReg->specular.b);
// 
// 				pLight->attenStart		= f24f32(pReg->atten.start);
// 				pLight->attenEnd		= f24f32(pReg->atten.end);
// 				pLight->attenWeight[0]	= f24f32(pReg->atten.weight[0]);
// 				pLight->attenWeight[1]	= f24f32(pReg->atten.weight[1]);
// 				pLight->attenWeight[2]	= f24f32(pReg->atten.weight[2]);
// 
// 				pLight->atten.all		= pReg->atten.mode;
				break;
			}else{
				// material
				int id							= (Offset32 - (RC_REG_OFFSET_MATERIAL>>2)) / REG_MATERIAL_SIZE;
				//RC_MATERIAL*	pMat			= &material[id];
				REG_MATERIAL*	pReg			= (REG_MATERIAL*)pData;
				iStride							= REG_MATERIAL_SIZE;
				assert(id >= 0 && id < 512);
				assert(iCount == iStride);
				memcpy(&g_reg.material[id], pData, iCount * sizeof(DWORD));
// 				pMaterial->tex.enable		= pReg->texture.enable;
// 				pMaterial->tex.address		= pReg->texture.address << 13;	// [27:13]
// 				pMaterial->tex.log.width	= pReg->texture.log_width;
// 				pMaterial->tex.log.height	= pReg->texture.log_height;

// 				pMat->ambient.r					= f24f32(pReg->ambient.r);
// 				pMat->ambient.g					= f24f32(pReg->ambient.g);
// 				pMat->ambient.b					= f24f32(pReg->ambient.b);
// 				pMat->diffuse.r					= f24f32(pReg->diffuse.r);
// 				pMat->diffuse.g					= f24f32(pReg->diffuse.g);
// 				pMat->diffuse.b					= f24f32(pReg->diffuse.b);
// 				pMat->specular.r				= f24f32(pReg->specular.r);
// 				pMat->specular.g				= f24f32(pReg->specular.g);
// 				pMat->specular.b				= f24f32(pReg->specular.b);
// 				pMat->specular_level			= f24f32(pReg->specular.level);
// 				pMat->specular_exponent			= f24f32(pReg->specular.exponent);
// 				pMat->reflectance				= f24f32(pReg->reflectance);
// 				pMat->transmittance				= f24f32(pReg->transmittance);
// 				pMat->refraction_index			= f24f32(pReg->refraction_index);
// 
// 				pMat->tex.diffuse.enable		= pReg->texture.enable;
// 				pMat->tex.diffuse.pData			= GetPointer(pReg->texture.address);
// 				pMat->tex.diffuse.width			= 1<<pReg->texture.log_width;
// 				pMat->tex.diffuse.height		= 1<<pReg->texture.log_height;
// 				pMat->tex.diffuse.mipmap		= pReg->texture.mipmap;
// 				pMat->tex.diffuse.repeat.u		= pReg->texture.repeat_u;
// 				pMat->tex.diffuse.repeat.v		= pReg->texture.repeat_v;
// 				pMat->tex.diffuse.mirror.u		= pReg->texture.mirror_u;
// 				pMat->tex.diffuse.mirror.v		= pReg->texture.mirror_v;
// 
// 				pMat->tex.normal.enable			= pReg->normal.enable;
// 				pMat->tex.normal.pData			= GetPointer(pReg->normal.address);
// 				pMat->tex.normal.width			= 1<<pReg->normal.log_width;
// 				pMat->tex.normal.height			= 1<<pReg->normal.log_height;
// 				pMat->tex.normal.mipmap			= pReg->normal.mipmap;
// 				pMat->tex.normal.repeat.u		= pReg->normal.repeat_u;
// 				pMat->tex.normal.repeat.v		= pReg->normal.repeat_v;
// 				pMat->tex.normal.mirror.u		= pReg->normal.mirror_u;
// 				pMat->tex.normal.mirror.v		= pReg->normal.mirror_v;
				break;
			}
			assert(0);
			return;
		}
		iCount		-= iStride;
		Offset32	+= iStride;
		pData		+= iStride;
	}
}

RC_OPTION		*g_Opt;
RC_LIGHT		*g_light;

BOUNDING_BOX	*g_Bound;
NODE			*g_Node;
LIST			*g_List;

_CountInfo CountInfo;

// test
#include "raytracesw.h"

void Emulation::RenderScene(void){
	/*
	int x, y;
	int tileSizeX, tileSizeY;

	memset(&CountInfo, 0, sizeof(_CountInfo));

	g_light			= &lights[0];

	//g_Node			= kd_node;
	g_Material		= material;
	g_Primitive		= PrePrimitive;
	g_List			= kd_list;

	tileSizeX		= opt.resolution.width;
	tileSizeY		= opt.resolution.height;

	x	= 0;
	y	= 0;

	rayTrace.Initialize(x, y);
	rayTrace.m_FrameBuffer.pBuffer	= g_reg.pFrameBuffer;
	rayTrace.m_FrameBuffer.width	= opt.resolution.width;
	rayTrace.m_FrameBuffer.height	= opt.resolution.height;

	rayTrace.material		= material;
	rayTrace.texture		= texture;
	rayTrace.light			= &lights[0];
	rayTrace.opt			= &opt;
	rayTrace.camera			= &camera;

	for(;;){

		if (rayTrace.ray.depth == -1){
			rayTrace.UpdateFrameBuffer();

			rayTrace.Initialize(x, y);

			// 다음 픽셀로 이동
			if(y==tileSizeY){
				break;
			}else{
				x++;   
				if(x==tileSizeX){

					y++;
					x	= 0;
				}
			}
		}

		if((rayTrace.RayKind!=SHADOW_RAY)&&(rayTrace.RayKind!=SHADOW_RAY))
			CountInfo.RayCount++;
		else
			CountInfo.ShadowCount++;

		rayTrace.depth = rayTrace.ray.depth;

		if(rayTrace.RayKind==SHADOW_RAY && !opt.set.shadow) 
			rayTrace.valid_traversal = 0;         

		rayTrace.traversal();

		if(rayTrace.RayKind == SHADOW_RAY || rayTrace.RayKind==TextureSHADOW_RAY)
			rayTrace.RayLength = rayTrace.ray.length;
		else
			rayTrace.RayLength = rayTrace.hit.t;

		if(rayTrace.hit.hit_result)
			rayTrace.ShadowRayMinDist = rayTrace.hit.t;

		if (rayTrace.valid_traversal){

			if (rayTrace.RayKind == PRIMARY_RAY && rayTrace.hit.hit_result == 0)
				rayTrace.RayHit.prim_num=0;

			if (rayTrace.RayKind!=SHADOW_RAY&&rayTrace.RayKind!=TextureSHADOW_RAY)
				rayTrace.RayHit = rayTrace.hit;

			//1차 광선/반사 광선/굴절 광선이면서 교차했다면, 즉 그림자 광선이 아닐 경우에
			if ((rayTrace.RayKind!=SHADOW_RAY&&rayTrace.RayKind!=TextureSHADOW_RAY) && rayTrace.hit.hit_result == 1){


				VECTOR3_LINE(rayTrace.Position, rayTrace.ray.o, rayTrace.ray.d, rayTrace.RayHit.t);

				rayTrace.ray.TotLength = rayTrace.ray.TotLength + rayTrace.RayHit.t;

				// ray direction initialize
				VECTOR3_COPY(rayTrace.Direction, rayTrace.ray.d);
			}
		}

		if((rayTrace.RayKind==SHADOW_RAY||rayTrace.RayKind == TextureSHADOW_RAY)&&(rayTrace.hit.hit_result==0))
			rayTrace.light_num++;

		rayTrace.shading(PrePrimitive, opt, camera.distance, LodTable, opt.background_color);

		if(rayTrace.light_num==opt.set.light_number){
			rayTrace.light_num	= 0;
			//마지막 라이트 여기서 color accumulation
		}
		else if(rayTrace.RayHit.hit_result==0){
			rayTrace.light_num	= 0;
		}
		else if(rayTrace.RayKind == TextureSHADOW_RAY){
		}
		else 
			rayTrace.RayKind = SHADOW_RAY;

		if (rayTrace.RayKind==SHADOW_RAY||rayTrace.RayKind == TextureSHADOW_RAY) 
			rayTrace.light = &lights[rayTrace.light_num];
	}
	*/
	{
		RayTraceSW m_RayTrace;

		{	// 한 프레임만 테스트
			static BOOL bFirst = TRUE;
			if(!bFirst) exit(0);
			bFirst	= FALSE;
		}

		m_RayTrace.trace_blm();
	}
}
