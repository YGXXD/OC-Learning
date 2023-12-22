#ifndef RENDER_UTIL_HPP
#define RENDER_UTIL_HPP

#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

#include "XMath.hpp"

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

namespace xxd
{
typedef struct{
	simd_float4x4 worldViewPersMat;
}uniform;

typedef struct{
	simd_float3 position;	
	simd_float3 color;
}vertex;

struct MeshData
{
	std::vector<vertex> vertices;
	std::vector<uint32_t> indices;
	
	std::vector<uint16_t>& GetIndices16()
	{
		if(indices16.empty())
		{
			indices16.resize(indices.size());
			for(int i = 0; i < indices.size(); ++i)
			{
				indices16[i] = static_cast<uint16_t>(indices[i]);
			}
		}

		return indices16;
	}
	
	static MeshData GenerateBoxMesh(float width, float height, float depth, uint32_t subdivisions = 0)
	{
		MeshData boxMesh;
		float w2 = 0.5f*width;
		float h2 = 0.5f*height;
		float d2 = 0.5f*depth;
	
		boxMesh.vertices = 
		{
             {(simd_float3){ -w2, -h2, +d2 }, (simd_float3){ 1, 0, 0 }},
             {(simd_float3){ +w2, -h2, +d2 }, (simd_float3){ 0, 1, 0 }},
             {(simd_float3){ +w2, +h2, +d2 }, (simd_float3){ 0, 0, 1 }},
             {(simd_float3){ -w2, +h2, +d2 }, (simd_float3){ 0, 0, 0 }},
 
             {(simd_float3){ -w2, -h2, -d2 }, (simd_float3){ 1, 1, 0 }},
             {(simd_float3){ -w2, +h2, -d2 }, (simd_float3){ 0, 1, 1 }},
             {(simd_float3){ +w2, +h2, -d2 }, (simd_float3){ 1, 0, 1 }},
             {(simd_float3){ +w2, -h2, -d2 }, (simd_float3){ 1, 1, 1 }}
    	};

        boxMesh.indices = 
	    {
            0, 1, 2, 2, 3, 0, /* front */ 
            1, 7, 6, 6, 2, 1, /* right */ 
            7, 4, 5, 5, 6, 7, /* back */
            4, 0, 3, 3, 5, 4, /* left */
            3, 2, 6, 6, 5, 3, /* top */
            4, 7, 1, 1, 0, 4 /* bottom */
        };

		return boxMesh;
	}
	static MeshData GenerateSphereMesh(float radius, uint32_t sliceCount, uint32_t stackCount);
private:
	std::vector<uint16_t> indices16;
};

}

#endif
