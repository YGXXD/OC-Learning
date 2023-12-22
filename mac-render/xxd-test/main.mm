#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"
#include "Logger.hpp"

int main(int argc, const char * argv[]) {

	DEBUG_LOG("Helow", " Chenqiudu");
    INFO_LOG("Helow", " Chenqiudu");
    WARN_LOG("Helow", " Chenqiudu");
    ERROR_LOG("Helow", " Chenqiudu");
    FATAL_LOG("Helow", " Chenqiudu");
    
    @autoreleasepool {
        AppDelegate* del = [[[AppDelegate alloc] init] autorelease];

        NSApplication* pSharedApplication = [[NSApplication sharedApplication] autorelease];
		if(pSharedApplication != nil) {
        	pSharedApplication.delegate = del;
        	[pSharedApplication run];
		}
    }
    return 0;
}
