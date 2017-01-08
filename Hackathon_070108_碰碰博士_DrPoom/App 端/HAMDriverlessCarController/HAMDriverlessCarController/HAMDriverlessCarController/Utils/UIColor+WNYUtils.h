//
//  UIColor+Utils.h
//  WanYi
//
//  Created by yue.dai on 15/7/9.
//  Copyright (c) 2015年 NaGoo Tech. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIColor (WNYUtils)

+ (UIColor *)colorWithHex:(NSInteger)hexValue;

/// Color with hex. Use hex as 0xffffff
+ (UIColor *)colorWithHex:(NSInteger)hexValue alpha:(CGFloat)alpha;

/// Color with ARGB. Use ARGB as 0xffffffff
+ (UIColor *)colorWithARGB:(NSInteger)ARGBValue;

/// Color with hexString. Use hexString like @"#ffffff"
+ (UIColor *)colorFromHexString:(NSString *)hexString;

/// Color with hexString
+ (UIColor *)colorFromHexString:(NSString *)hexString alpha:(CGFloat)alpha;

@end
