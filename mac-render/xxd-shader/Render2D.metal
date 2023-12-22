#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

v2f vertex render2DVertexMain( uint vertexId [[vertex_id]],
                       device const float3* positions [[buffer(0)]],
                       device const float3* colors [[buffer(1)]],
                       device const float2* viewportOffset [[buffer(2)]] )
{
    float3 transformPos = float3(positions[ vertexId ]);
    transformPos[0] = transformPos[0] + viewportOffset[0][0];
    transformPos[1] = transformPos[1] + viewportOffset[0][1];
    
    v2f o;
    o.position = float4( transformPos, 1.0 );
    o.color = half3 ( colors[ vertexId ] );
    return o;
}

half4 fragment render2DFragmentMain( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}

