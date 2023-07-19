//
//  MetalCamera.hpp
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#ifndef MetalCamera_hpp
#define MetalCamera_hpp

#include "MetalGeometry.hpp"

namespace xxd
{
class Camera
{
public:
    Camera(float asp);
    simd_float4x4 GetViewPerspecMatrix() const;
    simd_float4x4 GetPerspectiveMatrix() const;
    simd_float4x4 GetViewMatix() const;
public:
    // 参数
    float fovAngle;
    float aspect;
    float near;
    float far;
    simd_float3 focus;
    
    // 位置
    float raduis;
    float phi;
    float theta;
};

};

#endif /* MetalCamera_hpp */
