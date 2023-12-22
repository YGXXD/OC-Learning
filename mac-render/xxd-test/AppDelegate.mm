#import "AppDelegate.h"
#import <QuartzCore/QuartzCore.h>
#import "TestView.h"

@interface AppDelegate ()
{
    NSWindow* window; // window关闭时会自动release
}

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    NSRect rect;
    rect.origin.x = 100;
    rect.origin.y = 100;
    rect.size.width = 1280;
    rect.size.height = 720;
    
    self->window = [[NSWindow alloc] initWithContentRect:rect styleMask:NSWindowStyleMaskClosable|NSWindowStyleMaskTitled backing:NSBackingStoreBuffered defer:false];
    
    NSLog(@"launch");
    
    TestView* testView = [[TestView alloc] initWithFrame:rect];
    
	[self->window setContentView:testView];
    [self->window setTitle:@"xxd"];
    [self->window makeKeyWindow];
    [self->window orderFrontRegardless];
	
	[testView release];
    
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    NSLog(@"Application Destroy");
    return YES;
}

@end
