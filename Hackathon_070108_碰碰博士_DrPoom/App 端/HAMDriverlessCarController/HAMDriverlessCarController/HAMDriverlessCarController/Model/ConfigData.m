//
//  ConfigData.m
//  WifiPlatform
//
//  Created by 赵嵩阳 on 16/3/29.
//  Copyright © 2016年 Robot Time. All rights reserved.
//

#import "ConfigData.h"

@implementation ConfigData

static NSString *hostIp = @"192.168.8.1";
static NSString *moviePort = @"8083";
static int16_t controlPort = 2001;

+ (NSString*)movieIp {
    return hostIp;
}
+ (NSString*)moviePort {
    return moviePort;
}
+ (NSString*)movieURL {
    return [NSString stringWithFormat:@"http://%@:%@/?action=stream", hostIp, moviePort];
}

+ (NSString*)controlHost {
    return hostIp;
}
+ (int16_t)controlPort {
    return controlPort;
}

+ (void)setHostIp:(NSString *)ip {
    hostIp = ip;
}
+ (void)setMoviePort:(NSString *)port {
    moviePort = port;
}
+ (void)setControlPort:(int16_t)port {
    controlPort = port;
}

@end
