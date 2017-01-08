//
//  TransData.h
//  WifiPlatform
//
//  Created by 赵嵩阳 on 16/3/30.
//  Copyright © 2016年 Robot Time. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TransData : NSObject

+ (NSData*)pinCommand;
+ (NSData*)moveCommandWithVx:(Byte)vx Vy:(Byte)vy W:(Byte)w;
+ (NSData*)moveToCommandWithX:(Byte)x Y:(Byte)y;


@end
