//
//  XXdMath.h
//  XXdMath
//
//  Created by chenqiudu on 2023/7/12.
//

/** 左手坐标系 **/
/*
    世界坐标系正方向为X，右方向为Y，上方向为Z
    相机几何坐标系正方向为Z，上方向为Y，右方向为X
*/


#ifndef XXdMath_h
#define XXdMath_h

#include <simd/simd.h>

// 平方根倒数
float xxdInvSqrt(float x);

// 使用平方根倒数速算的向量归一化
simd_float3 xxd_simd_normalize(simd_float3 v);

// 创建单位矩阵
simd_float4x4 xxdMakeIdentity(void);

// 创建世界坐标系变换到相机几何坐标系的矩阵
simd_float4x4 xxdMakeViewLookAt(simd_float3 eyePos, simd_float3 focusPos, simd_float3 up);

// 创建相机投影到裁剪空间的矩阵，裁剪空间做完透视除法之后，X[-1, 1]，Y[-1, 1]，Z[0, 1]
simd_float4x4 xxdMakePerspective(float fovRadians, float aspect, float znear, float zfar);

// 创建绕X轴旋转矩阵
simd_float4x4 xxdMakeXRotate(float angleRadians);

// 创建绕Y轴旋转矩阵
simd_float4x4 xxdMakeYRotate(float angleRadians);

// 创建绕Z轴旋转矩阵
simd_float4x4 xxdMakeZRotate(float angleRadians);

// 创建平移矩阵
simd_float4x4 xxdMakeTranslate(const simd_float3 v);

// 创建缩放矩阵
simd_float4x4 xxdMakeScale(const simd_float3 v);

#endif /* XXdMath_h */
