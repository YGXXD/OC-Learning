//
//  RenderView.m
//  ios-render
//
//  Created by chenqiudu on 2023/7/18.
//

#import "RenderView.h"
#include "MetalRender.hpp"
#include "MetalCamera.hpp"

@interface RenderView () <MTKViewDelegate>
{
    xxd::Camera* cam;
    xxd::Renderer* render;
}

@end

@implementation RenderView

- (instancetype)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if(self != nil){
        //self.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
        float asc = frame.size.width / frame.size.height;
        cam = new xxd::Camera(asc);
        render = new xxd::Renderer(cam, (__bridge CA::MetalLayer*)self.layer);
        self.delegate = self;
        self.backgroundColor = UIColor.whiteColor;
    }
    
    return self;
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size{
    
}

- (void)drawInMTKView:(nonnull MTKView *)view{
    render->Draw();
}

- (void)dealloc{
    delete render;
    delete cam;
}

@end

