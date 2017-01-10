//
//  ViewController.m
//  HAMDriverlessCarController
//
//  Created by DaiYue’s Macbook on 2017/1/7.
//  Copyright © 2017年 Find the Lamp Studio. All rights reserved.
//

#import "ViewController.h"

#import "UIColor+WNYUtils.h"

@interface ViewController ()

@property (strong, nonatomic) IBOutletCollection(UIButton) NSArray *buttons;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupViews];
}

- (void)setupViews {
    for (UIButton* button in self.buttons) {
        button.layer.borderWidth = 1.0f;
        button.layer.borderColor = [[UIColor colorWithHex:0x87b345] CGColor];
    }
}

@end
