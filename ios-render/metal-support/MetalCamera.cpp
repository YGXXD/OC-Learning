//
//  MetalCamera.cpp
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#include "MetalCamera.h"

xxd::Camera::Camera(float asp) : aspect(asp)
{
    fovAngle = M_PI / 2;
    near = 1;
    far = 10000;
    focus = {0 , 0, 0};
    raduis = 20;
    phi = M_PI / 2.f;
    theta = 0;
}

simd_float4x4 xxd::Camera::GetViewPerspecMatrix() const
{
    return simd_mul(GetViewMatix(), GetPerspectiveMatrix());
}

simd_float4x4 xxd::Camera::GetPerspectiveMatrix() const
{
    return MakePerspective(fovAngle, aspect, near, far);
}

simd_float4x4 xxd::Camera::GetViewMatix() const {
    simd_float3 eye = { raduis * sinf(phi) * cosf(theta), raduis * sinf(phi) * sinf(theta), raduis * cosf(phi) };
    return MakeViewLookAt(eye, focus, (simd_float3){0, 0, 1});
}



