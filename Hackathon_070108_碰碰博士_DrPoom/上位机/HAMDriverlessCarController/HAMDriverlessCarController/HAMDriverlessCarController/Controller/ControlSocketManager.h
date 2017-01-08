//
//  ControlSocketManager.h
//  WifiPlatform
//
//  Created by 赵嵩阳 on 16/3/29.
//  Copyright © 2016年 Robot Time. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ControlSocketManager : NSObject

+ (ControlSocketManager *)sharedManager;

- (void)connect;
- (void)disConnect;
- (BOOL)sendData:(NSData*)data withTimeout:(NSTimeInterval)timeout tag:(long)tag;

@end
