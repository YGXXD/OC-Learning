//
//  MetalContext.hpp
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#ifndef MetalContext_hpp
#define MetalContext_hpp

#include <string>
#include <simd/simd.h>
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "Singleton.hpp"

namespace xxd
{
typedef struct
{
    simd_float3 local;
    simd_float3 color;
}vertex;

class MetalContext : public Singleton<MetalContext>
{
public:
    MTL::Device *extracted();
    
    MetalContext();
    ~MetalContext();
    
    inline MTL::PixelFormat GetDepthFormat() const { return mtDepthFormat; }
    inline MTL::PixelFormat GetRenderFormat() const { return mtRenderFormat; };
    inline MTL::Device* GetDevice() const { return mtDevice; }
    inline MTL::CommandQueue* GetCmdQueue() const { return mtCmdQueue; }
    inline MTL::Buffer* GetVertexBuffer() const { return mtVertexBuffer; }
    inline MTL::Buffer* GetIndexBuffer() const { return mtIndexBuffer; }
    inline MTL::Buffer* GetCameraBuffer() const { return mtCameraBuffer; }
    inline MTL::RenderPipelineState* GetPSO() const { return mtPSO; }
    
    bool CreateGeometry(uint16_t vertexNum, const vertex* vertexData, uint16_t indexNum, const uint16_t* indexData);
    
private:
    MTL::PixelFormat mtDepthFormat;
    MTL::PixelFormat mtRenderFormat;
    MTL::Device* mtDevice;
    MTL::CommandQueue* mtCmdQueue;
    MTL::Buffer* mtVertexBuffer;
    MTL::Buffer* mtIndexBuffer;
    MTL::Buffer* mtCameraBuffer;
    MTL::RenderPipelineState* mtPSO;
};

};

#endif /* MetalContext_hpp */
