#import "TestView.h"
#include "RenderLayer.hpp"

@interface TestView ()
{
    xxd::RenderLayer* renderer;
}

@end

@implementation TestView

- (instancetype)initWithFrame:(NSRect)frameRect{
    self = [super initWithFrame:frameRect];
    if(self != nil)
    {
        CAMetalLayer* l = (CAMetalLayer*)self.layer;
        l.drawableSize = self.drawableSize;
        NSLog(@"%f, %f", l.drawableSize.width, l.drawableSize.height);

        renderer = new xxd::RenderLayer((__bridge CA::MetalLayer*)self.layer);
        self.delegate = self;
    }
    return self;
}

- (void)dealloc{
    delete renderer;
    NSLog(@"TestView Dealloc");
	[super dealloc];
}

#pragma MTKViewDelegate
- (void)drawInMTKView:(nonnull MTKView *)view{
     renderer->Draw();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size{
    
}

@end

