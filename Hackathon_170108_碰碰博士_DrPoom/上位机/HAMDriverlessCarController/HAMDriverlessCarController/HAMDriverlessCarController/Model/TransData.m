//
//  TransData.m
//  WifiPlatform
//
//  Created by 赵嵩阳 on 16/3/30.
//  Copyright © 2016年 Robot Time. All rights reserved.
//

#import "TransData.h"

//起始位 [FF,00] / 结束位 [00,FF]
#define kTRANS_BEGIN_SIGNAL 0xFF,0x00
#define kTRANS_END_SIGNAL 0x00,0xFF

//连接指令 [E0]
#define kPIN_TYPE 0xE0

//移动指令 [E1,[D0(Vx),(00~C8)],[D1(Vy),(00~C8)],[D2(w),(00~C8)]]
#define kMOVE_TYPE 0xE1
#define kMOVE_VX 0xD0
#define kMOVE_VY 0xD1
#define kMOVE_W 0xD2

//移动到指令 [E2,[D0(x),(00~9F)],[D1(y),(00~77)]]
#define kMOVETO_TYPE 0xE1
#define kMOVETO_X 0xD0
#define kMOVETO_Y 0xD1

@implementation TransData

+ (NSData*)pinCommand {
    Byte byteArray[] = {kTRANS_BEGIN_SIGNAL, kPIN_TYPE, kTRANS_END_SIGNAL};
    return [NSData dataWithBytes:byteArray length:sizeof(byteArray)];
}

+ (NSData*)moveCommandWithVx:(Byte)vx Vy:(Byte)vy W:(Byte)w {
    if (vx > 0xC8 || vy > 0xC8 || w > 0xC8) {
        return nil;
    }
    //Byte byteArray[] = {kTRANS_BEGIN_SIGNAL, kMOVE_TYPE, kMOVE_VX, vx, kMOVE_VY, vy, kMOVE_W, w, kTRANS_END_SIGNAL};
    Byte byteArray[] = {kTRANS_BEGIN_SIGNAL, kMOVE_TYPE, vx, vy, w, kTRANS_END_SIGNAL};
    return [NSData dataWithBytes:byteArray length:sizeof(byteArray)];
}

+ (NSData*)moveToCommandWithX:(Byte)x Y:(Byte)y {
    if (x > 0x9F || y > 0x77) {
        return nil;
    }
    Byte byteArray[] = {kTRANS_BEGIN_SIGNAL, kMOVETO_TYPE, kMOVETO_X, x, kMOVETO_Y, y, kTRANS_END_SIGNAL};
    return [NSData dataWithBytes:byteArray length:sizeof(byteArray)];
}

@end
