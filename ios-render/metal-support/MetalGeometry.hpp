//
//  MetalGeometry.hpp
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#ifndef MetalGeometry_hpp
#define MetalGeometry_hpp

#include <simd/simd.h>

namespace xxd{

    // 平方根倒数
    float InvSqrt(float x);

    // 使用平方根倒数速算的向量归一化
    simd_float3 Normalize(const simd_float3& v);

    // 创建单位矩阵
    simd_float4x4 MakeIdentity();

    // 创建世界坐标系变换到相机几何坐标系的矩阵
    simd_float4x4 MakeViewLookAt(const simd_float3& eyePos, const simd_float3& focusPos, const simd_float3& up);

    // 创建相机投影到裁剪空间的矩阵，裁剪空间做完透视除法之后，X[-1, 1]，Y[-1, 1]，Z[0, 1]
    simd_float4x4 MakePerspective(float fovRadians, float aspect, float znear, float zfar);

    // 创建绕X轴旋转矩阵
    simd_float4x4 MakeXRotate(float angleRadians);

    // 创建绕Y轴旋转矩阵
    simd_float4x4 MakeYRotate(float angleRadians);

    // 创建绕Z轴旋转矩阵
    simd_float4x4 MakeZRotate(float angleRadians);

    // 创建平移矩阵
    simd_float4x4 MakeTranslate(const simd_float3& v);

    // 创建缩放矩阵
    simd_float4x4 MakeScale(const simd_float3& v);
}

#endif /* MetalGeometry_hpp */
