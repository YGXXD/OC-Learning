//
//  ViewController.m
//  AppTest
//
//  Created by chenqiudu on 2023/7/10.
//

#import "MainViewController.h"
#import "MainView.h"
#import "TriangleViewController.h"
#import "BoxViewController.h"

@interface MainViewController() <UITableViewDataSource, UITableViewDelegate>
{
    // Test Data, 0:Render, 1:Other
    NSMutableArray<NSMutableArray*>* sectionNameData;
    NSMutableArray<NSString*>* groupNameData;
}

@end

@implementation MainViewController

- (void)loadView{
    MainView* mainView = [[MainView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    mainView.tableView.dataSource = self;
    mainView.tableView.delegate = self;
    self.view = mainView;
    self.navigationItem.title = @"Chenqiudu's App Test";
    self.navigationItem.backButtonTitle = @"Back";
    
    NSArray* defaultRenderNameData = @[@"Triangle", @"Box", @"Sphere"];
    NSArray* defaultOtherNameData = @[@"Other One", @"Other Two"];
    
    sectionNameData = [[NSMutableArray alloc] init];
    [sectionNameData addObject:[[NSMutableArray alloc] initWithArray:defaultRenderNameData]]; // 加入RenderNames
    [sectionNameData addObject:[[NSMutableArray alloc] initWithArray:defaultOtherNameData]]; // 加入OtherNames
    
    NSArray* defaultGroupNameData = @[@"Render", @"Other"];
    groupNameData = [[NSMutableArray alloc] initWithArray:defaultGroupNameData];
    
}

#pragma UITableViewDataSource Implementation
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    // 返回每组行数
    return sectionNameData[section].count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    // 以组名标记对象池
    NSString* groupName = groupNameData[indexPath.section];
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:groupName];
    if(cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:groupName];
    }
    cell.textLabel.text = sectionNameData[indexPath.section][indexPath.row];
    cell.imageView.image = nil;
    cell.detailTextLabel.text = groupName;
    cell.detailTextLabel.font = [UIFont systemFontOfSize:8];
    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    
    // 返回UITableViewCell
    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    // 返回组数
    return sectionNameData.count;
}

- (nullable NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section{
    // 返回组名
    return groupNameData[section];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    NSString* selectName = sectionNameData[indexPath.section][indexPath.row];
    if([selectName isEqualToString:@"Triangle"]){
        [self.navigationController pushViewController:[[TriangleViewController alloc] init] animated:true];
    }
    else if([selectName isEqualToString:@"Box"]){
        [self.navigationController pushViewController:[[BoxViewController alloc] init] animated:true];
    }
}

@end
