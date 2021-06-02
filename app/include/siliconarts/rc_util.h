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
#include "STDInterface.h"

#define FIXED_PRECISION 	16							// number of fractional bits
#define FIXED_ONE		  	(1 << FIXED_PRECISION)		// representation of 1
#define FIXED_ZERO	  		0							// representation of 0
#define FIXED_HALF	  		0x08000						// S15.16 0.5
#define FIXED_PINF	  		0x7fffffff					// +inf
#define FIXED_MINF	  		0x80000000					// -inf

#define SUB         -
#define ADD         +
#define MUL         *
#define DIV         /

#define VECTOR3_NORMALIZATION (vector3Normalization)

#define VECTOR2_OPERATION(out, operand1, instruction, operand2) \
    out[0] = operand1[0] instruction operand2[0];  \
    out[1] = operand1[1] instruction operand2[1]

#define VECTOR2_OPERATION_ONE(out, operand, instruction, value) \
    out[0] = operand[0] instruction value;  \
    out[1] = operand[1] instruction value

#define VECTOR2_SUBSTITUTE(out, value) \
    out[0] = value;  \
    out[1] = value

#define VECTOR2_COPY(out, in) \
    out[0] = in[0];  \
    out[1] = in[1]

#define VECTOR2_CONVERT_24F(out, in) \
    out.x = f32f24(in.x);  \
    out.y = f32f24(in.y)


#define VECTOR3_OPERATION(out, operand1, instruction, operand2) \
    out[0] = operand1[0] instruction operand2[0];  \
    out[1] = operand1[1] instruction operand2[1];  \
    out[2] = operand1[2] instruction operand2[2]

#define VECTOR3_OPERATION_ONE(out, operand, instruction, value) \
    out[0] = operand[0] instruction value;  \
    out[1] = operand[1] instruction value;  \
    out[2] = operand[2] instruction value

#define ONE_OPERATION_VECTOR3(out, value, instruction, operand) \
    out[0] = value instruction operand[0];  \
    out[1] = value instruction operand[1];  \
    out[2] = value instruction operand[2] 

#define VECTOR3_SUBSTITUTE(out, value) \
    out[0] = value;  \
    out[1] = value;  \
    out[2] = value

#define VECTOR3_SUBSTITUTE_3(out, value0, value1, value2) \
    out[0] = value0;  \
    out[1] = value1;  \
    out[2] = value2

#define VECTOR3_COPY(out, in) \
    out[0] = in[0];  \
    out[1] = in[1];  \
    out[2] = in[2]

#define VECTOR3_LINE(out, o, d, t)  \
    out[0] = o[0] + (t * d[0]);     \
    out[1] = o[1] + (t * d[1]);     \
    out[2] = o[2] + (t * d[2])

#define VECTOR3_CONVERT_24F(out, in) \
    out.x = f32f24(in.x);  \
    out.y = f32f24(in.y);  \
    out.z = f32f24(in.z)

#define VECTOR4_OPERATION(out, operand1, instruction, operand2) \
    out[0] = operand1[0] instruction operand2[0];  \
    out[1] = operand1[1] instruction operand2[1];  \
    out[2] = operand1[2] instruction operand2[2];   \
    out[3] = operand1[3] instruction operand2[3]

#define VECTOR4_OPERATION_ONE(out, operand, instruction, value) \
    out[0] = operand[0] instruction value;  \
    out[1] = operand[1] instruction value;  \
    out[2] = operand[2] instruction value;  \
    out[3] = operand[2] instruction value

#define VECTOR4_SUBSTITUTE(out, value) \
    out[0] = value;  \
    out[1] = value;  \
    out[2] = value;  \
    out[3] = value

#define VECTOR4_COPY(out, in) \
    out[0] = in[0];  \
    out[1] = in[1];  \
    out[2] = in[2];  \
    out[3] = in[3]


#define VECTOR3_CROSS_PRODUCT(out, operand1, operand2)          \
    out[0] = operand1[1]*operand2[2] - operand1[2]*operand2[1]; \
    out[1] = operand1[2]*operand2[0] - operand1[0]*operand2[2]; \
    out[2] = operand1[0]*operand2[1] - operand1[1]*operand2[0]

#ifdef __cplusplus
extern "C" {
#endif

DWORD f32f24(float x);
float f24f32(int x);
float x32f32(DWORD x);
DWORD f32x32(float x);
DWORD f32f16(float x);
float f16f32(int x);

#ifdef __cplusplus
}
#endif
