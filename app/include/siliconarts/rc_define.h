/*    =====================================================
    RayCoreType.h -- type definition header file
    RayCoreSR source code Copyright(c) 2012 Siliconarts Inc.

    Implemented by Dongseok Kim (dskim@siliconarts.com), Siliconarts Inc.
    =====================================================*/

#pragma once

#define UPPER_PACKING(data, n)  ((data & 0x00FFFFFF)<<(n))
#define LOWER_PACKING(data, n)  ((data & 0x00FFFFFF)>>(n))

#define PACKING(high, low, n)   (UPPER_PACKING(high, n) | LOWER_PACKING(low, (24-n)))
#define RGB_PACKING(r, g, b) ((0xff)<<24)+((b)<<16)+((g)<<8)+r
#define RGBA_PACKING(r, g, b, a) ((r)<<24)+((g)<<16)+((b)<<8)+ (a)
