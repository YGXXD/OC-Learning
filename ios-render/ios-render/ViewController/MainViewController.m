//
//  ViewController.m
//  ios-render
//
//  Created by chenqiudu on 2023/7/18.
//

#import "MainViewController.h"
#import "RenderView.h"

@interface MainViewController ()

@end

@implementation MainViewController

- (void)loadView{
    RenderView* renderView = [[RenderView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.view = renderView;

}


@end
