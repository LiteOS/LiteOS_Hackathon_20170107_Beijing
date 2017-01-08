//
//  HAMInputCodeVC.m
//  HAMDriverlessCarController
//
//  Created by DaiYue’s Macbook on 2017/1/8.
//  Copyright © 2017年 Find the Lamp Studio. All rights reserved.
//

#import "HAMInputCodeVC.h"
#import "SVProgressHUD.h"

@interface HAMInputCodeVC ()

@end

@implementation HAMInputCodeVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
- (IBAction)dismissAction:(id)sender {
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- (IBAction)confirmAction:(id)sender {
    [SVProgressHUD showWithStatus:@"正在验证..."];
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC), dispatch_get_main_queue(),^{
        [SVProgressHUD showSuccessWithStatus:@"验证成功"];
        
        [self performSegueWithIdentifier:@"pushToClient" sender:self];
    });
}

@end
