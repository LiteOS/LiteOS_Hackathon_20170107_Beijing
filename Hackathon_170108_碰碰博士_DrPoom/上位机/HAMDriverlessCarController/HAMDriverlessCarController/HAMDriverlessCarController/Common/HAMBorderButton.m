
//
//  HAMBorderButton.m
//  HAMDriverlessCarController
//
//  Created by DaiYue’s Macbook on 2017/1/7.
//  Copyright © 2017年 Find the Lamp Studio. All rights reserved.
//

#import "HAMBorderButton.h"
#import "UIColor+WNYUtils.h"

@implementation HAMBorderButton

- (void)awakeFromNib {
    [super awakeFromNib];
    self.layer.cornerRadius = 4.0f;
    self.layer.borderWidth = 1.0f;
    self.layer.borderColor = [[UIColor colorWithHex:0x87b345] CGColor];
}

@end
