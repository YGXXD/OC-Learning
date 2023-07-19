//
//  XXdViewController.m
//  AppTest
//
//  Created by chenqiudu on 2023/7/10.
//

#import "TriangleViewController.h"
#import "XXdView.h"
#import <Metal/Metal.h>
#import <simd/simd.h>

@interface TriangleViewController() <MTKViewDelegate>
{
    id<MTLDevice> xDevice;
    id<MTLCommandQueue> xCmdQueue;
    id<MTLRenderPipelineState> xPSO;
    id<MTLBuffer> positionBuffer;
    id<MTLBuffer> colorBuffer;
    id<MTLBuffer> transformBuffer; // viewport offset
    CGRect xViewportDrawable;
    simd_float2 xDrawbleToScreenVecScale;
    
    simd_float2 touchScreenPoint;
    simd_float2 deltaScreenPoint;
}

@end


@implementation TriangleViewController

- (void)loadView{
    // init metal
    xDevice = MTLCreateSystemDefaultDevice();
    XXdView* xView = [[XXdView alloc] initWithFrame:[UIScreen mainScreen].bounds device:xDevice];
    xView.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    xView.clearColor = MTLClearColorMake(0.9, 0.9, 1, 1);
    xView.delegate = self;
    xView.titleText.text = @"Chenqiudu's Metal Triangle";
    self.view = xView;
    xCmdQueue = [xDevice newCommandQueue];
    
    // calc viewPort
    xViewportDrawable.origin.x = 0;
    xViewportDrawable.origin.y = xView.drawableSize.height / 4;
    xViewportDrawable.size.width = xView.drawableSize.width;
    xViewportDrawable.size.height = xView.drawableSize.height / 2;
    xDrawbleToScreenVecScale.x = xView.bounds.size.width / xView.drawableSize.width;
    xDrawbleToScreenVecScale.y = xView.bounds.size.height / xView.drawableSize.height;
    
    // build vertex buffer
    simd_float3 positions[3] =
       {
           { -0.8f,  0.8f, 0.0f },
           {  0.0f, -0.8f, 0.0f },
           { +0.8f,  0.8f, 0.0f }
       };

    simd_float3 colors[3] =
       {
           {  1.f, 0.f, 0.f },
           {  0.f, 1.f, 0.f },
           {  0.f, 0.f, 1.f }
       };
    
    NSInteger positionBufferSize = sizeof(simd_float3) * 3;
    NSInteger colorBufferSize = sizeof(simd_float3) * 3;
    
    positionBuffer = [xDevice newBufferWithBytes:positions length:positionBufferSize options:MTLResourceStorageModeShared];
    colorBuffer = [xDevice newBufferWithBytes:colors length:colorBufferSize options:MTLResourceStorageModeShared];
    
    // build upload buffer
    NSInteger transformBufferSize = sizeof(simd_float2);
    transformBuffer = [xDevice newBufferWithLength:transformBufferSize options:MTLResourceStorageModeShared];
    simd_float2 defaultViewportOffset = {0, 0};
    memcpy([transformBuffer contents], &defaultViewportOffset, transformBufferSize);
    
    // build pso
    id<MTLLibrary> xLibrary = [xDevice newDefaultLibrary];
    id<MTLFunction> vFunc = [xLibrary newFunctionWithName:@"triangleVertexMain"];
    id<MTLFunction> fFunc = [xLibrary newFunctionWithName:@"triangleFragmentMain"];
    
    MTLRenderPipelineDescriptor* xPSODesc = [[MTLRenderPipelineDescriptor alloc] init];
    [xPSODesc colorAttachments][0].pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    xPSODesc.vertexFunction = vFunc;
    xPSODesc.fragmentFunction = fFunc;
    
    xPSO = [xDevice newRenderPipelineStateWithDescriptor:xPSODesc error:nil];
}


#pragma MTKViewDelegate
- (void)drawInMTKView:(nonnull MTKView *)view{
    id<MTLCommandBuffer> pCmd = [xCmdQueue commandBuffer];
    MTLRenderPassDescriptor* pRpd = [view currentRenderPassDescriptor];
    
    id<MTLRenderCommandEncoder> pEnc = [pCmd renderCommandEncoderWithDescriptor:pRpd];
    
    [pEnc setViewport:(MTLViewport){xViewportDrawable.origin.x, xViewportDrawable.origin.y, xViewportDrawable.size.width, xViewportDrawable.size.height, -1, 1}];
    [pEnc setScissorRect:(MTLScissorRect){static_cast<NSUInteger>((xViewportDrawable.origin.x)), static_cast<NSUInteger>((xViewportDrawable.origin.y)), static_cast<NSUInteger>((xViewportDrawable.size.width)), static_cast<NSUInteger>((xViewportDrawable.size.height))}];
    [pEnc setCullMode:MTLCullModeBack];
    [pEnc setFrontFacingWinding:MTLWindingCounterClockwise];
    [pEnc setRenderPipelineState:xPSO];
    [pEnc setVertexBuffer:positionBuffer offset:0 atIndex:0];
    [pEnc setVertexBuffer:colorBuffer offset:0 atIndex:1];
    [pEnc setVertexBuffer:transformBuffer offset:0 atIndex:2];
    [pEnc drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];

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
        touchScreenPoint = (simd_float2){static_cast<float>((point.x)), static_cast<float>((point.y))};
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    if(touches.count){
        CGPoint point = [[touches allObjects][0] locationInView:self.view];
        
        simd_float2 currScreenPoint = (simd_float2){static_cast<float>((point.x)), static_cast<float>((point.y))};
        deltaScreenPoint += touchScreenPoint - currScreenPoint;
        touchScreenPoint = currScreenPoint;
        
        simd_float2 transformOffset = deltaScreenPoint / ((simd_float2){static_cast<float>(-(xViewportDrawable.size.width)), static_cast<float>((xViewportDrawable.size.height))} * xDrawbleToScreenVecScale);
        
        simd_float2* deltaViewportPoint = (simd_float2*)transformBuffer.contents;
        *deltaViewportPoint = transformOffset;
        
//        NSLog(@"touchScreenPoint: %f, %f", touchScreenPoint[0], touchScreenPoint[1]);
//        NSLog(@"deltaScreenPoint: %f, %f", deltaScreenPoint[0], deltaScreenPoint[1]);
//        NSLog(@"deltaViewportPoint: %f, %f", (*deltaViewportPoint)[0], (*deltaViewportPoint)[1]);
    }
}
@end
