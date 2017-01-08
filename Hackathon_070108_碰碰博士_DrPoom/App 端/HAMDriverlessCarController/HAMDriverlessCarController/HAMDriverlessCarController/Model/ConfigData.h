//
//  ConfigData.h
//  WifiPlatform
//
//  Created by 赵嵩阳 on 16/3/29.
//  Copyright © 2016年 Robot Time. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ConfigData : NSObject

+ (NSString*)movieURL;
+ (NSString*)movieIp;
+ (NSString*)moviePort;

+ (NSString*)controlHost;
+ (int16_t)controlPort;

+ (void)setHostIp:(NSString*) ip;
+ (void)setMoviePort:(NSString*) port;
+ (void)setControlPort:(int16_t) port;

@end
