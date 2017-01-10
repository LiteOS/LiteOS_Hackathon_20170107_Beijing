//
//  HAMDeliverySucceedVC.m
//  HAMDriverlessCarController
//
//  Created by DaiYue’s Macbook on 2017/1/8.
//  Copyright © 2017年 Find the Lamp Studio. All rights reserved.
//

#import "HAMDeliverySucceedVC.h"
#import "SVProgressHUD.h"

@interface HAMDeliverySucceedVC ()
@property (weak, nonatomic) IBOutlet UIButton *passwordButton;

@end

@implementation HAMDeliverySucceedVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (IBAction)viewCodeAction:(id)sender {
    [self.passwordButton setTitle:@"开箱密码：527413" forState:UIControlStateNormal];
}

- (IBAction)reviewAction:(id)sender {
    [SVProgressHUD showSuccessWithStatus:@"碰碰博士感受到了你的爱，谢谢：）"];
}

- (IBAction)dismissAction:(id)sender {
    [self.navigationController popToRootViewControllerAnimated:YES];
}

@end
