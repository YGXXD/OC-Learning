//
//  RenderView.m
//  AppTest
//
//  Created by chenqiudu on 2023/7/10.
//

#import "XXdView.h"

@implementation XXdView

@synthesize titleText;

- (instancetype)initWithFrame:(CGRect)frameRect device:(id<MTLDevice>)device{
    if(self = [super initWithFrame:frameRect device:device]){
        
//        NSLog(@"DrawavleSize:%f, %f", self.drawableSize.width, self.drawableSize.height);
//        NSLog(@"MtkViewBounds:%f, %f", self.bounds.size.width, self.bounds.size.height);
        
        CGFloat centerX = [UIScreen mainScreen].bounds.size.width / 2;
        CGFloat buttomY = [UIScreen mainScreen].bounds.size.height;
        
        titleText = [[UILabel alloc] initWithFrame:CGRectMake(centerX - 150, buttomY - 100, 300, 100)];
        titleText.font = [UIFont systemFontOfSize:20];
        titleText.textAlignment = NSTextAlignmentCenter;
        
        [self addSubview:titleText];
    }
    
    return self;
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event{
    return self;
}

@end
