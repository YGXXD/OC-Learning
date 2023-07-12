//
//  XXdMath.h
//  AppTest
//
//  Created by chenqiudu on 2023/7/11.
//

#ifndef XXdMath_h
#define XXdMath_h
#import <simd/simd.h>

// 矩阵为col排列

simd_float4x4 makeIdentity(void)
{
    return (simd_float4x4){ (simd_float4){ 1.f, 0.f, 0.f, 0.f },
                            (simd_float4){ 0.f, 1.f, 0.f, 0.f },
                            (simd_float4){ 0.f, 0.f, 1.f, 0.f },
                            (simd_float4){ 0.f, 0.f, 0.f, 1.f } };
}

simd_float4x4 makeViewLookAt(simd_float3 eyePos, simd_float3 focusPos, simd_float3 up)
{
    simd_float3 z = simd_normalize(focusPos - eyePos);
    simd_float3 x = simd_normalize(simd_cross(up, z));
    simd_float3 y = simd_cross(z, x);
    float wx = -simd_dot(eyePos, x);
    float wy = -simd_dot(eyePos, y);
    float wz = -simd_dot(eyePos, z);


    return simd_matrix_from_rows((simd_float4){ x.x, y.x, z.x, 0.f },
                                 (simd_float4){ x.y ,y.y, z.y, 0.f },
                                 (simd_float4){ x.z, y.z, z.z, 0.f },
                                 (simd_float4){ wx, wy, wz, 1.f });
}

simd_float4x4 makePerspective(float fovRadians, float aspect, float znear, float zfar)
{
    float ys = 1.f / tanf(fovRadians * 0.5f);
    float xs = ys / aspect;
    float zs = zfar / ( znear - zfar );
    return simd_matrix_from_rows((simd_float4){ xs, 0.0f, 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, ys, 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, 0.0f, -zs, 1.f },
                                 (simd_float4){ 0, 0, znear * zs, 0.f });
}

#endif /* XXdMath_h */
