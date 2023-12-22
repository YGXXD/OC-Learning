#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

struct vin
{
    float3 position [[attribute(0)]];
    float3 color [[attribute(1)]];
};

v2f vertex render3DVertexMain( uint vertexId [[vertex_id]],
						  vin in [[stage_in]],
                          device const float4x4* cameraData [[buffer(1)]] )
{
    v2f o;
    o.position = float4( in.position, 1.0 ) * cameraData[0];
    o.color = half3 ( in.color );
    return o;
}

half4 fragment render3DFragmentMain( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}

