//
//  MainUI.m
//  AppTest
//
//  Created by chenqiudu on 2023/7/10.
//

#import "MainView.h"

@implementation MainView

//@synthesize titleText;
//@synthesize enterButton;
@synthesize tableView;

- (instancetype)initWithFrame:(CGRect)frame{
    if(self = [super initWithFrame:frame]){
//        CGFloat centerX = [UIScreen mainScreen].bounds.size.width / 2;
        
//        titleText = [[UILabel alloc] initWithFrame:CGRectMake(centerX - 150, 50, 300, 100)];
//        titleText.text = @"Chenqiudu's App Test";
//        titleText.font = [UIFont systemFontOfSize:20];
//        titleText.textAlignment = NSTextAlignmentCenter;
//        [self addSubview:titleText];
        
//        enterButton = [[UIButton alloc] initWithFrame:CGRectMake(centerX - 50, 300, 100, 100)];
//        enterButton.backgroundColor = UIColor.blueColor;
//        [enterButton setTitle:@"Enter" forState:UIControlStateNormal];
//        [self addSubview:enterButton];
        
        tableView = [[UITableView alloc] initWithFrame:self.bounds];
        tableView.backgroundColor = UIColor.whiteColor;
        tableView.separatorColor = UIColor.greenColor;
        tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
        
        [self addSubview:tableView];
    }
    
    return self;
}

//- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event{
//    NSLog(@"MainView HitTest");
//    UIView* view = [super hitTest:point withEvent:event];
//    
//    return view;
//}

@end
