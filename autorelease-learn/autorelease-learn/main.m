//
//  main.m
//  autorelease-learn
//
//  Created by chenqiudu on 2023/8/4.
//

#import <Cocoa/Cocoa.h>

bool quit;

@interface MacWindowDelegate : NSObject<NSWindowDelegate>

//@property(readonly) xxd::SingleDelegate<void, xxd::Event&> eventCallback;

@end

@implementation MacWindowDelegate
- (void)windowWillClose:(NSNotification *)notification
{
    NSLog(@"窗口关闭");
    quit = true;
}

- (void)windowDidResize:(NSNotification *)notification
{
    NSLog(@"窗口大小改变");
}

-(void)dealloc
{
    [super dealloc];
    NSLog(@"代理被销毁");
    
}

@end

@interface XXNS : NSWindow


@end

@implementation XXNS

- (void)dealloc
{
    [super dealloc];
    NSLog(@"NSWindow销毁");
}

@end

XXNS* window;

void fun(void)
{
    @autoreleasepool {
        
        NSRect rect;
        rect.origin.x = 100;
        rect.origin.y = 100;
        rect.size.width = 1280;
        rect.size.height = 720;
        
        window = [[XXNS alloc] initWithContentRect:rect
                                         styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                                           backing:NSBackingStoreBuffered defer:false];
        
        NSString* title = @"xxd";
        [window setTitle:title];
        [window makeKeyWindow];
        [window orderFrontRegardless];
        [window setBackgroundColor:NSColor.whiteColor];
        
        MacWindowDelegate* del = [[MacWindowDelegate alloc] init];
        [window setDelegate:del];
        NSLog(@"%d", (int)del.retainCount); // 引用5
        [del release];
        NSLog(@"%d", (int)del.retainCount); // 引用4
    } //销毁del
    
    // 如果这里没有autoreleasepool的话，会在main函数执行完才会销毁delegate
}

int main(int argc, const char * argv[]) {
    

//        [NSApplication sharedApplication];
//        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
//        [NSApp finishLaunching];
//        [NSApp activateIgnoringOtherApps:true];
    
    @autoreleasepool {
         
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp finishLaunching];
        [NSApp activateIgnoringOtherApps:true];
        
        fun();
        
        // nil 会指定 [NSDate distantPast]
        while (!quit)
        {
            NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:true];
            while(event != nil)
            {
                [NSApp sendEvent:event];
                event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:true];
            }
        }
        
    }
    return 0;
}
