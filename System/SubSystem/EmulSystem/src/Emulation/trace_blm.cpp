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
#include "Emulation_Common.h"
#include "RayTraceSW.h"
#include "RayRendering.h"
#include "operate.h"
#include <stdio.h>
#include <process.h>
#include <string.h>

_option*			RtOpt;

_material*			RtMaterial;

_CountInfo			g_CountInfo;

void RayTraceSW::trace_blm(void){
	int i,j, k, ValidSum;
	int tileSize, tileLoop, tileStartX, tileStartY;
	int testcount = 0;
	RayRendering* m_RayRendering	= new RayRendering[1024];
   
	int print=0;
#ifdef USE_REDUCED_ERROR_CORRECTION
	_DataMargin2 FadderError0;//, FadderError1;//dk
	_DataMargin2 FmulError0;//, FmulError1;//dk
#endif

#if(MODE==2)
	struct _traversal_buffer InData[1024], OutData[1024];
#endif

	int MaxBuf=0x1;
	//int MaxBuf=64;
	i=0;
	j=0;
	tileStartX=0;
	tileStartY=0;
	tileLoop=0;
	tileSize=16;//dk tile size is 16*16

#ifdef USE_REDUCED_ERROR_CORRECTION
	RayFloat FMtemp1;//, FMtemp2;//dk
#endif

	g_CountInfo.ListCount = 0;
	g_CountInfo.NodeCount = 0;
	g_CountInfo.TriCount = 0;
	g_CountInfo.SITCount = 0;//dk SIT
	g_CountInfo.SITnoCount = 0;//dk SIT

	g_CountInfo.RayCount = 0;
	g_CountInfo.RayBoxCount  = 0;
	g_CountInfo.RayNodeCount  = 0;
	g_CountInfo.RayTriCount  = 0;
	g_CountInfo.ShadowCount  = 0;
	g_CountInfo.ShadowBoxCount  = 0;
	g_CountInfo.ShadowNodeCount = 0;
	g_CountInfo.ShadowTriCount = 0;
   
	RtOpt = &opt;

	RtMaterial = material;
	for(k=0;k <MaxBuf; k++){
		tileStartX = i%tileSize + (i/(tileSize*tileSize))*tileSize;
		tileStartY = (i/tileSize)%tileSize + j*tileSize;

		m_RayRendering[k].Initialize(tileStartX,tileStartY);
		m_RayRendering[k].material=material;
		m_RayRendering[k].texture_id=texture_id;
		m_RayRendering[k].texture=texture;
		m_RayRendering[k].normalmap=normalmap;//dk normal mapping
		m_RayRendering[k].light = &g_reg.light[0];
		m_RayRendering[k].opt = &opt;

		i++;
		if(i==g_reg.control.resolution.width*tileSize){
			j++;
			i=0;
		}
		if(j==g_reg.control.resolution.height/tileSize)
			break;
	}

	do{
		for(k=0;k<MaxBuf;k++){
			ray_generation(&m_RayRendering[k].RayGenInData, &m_RayRendering[k].RayGenOutData);
			if (m_RayRendering[k].ray.depth == -1){
				if(m_RayRendering[k].Valid){
					m_RayRendering[k].UpdateFrameBuffer();
				}

				tileStartX = i%tileSize + (i/(tileSize*tileSize))*tileSize;
				tileStartY = (i/tileSize)%tileSize + j*tileSize;

				m_RayRendering[k].Initialize(tileStartX,tileStartY);
				m_RayRendering[k].material=material;
				m_RayRendering[k].texture_id=texture_id;
				m_RayRendering[k].texture=texture;
				m_RayRendering[k].normalmap=normalmap;//dk normal mapping
				m_RayRendering[k].light = &g_reg.light[0];
				m_RayRendering[k].opt = &opt;

				ray_generation(&m_RayRendering[k].RayGenInData, &m_RayRendering[k].RayGenOutData);
				if(j==g_reg.control.resolution.height/tileSize)
					m_RayRendering[k].Valid = 0;
				else{
					i++;
					if(i==g_reg.control.resolution.width*tileSize){
						j++;
						i=0;
					}
				}
			}
		}

		for(k=0;k<MaxBuf;k++){
			if((m_RayRendering[k].RayKind!=RAY_TYPE_SHADOW)&&(m_RayRendering[k].RayKind!=RAY_TYPE_SHADOW))
				g_CountInfo.RayCount++;
			else
				g_CountInfo.ShadowCount++;
			m_RayRendering[k].depth=m_RayRendering[k].ray.depth;

			if(m_RayRendering[k].RayKind==RAY_TYPE_SHADOW&&g_reg.option.set.shadow==0) m_RayRendering[k].valid_traversal = 0;

			if(m_RayRendering[k].RayKind == RAY_TYPE_PRIMARY)//dk 130318 added
				m_RayRendering[k].SITbuf.count = 0;
			else if(m_RayRendering[k].RayKind == 0)
				m_RayRendering[k].SITbuf.count--;
			if(m_RayRendering[k].RayKind != RAY_TYPE_PRIMARY)
				if(m_RayRendering[k].SITbuf.SIT_primNum[m_RayRendering[k].SITbuf.count] != m_RayRendering[k].SIT_primNum)
					printf("error 1 \n");
			traversal(&m_RayRendering[k].TnIInData, &m_RayRendering[k].TnIOutData);

			if(m_RayRendering[k].RayKind == RAY_TYPE_PRIMARY)//dk primary ray
				m_RayRendering[k].SITbuf.SIT_primNum[0] = m_RayRendering[k].hit.prim_num;
			else if(m_RayRendering[k].RayKind == 6){//dk reflection and refraction ray
				m_RayRendering[k].SITbuf.count++;
				if(m_RayRendering[k].SITbuf.count>4)
					printf("error 2 \n");
				m_RayRendering[k].SITbuf.SIT_primNum[m_RayRendering[k].SITbuf.count] = m_RayRendering[k].hit.prim_num;
			}
			else if(m_RayRendering[k].RayKind == 0)//dk NULL ray
				m_RayRendering[k].SITbuf.SIT_primNum[m_RayRendering[k].SITbuf.count] = m_RayRendering[k].hit.prim_num;
			else if(m_RayRendering[k].RayKind != RAY_TYPE_SHADOW && m_RayRendering[k].RayKind != RAY_TYPE_TEXTURE_SHADOW && m_RayRendering[k].RayKind != RAY_TYPE_NORMAL && m_RayRendering[k].RayKind != RAY_TYPE_SKYBOX)//dk reflection ray, refraction ray
				m_RayRendering[k].SITbuf.SIT_primNum[m_RayRendering[k].SITbuf.count] = m_RayRendering[k].hit.prim_num;
			else{//dk shadow ray, texture shadow ray, normal ray, skybox ray
			}
		}
#if(MODE==2)
		for(k=0;k<MaxBuf;k++){
			InData[k].buffer[0]=(m_RayRendering[k].RayKind<<2)|2|m_RayRendering[k].valid_traversal;//ray_kind, ColorShaodw, valid
			InData[k].buffer[1]=m_RayRendering[k].ray.length;
			InData[k].buffer[2]=m_RayRendering[k].ray.inv_d[0];
			InData[k].buffer[3]=m_RayRendering[k].ray.inv_d[1];
			InData[k].buffer[4]=m_RayRendering[k].ray.inv_d[2];
			InData[k].buffer[5]=m_RayRendering[k].ray.d[0];
			InData[k].buffer[6]=m_RayRendering[k].ray.d[1];
			InData[k].buffer[7]=m_RayRendering[k].ray.d[2];
			InData[k].buffer[8]=m_RayRendering[k].ray.o[0];
			InData[k].buffer[9]=m_RayRendering[k].ray.o[1];
			InData[k].buffer[10]=m_RayRendering[k].ray.o[2];
			InData[k].buffer[11] = (k<<20)|(m_RayRendering[k].m_iY<<10)|m_RayRendering[k].m_iX;
			InData[k].buffer[12] = (g_reg.ast.def_list_num<<24)|g_reg.ast.def_list_addr;
			InData[k].buffer[13] = ((m_RayRendering[k].light_num&0xf)<<17)|(1<<16);
			InData[k].buffer[14] = m_RayRendering[k].ShadowRayMinDist;// address
			InData[k].buffer[15] = 0;
		}
		InData[k-1].buffer[15] = ((MaxBuf-1)<<2)|1;
#endif

#if(MODE==2)
		for(k=0;k<MaxBuf;k++){
			OutData[k].buffer[0] = ((m_RayRendering[k].hit.prim_num&0xfffff)<<8)|m_RayRendering[k].hit.hit_result;
			OutData[k].buffer[1] = m_RayRendering[k].hit.t;
			OutData[k].buffer[2] = m_RayRendering[k].ray.d[0];
			OutData[k].buffer[3] = m_RayRendering[k].ray.d[1];
			OutData[k].buffer[4] = m_RayRendering[k].ray.d[2];
			OutData[k].buffer[5] = m_RayRendering[k].ray.o[0];
			OutData[k].buffer[6] = m_RayRendering[k].ray.o[1];
			OutData[k].buffer[7] = m_RayRendering[k].ray.o[2];
			OutData[k].buffer[8] = m_RayRendering[k].hit.beta;
			OutData[k].buffer[9] = m_RayRendering[k].hit.gamma;
			OutData[k].buffer[10] = m_RayRendering[k].RayKind|((m_RayRendering[k].hit.significantFigure&0xff)<<8);
			if((m_RayRendering[k].RayKind==RAY_TYPE_SHADOW)||(m_RayRendering[k].RayKind==RAY_TYPE_TEXTURE_SHADOW)){
				OutData[k].buffer[11]= m_RayRendering[k].ray.dielectric[0];
				OutData[k].buffer[12]= m_RayRendering[k].ray.dielectric[1];
				OutData[k].buffer[13]= m_RayRendering[k].ray.dielectric[2];
			}
			else{
				OutData[k].buffer[11]= m_RayRendering[k].ray.weight[0];
				OutData[k].buffer[12]= m_RayRendering[k].ray.weight[1];
				OutData[k].buffer[13]= m_RayRendering[k].ray.weight[2];
			}
			OutData[k].buffer[14]= k;   
		}

		g_CountInfo.ListCount=0;
		g_CountInfo.NodeCount=0;
		g_CountInfo.TriCount=0;
#endif
		for(k=0;k<MaxBuf;k++){
			if(m_RayRendering[k].RayKind == RAY_TYPE_SHADOW||m_RayRendering[k].RayKind==RAY_TYPE_TEXTURE_SHADOW)
				m_RayRendering[k].RayLength = m_RayRendering[k].ray.length;
			else
				m_RayRendering[k].RayLength = m_RayRendering[k].hit.t;

			if(m_RayRendering[k].hit.hit_result)
				m_RayRendering[k].ShadowRayMinDist = m_RayRendering[k].hit.t;

			if (m_RayRendering[k].valid_traversal){
				if (m_RayRendering[k].RayKind == RAY_TYPE_PRIMARY && m_RayRendering[k].hit.hit_result == 0){
					m_RayRendering[k].RayHit.prim_num=0;
				}

				if (m_RayRendering[k].RayKind!=RAY_TYPE_SHADOW&&m_RayRendering[k].RayKind!=RAY_TYPE_TEXTURE_SHADOW)
					m_RayRendering[k].RayHit = m_RayRendering[k].hit;

				//primary ray, reflection ray, refraction ray with hit
				if ((m_RayRendering[k].RayKind!=RAY_TYPE_SHADOW&&m_RayRendering[k].RayKind!=RAY_TYPE_TEXTURE_SHADOW) && m_RayRendering[k].hit.hit_result == 1){
					/*m_RayRendering[k].Position[0] = fadder(m_RayRendering[k].ray.o[0], fmul(m_RayRendering[k].ray.d[0], m_RayRendering[k].RayHit.t),0);
					m_RayRendering[k].Position[1] = fadder(m_RayRendering[k].ray.o[1], fmul(m_RayRendering[k].ray.d[1], m_RayRendering[k].RayHit.t),0);
					m_RayRendering[k].Position[2] = fadder(m_RayRendering[k].ray.o[2], fmul(m_RayRendering[k].ray.d[2], m_RayRendering[k].RayHit.t),0);*///dk
#ifdef USE_REDUCED_ERROR_CORRECTION
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////dk Rendering Error Correction : Use significant figure
					FmulError0.In0 = ((m_RayRendering[k].ray.d[0]>>16)&0x7f) - 0x4e;
					FmulError0.In1 = m_RayRendering[k].RayHit.significantFigure;
					FMtemp1 = fmulMargin(m_RayRendering[k].ray.d[0], m_RayRendering[k].RayHit.t, &FmulError0);

					FadderError0.In0 = ((m_RayRendering[k].ray.o[0]>>16)&0x7f) - 0x4f;
					FadderError0.In1 = FmulError0.Out;
					m_RayRendering[k].Position[0] = fadderMargin(m_RayRendering[k].ray.o[0], FMtemp1, 0, &FadderError0);
					m_RayRendering[k].positionSignificantFigure[0] = FadderError0.Out;

					FmulError0.In0 = (m_RayRendering[k].ray.d[1]>>16&0x7f) - 0x4e;
					FmulError0.In1 = m_RayRendering[k].RayHit.significantFigure;
					FMtemp1 = fmulMargin(m_RayRendering[k].ray.d[1], m_RayRendering[k].RayHit.t, &FmulError0);

					FadderError0.In0 = (m_RayRendering[k].ray.o[1]>>16&0x7f) - 0x4f;
					FadderError0.In1 = FmulError0.Out;
					m_RayRendering[k].Position[1] = fadderMargin(m_RayRendering[k].ray.o[1], FMtemp1, 0, &FadderError0);
					m_RayRendering[k].positionSignificantFigure[1] = FadderError0.Out;

					FmulError0.In0 = (m_RayRendering[k].ray.d[2]>>16&0x7f) - 0x4e;
					FmulError0.In1 = m_RayRendering[k].RayHit.significantFigure;
					FMtemp1 = fmulMargin(m_RayRendering[k].ray.d[2], m_RayRendering[k].RayHit.t, &FmulError0);

					FadderError0.In0 = (m_RayRendering[k].ray.o[2]>>16&0x7f) - 0x4f;
					FadderError0.In1 = FmulError0.Out;
					m_RayRendering[k].Position[2] = fadderMargin(m_RayRendering[k].ray.o[2], FMtemp1, 0, &FadderError0);
					m_RayRendering[k].positionSignificantFigure[2] = FadderError0.Out;
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
					m_RayRendering[k].Position[0] = fadder(m_RayRendering[k].ray.o[0], fmul(m_RayRendering[k].ray.d[0], m_RayRendering[k].RayHit.t),0);
					m_RayRendering[k].Position[1] = fadder(m_RayRendering[k].ray.o[1], fmul(m_RayRendering[k].ray.d[1], m_RayRendering[k].RayHit.t),0);
					m_RayRendering[k].Position[2] = fadder(m_RayRendering[k].ray.o[2], fmul(m_RayRendering[k].ray.d[2], m_RayRendering[k].RayHit.t),0);
#endif
					m_RayRendering[k].ray.TotLength = fadder(m_RayRendering[k].ray.TotLength, m_RayRendering[k].RayHit.t, 0);

					m_RayRendering[k].Direction[0] = m_RayRendering[k].ray.d[0];
					m_RayRendering[k].Direction[1] = m_RayRendering[k].ray.d[1];
					m_RayRendering[k].Direction[2] = m_RayRendering[k].ray.d[2];
				}
			}
		}

		for(k=0;k<MaxBuf;k++){
			if((m_RayRendering[k].RayKind==RAY_TYPE_SHADOW||m_RayRendering[k].RayKind == RAY_TYPE_TEXTURE_SHADOW)&&(m_RayRendering[k].hit.hit_result==0))
				m_RayRendering[k].light_num++;

			m_RayRendering[k].shading(opt, g_reg.option.camera.distance, texture_index, normalmap_index);
		}

		for(k=0;k<MaxBuf;k++){
			if(m_RayRendering[k].light_num==g_reg.option.set.light_number){
				m_RayRendering[k].light_num=0;
				m_RayRendering[k].reflection_color[0] = m_RayRendering[k].reflection_color[0]&0xfffff0;
				m_RayRendering[k].reflection_color[1] = m_RayRendering[k].reflection_color[1]&0xfffff0;
				m_RayRendering[k].reflection_color[2] = m_RayRendering[k].reflection_color[2]&0xfffff0;
				m_RayRendering[k].refraction_color[0] = m_RayRendering[k].refraction_color[0]&0xfffff0;
				m_RayRendering[k].refraction_color[1] = m_RayRendering[k].refraction_color[1]&0xfffff0;
				m_RayRendering[k].refraction_color[2] = m_RayRendering[k].refraction_color[2]&0xfffff0;
			}
			else if(m_RayRendering[k].RayHit.hit_result==0){
				m_RayRendering[k].light_num=0;
				m_RayRendering[k].reflection_color[0] = m_RayRendering[k].reflection_color[0]&0xfffff0;
				m_RayRendering[k].reflection_color[1] = m_RayRendering[k].reflection_color[1]&0xfffff0;
				m_RayRendering[k].reflection_color[2] = m_RayRendering[k].reflection_color[2]&0xfffff0;
				m_RayRendering[k].refraction_color[0] = m_RayRendering[k].refraction_color[0]&0xfffff0;
				m_RayRendering[k].refraction_color[1] = m_RayRendering[k].refraction_color[1]&0xfffff0;
				m_RayRendering[k].refraction_color[2] = m_RayRendering[k].refraction_color[2]&0xfffff0;
			}
			else if(m_RayRendering[k].RayKind == RAY_TYPE_TEXTURE_SHADOW){
			}
			else if(m_RayRendering[k].RayKind == RAY_TYPE_SKYBOX){
			}
			else if(m_RayRendering[k].RayKind == RAY_TYPE_NORMAL){
			}
			else
				m_RayRendering[k].RayKind = RAY_TYPE_SHADOW;
		}

		for(k=0;k<MaxBuf;k++){
			if (m_RayRendering[k].RayKind==RAY_TYPE_SHADOW||m_RayRendering[k].RayKind == RAY_TYPE_TEXTURE_SHADOW) 
				m_RayRendering[k].light = &g_reg.light[m_RayRendering[k].light_num];

			m_RayRendering[k].valid_ray_generation = 1;
		}
		ValidSum = 0;
		for(k=0;k<MaxBuf;k++)
			ValidSum += m_RayRendering[k].Valid;
		testcount++;
	}
	while(ValidSum);
	g_CountInfo.RayCount--;

	g_CountInfo.ListCount=0;
	g_CountInfo.NodeCount=0;
	g_CountInfo.TriCount=0;
	g_CountInfo.RayCount=0;
	g_CountInfo.RayBoxCount=0;
	g_CountInfo.RayNodeCount=0;
	g_CountInfo.RayTriCount=0;
	g_CountInfo.ShadowCount=0;
	g_CountInfo.ShadowBoxCount=0;
	g_CountInfo.ShadowNodeCount=0;
	g_CountInfo.ShadowTriCount=0;
	g_CountInfo.SITCount=0;//dk SIT
	g_CountInfo.SITnoCount=0;//dk SIT

	delete [] m_RayRendering;
}