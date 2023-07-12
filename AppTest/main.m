//
//  main.m
//  AppTest
//
//  Created by chenqiudu on 2023/7/10.
//

#import "AppDelegate.h"
#import <simd/simd.h>

int main(int argc, char * argv[]) {
    
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
