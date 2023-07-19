//
//  BoxShader.metal
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#include <metal_stdlib>
using namespace metal;

#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

struct vin
{
    float3 position;
    float3 color;
};

v2f vertex vertexMain( uint vertexId [[vertex_id]],
                       device const vin* vertexData [[buffer(0)]],
                       device const float4x4* cameraData [[buffer(1)]] )
{
    v2f o;
    o.position = float4( vertexData[ vertexId ].position, 1.0 ) * cameraData[0];
    o.color = half3 ( vertexData[ vertexId ].color );
    return o;
}

half4 fragment fragmentMain( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}
