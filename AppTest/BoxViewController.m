//
//  BoxViewController.m
//  AppTest
//
//  Created by chenqiudu on 2023/7/11.
//

#import "BoxViewController.h"
#import "XXdView.h"
#import "XXdMath.h"

@interface BoxViewController() <MTKViewDelegate>
{
    id<MTLDevice> xDevice;
    id<MTLCommandQueue> xCmdQueue;
    id<MTLRenderPipelineState> xPSO;
    id<MTLBuffer> vertexBuffer;
    id<MTLBuffer> indexBuffer;
    id<MTLBuffer> cameraBuffer;
    
    CGFloat aspect;
    simd_float2 touchScreenPoint;
    
    // Camera Data
    float raduis;
    float theta;
    float phi;
}

@end

@implementation BoxViewController

- (void)loadView{
    // init metal
    xDevice = MTLCreateSystemDefaultDevice();
    XXdView* xView = [[XXdView alloc] initWithFrame:[UIScreen mainScreen].bounds device:xDevice];
    xView.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    xView.clearColor = MTLClearColorMake(0.9, 0.9, 1, 1);
    xView.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
    xView.clearDepth = 1.f;
    xView.delegate = self;
    xView.titleText.text = @"Chenqiudu's Metal Box";
    self.view = xView;
    xCmdQueue = [xDevice newCommandQueue];
    
    aspect = xView.drawableSize.width / xView.drawableSize.height;
    
    // build vertex buffer
    float s = 1.f;
    
    simd_float2x3 verts[] = {
        (simd_float2x3){(simd_float3){ -s, -s, +s }, (simd_float3){ 1, 0, 0 }},
        (simd_float2x3){(simd_float3){ +s, -s, +s }, (simd_float3){ 0, 1, 0 }},
        (simd_float2x3){(simd_float3){ +s, +s, +s }, (simd_float3){ 0, 0, 1 }},
        (simd_float2x3){(simd_float3){ -s, +s, +s }, (simd_float3){ 0, 0, 0 }},
        
        (simd_float2x3){(simd_float3){ -s, -s, -s }, (simd_float3){ 1, 1, 0 }},
        (simd_float2x3){(simd_float3){ -s, +s, -s }, (simd_float3){ 0, 1, 1 }},
        (simd_float2x3){(simd_float3){ +s, +s, -s }, (simd_float3){ 1, 0, 1 }},
        (simd_float2x3){(simd_float3){ +s, -s, -s }, (simd_float3){ 1, 1, 1 }}
    };

    uint16_t indices[] = {
        0, 1, 2, /* front */
        2, 3, 0,

        1, 7, 6, /* right */
        6, 2, 1,

        7, 4, 5, /* back */
        5, 6, 7,

        4, 0, 3, /* left */
        3, 5, 4,

        3, 2, 6, /* top */
        6, 5, 3,

        4, 7, 1, /* bottom */
        1, 0, 4
    };
    
    NSInteger vertexBufferSize = sizeof(simd_float2x3) * 8;
    NSInteger indexBufferSize = sizeof(uint16_t) * 36;
    
    vertexBuffer = [xDevice newBufferWithBytes:verts length:vertexBufferSize options:MTLResourceStorageModeShared];
    indexBuffer = [xDevice newBufferWithBytes:indices length:indexBufferSize options:MTLResourceStorageModeShared];
    
    // init camera
    raduis = 20;
    theta = 0;
    phi = M_PI / 2.f;
    
    // build camera buffer
    simd_float3 eyePos = { raduis * sinf(phi) * cosf(theta), raduis * sinf(phi) * sinf(theta), raduis * cosf(phi) };
    simd_float4x4 perspMat = makePerspective(M_PI * 0.25, aspect, 3, 10000);
    simd_float4x4 viewMat = makeViewLookAt(eyePos, (simd_float3){0, 0, 0}, (simd_float3){0, 0, 1});
    simd_float4x4 cameraMat = simd_mul(viewMat, perspMat); // 变换投影矩阵

    NSInteger cameraBufferSiaze = sizeof(simd_float4x4);
    cameraBuffer = [xDevice newBufferWithBytes:&cameraMat length:cameraBufferSiaze options:MTLResourceStorageModeShared];
  
//    // DebugLog
//    NSLog(@"%f,%f,%f", eyePos.x, eyePos.y, eyePos.z);
//    NSLog(@"\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f",
//          viewMat.columns[0].x, viewMat.columns[1].x, viewMat.columns[2].x, viewMat.columns[3].x,
//          viewMat.columns[0].y, viewMat.columns[1].y, viewMat.columns[2].y, viewMat.columns[3].y,
//          viewMat.columns[0].z, viewMat.columns[1].z, viewMat.columns[2].z, viewMat.columns[3].z,
//          viewMat.columns[0].w, viewMat.columns[1].w, viewMat.columns[2].w, viewMat.columns[3].w);
//
//    NSLog(@"\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f",
//          perspMat.columns[0].x, perspMat.columns[1].x, perspMat.columns[2].x, perspMat.columns[3].x,
//          perspMat.columns[0].y, perspMat.columns[1].y, perspMat.columns[2].y, perspMat.columns[3].y,
//          perspMat.columns[0].z, perspMat.columns[1].z, perspMat.columns[2].z, perspMat.columns[3].z,
//          perspMat.columns[0].w, perspMat.columns[1].w, perspMat.columns[2].w, perspMat.columns[3].w);
//
//    NSLog(@"\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f",
//          cameraMat.columns[0].x, cameraMat.columns[1].x, cameraMat.columns[2].x, cameraMat.columns[3].x,
//          cameraMat.columns[0].y, cameraMat.columns[1].y, cameraMat.columns[2].y, cameraMat.columns[3].y,
//          cameraMat.columns[0].z, cameraMat.columns[1].z, cameraMat.columns[2].z, cameraMat.columns[3].z,
//          cameraMat.columns[0].w, cameraMat.columns[1].w, cameraMat.columns[2].w, cameraMat.columns[3].w);
//
//    simd_float4 pos = simd_mul((simd_float4){0, 0, 0, 1}, cameraMat);
//    pos /= pos.w;
//    NSLog(@"\n%f, %f, %f, %f", pos.x, pos.y, pos.z, pos.w);
    
    // build pso
    id<MTLLibrary> xLibrary = [xDevice newDefaultLibrary];
    id<MTLFunction> vFunc = [xLibrary newFunctionWithName:@"boxVertexMain"];
    id<MTLFunction> fFunc = [xLibrary newFunctionWithName:@"boxFragmentMain"];
    
    MTLRenderPipelineDescriptor* xPSODesc = [[MTLRenderPipelineDescriptor alloc] init];
    [xPSODesc colorAttachments][0].pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    xPSODesc.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
    xPSODesc.vertexFunction = vFunc;
    xPSODesc.fragmentFunction = fFunc;
    
    xPSO = [xDevice newRenderPipelineStateWithDescriptor:xPSODesc error:nil];
    
}

#pragma MTKViewDelegate
- (void)drawInMTKView:(nonnull MTKView *)view{
    id<MTLCommandBuffer> pCmd = [xCmdQueue commandBuffer];
    MTLRenderPassDescriptor* pRpd = [view currentRenderPassDescriptor];
    
    id<MTLRenderCommandEncoder> pEnc = [pCmd renderCommandEncoderWithDescriptor:pRpd];
    
//    [pEnc setViewport:(MTLViewport){xViewportDrawable.origin.x, xViewportDrawable.origin.y, xViewportDrawable.size.width, xViewportDrawable.size.height, -1, 1}];
//    [pEnc setScissorRect:(MTLScissorRect){xViewportDrawable.origin.x, xViewportDrawable.origin.y, xViewportDrawable.size.width, xViewportDrawable.size.height}];
    
    [pEnc setCullMode:MTLCullModeBack];
    [pEnc setFrontFacingWinding:MTLWindingClockwise];
    [pEnc setRenderPipelineState:xPSO];
    [pEnc setVertexBuffer:vertexBuffer offset:0 atIndex:0];
    [pEnc setVertexBuffer:cameraBuffer offset:0 atIndex:1];
    [pEnc drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:36 indexType:MTLIndexTypeUInt16 indexBuffer:indexBuffer indexBufferOffset:0];
    [pEnc endEncoding];
    [pCmd presentDrawable:[view currentDrawable]];
    [pCmd commit];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size{
    
}


#pragma touch event
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    if(touches.count){
        CGPoint point = [[touches allObjects][0] locationInView:self.view];
        touchScreenPoint = (simd_float2){point.x, point.y};
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    if(touches.count){
        CGPoint point = [[touches allObjects][0] locationInView:self.view];
        
        simd_float2 currScreenPoint = (simd_float2){point.x, point.y};
        theta += -(touchScreenPoint.x - currScreenPoint.x) / 500;
        phi += (touchScreenPoint.y - currScreenPoint.y) / 500;
        phi = simd_clamp(phi, 0.01f, (float)M_PI - 0.01f);
        touchScreenPoint = currScreenPoint;
        
        simd_float3 eyePos = { raduis * sinf(phi) * cosf(theta), raduis * sinf(phi) * sinf(theta), raduis * cosf(phi) };
        simd_float4x4 perspMat = makePerspective(M_PI * 0.25, aspect, 3, 10000);
        simd_float4x4 viewMat = makeViewLookAt(eyePos, (simd_float3){0, 0, 0}, (simd_float3){0, 0, 1});
        simd_float4x4 cameraMat = simd_mul(viewMat, perspMat); // 变换投影矩阵
        
        simd_float4x4* cameraMatPtr = (simd_float4x4*)cameraBuffer.contents;
        *cameraMatPtr = cameraMat;
        
//        NSLog(@"touchScreenPoint: %f, %f", touchScreenPoint[0], touchScreenPoint[1]);
//        NSLog(@"deltaScreenPoint: %f, %f", deltaScreenPoint[0], deltaScreenPoint[1]);
//        NSLog(@"deltaViewportPoint: %f, %f", (*deltaViewportPoint)[0], (*deltaViewportPoint)[1]);
    }
}
@end
