#include "XMath.hpp"

float xxd::InvSqrt(float x)
{
    uint32_t i;
    float a = x;

    //将浮点数转化为32位整数
    i = *(uint32_t*)&a;
    //取出IEEE754指数和尾数,原算法u的值为0.0450465
    i = 0x5f3759df - (i >> 1);
    //求出平方根倒数近似解
    a = *(float*)&i;
    //用牛顿迭代法进行迭代增加精度
    a = a * (1.5f - (0.5f * x * a * a));

    return a;
}

simd_float3 xxd::Normalize(const simd_float3& v)
{
    return v * xxd::InvSqrt(simd_length_squared(v));
}

simd_float4x4 xxd::MakeIdentity()
{
    return (simd_float4x4){ (simd_float4){ 1.f, 0.f, 0.f, 0.f },
                             (simd_float4){ 0.f, 1.f, 0.f, 0.f },
                             (simd_float4){ 0.f, 0.f, 1.f, 0.f },
                             (simd_float4){ 0.f, 0.f, 0.f, 1.f } };
}

simd_float4x4 xxd::MakeViewLookAt(const simd_float3& eyePos, const simd_float3& focusPos, const simd_float3& up)
{
    simd_float3 z = Normalize(focusPos - eyePos);
    simd_float3 x = Normalize(simd_cross(up, z));
    simd_float3 y = simd_cross(z, x);
    float wx = -simd_dot(eyePos, x);
    float wy = -simd_dot(eyePos, y);
    float wz = -simd_dot(eyePos, z);

    return simd_matrix_from_rows((simd_float4){ x.x, y.x, z.x, 0.f },
                                 (simd_float4){ x.y ,y.y, z.y, 0.f },
                                 (simd_float4){ x.z, y.z, z.z, 0.f },
                                 (simd_float4){ wx, wy, wz, 1.f });
}

simd_float4x4 xxd::MakePerspective(float fovRadians, float aspect, float znear, float zfar)
{
    float ys = 1.f / tanf(fovRadians * 0.5f);
    float xs = ys / aspect;
    float zs = zfar / ( znear - zfar );
    return simd_matrix_from_rows((simd_float4){ xs, 0.0f, 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, ys, 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, 0.0f, -zs, 1.f },
                                 (simd_float4){ 0, 0, znear * zs, 0.f });
}

simd_float4x4 xxd::MakeXRotate(float angleRadians)
{
    const float a = angleRadians;
    return simd_matrix_from_rows((simd_float4){ 1.0f, 0.0f, 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, cosf( a ), sinf( a ), 0.0f },
                                 (simd_float4){ 0.0f, -sinf( a ), cosf( a ), 0.0f },
                                 (simd_float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd_float4x4 xxd::MakeYRotate(float angleRadians)
{
    const float a = angleRadians;
    return simd_matrix_from_rows((simd_float4){ cosf( a ), 0.0f, -sinf( a ), 0.0f },
                                 (simd_float4){ 0.0f, 1.0f, 0.0f, 0.0f },
                                 (simd_float4){ sinf( a ), 0.0f, cosf( a ), 0.0f },
                                 (simd_float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd_float4x4 xxd::MakeZRotate(float angleRadians)
{
    const float a = angleRadians;
    return simd_matrix_from_rows((simd_float4){ cosf( a ), sinf( a ), 0.0f, 0.0f },
                                 (simd_float4){ -sinf( a ), cosf( a ), 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, 0.0f, 1.0f, 0.0f },
                                 (simd_float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd_float4x4 xxd::MakeTranslate(const simd_float3& v)
{
    return simd_matrix_from_rows((simd_float4){ 1.0f, 0.0f, 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, 1.0f, 0.0f, 0.0f },
                                 (simd_float4){ 0.0f, 0.0f, 1.0f, 0.0f },
                                 (simd_float4){ v.x, v.y, v.z, 1.0f });
}

simd_float4x4 xxd::MakeScale(const simd_float3& v)
{
    return simd_matrix((simd_float4){ v.x, 0, 0, 0 },
                       (simd_float4){ 0, v.y, 0, 0 },
                       (simd_float4){ 0, 0, v.z, 0 },
                       (simd_float4){ 0, 0, 0, 1.0 });
}
