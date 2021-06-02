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
// Project : DDK
// DATE    : 5/24/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include <stdio.h>
#include <math.h>
#include <rc_util.h>

typedef union{
	float fvalue;
	int   ivalue;
}u_fi;

DWORD f32f24(float x) {

    u_fi fi;

    int sign;
    int exponent;
    int mantisa;
    
    fi.fvalue = x;
    exponent = (fi.ivalue & 0x7F800000);

    if (exponent <= 0x20000000) { 
        return 0;
    }

    if(exponent < 0x60000000 ){      
        mantisa  = (fi.ivalue>>7) & 0xFFFF;
        sign     = (fi.ivalue>>8) & 0x800000;   
        exponent -=0x20000000;
        exponent = (exponent>>7) & 0x7f0000;

        fi.ivalue = sign + exponent + mantisa;
        return fi.ivalue;   
    }   

    sign     = (fi.ivalue>>8) & 0x800000;   
    fi.ivalue = sign|0x7fffff;   

    return fi.ivalue; 
}


float f24f32(int x) {

    u_fi fi;
    int sign;
    int exponent;
    int mantisa;

    fi.ivalue = x;

    sign     = fi.ivalue & 0x800000;
    exponent = fi.ivalue & 0x7F0000;
    mantisa  = fi.ivalue & 0xFFFF;

    sign = sign >> 23;
    exponent = exponent >> 16;
    exponent += 64;


    sign = sign << 31;
    exponent = exponent << 23;
    mantisa = mantisa << 7;
    if(x==0)
        fi.ivalue=0;
    else fi.ivalue = sign + exponent + mantisa;

    return fi.fvalue;  
}

float x32f32(DWORD x){
	return (float)(x * (1.0f/FIXED_ONE));
}

DWORD f32x32(float x){
	return (DWORD) (x*0x10000);
}

DWORD f32f16(float x)
{
    unsigned int fi;

	if(x<0) x = 0;
	else
	if(x>1.f) x = 1.f;

	fi = (f32f24(x) >> 4);
	if((fi&0xF0000) != 0x30000) fi = 0;
	else
	fi &= 0xFFFF;

    return fi;   
}

float f16f32(int x) {
	if(x&0xF000) x |= 0x30000;
	x<<=4;

	return f24f32(x);  
}

void vector3Normalization(float *out, float *in) {

    float length;

    length = (float) ( 1 / sqrt((in[0] * in[0]) + (in[1] * in[1]) * (in[2] * in[2])) );

    out[0] = in[0] * length;

}
