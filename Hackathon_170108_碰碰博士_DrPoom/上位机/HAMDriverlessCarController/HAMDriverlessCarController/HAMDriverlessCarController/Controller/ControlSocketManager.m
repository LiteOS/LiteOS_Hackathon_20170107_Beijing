//
//  ControlSocketManager.m
//  WifiPlatform
//
//  Created by 赵嵩阳 on 16/3/29.
//  Copyright © 2016年 Robot Time. All rights reserved.
//

#import "ControlSocketManager.h"

#import "GCDAsyncSocket.h"

#import "ConfigData.h"

@interface ControlSocketManager() <GCDAsyncSocketDelegate>

@end

@implementation ControlSocketManager{
    GCDAsyncSocket *clientSocket;
    BOOL socketState;
}

+ (ControlSocketManager *)sharedManager
{
    static ControlSocketManager *controlSocketManagerInstance = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        controlSocketManagerInstance = [[self alloc] init];
    });
    return controlSocketManagerInstance;
}

- (id)init {
    if (self = [super init]) {
        clientSocket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
        socketState = NO;
    }
    return self;
}

- (void)connect {
    [clientSocket connectToHost:[ConfigData controlHost] onPort:[ConfigData controlPort] error:nil];
}

- (void)disConnect {
    [clientSocket disconnect];
}

- (BOOL)sendData:(NSData*)data withTimeout:(NSTimeInterval)timeout tag:(long)tag {
    if (!socketState) {
        return NO;
    }
    [clientSocket writeData:data withTimeout:timeout tag:tag];
    return YES;
}

#pragma mark - GCDAsyncSocketDelegate代理方法
-(void)socket:(GCDAsyncSocket *)sock didConnectToHost:(NSString *)host port:(uint16_t)port {
    NSLog(@"连接成功--host:%@--port:%d", host, port);
    socketState = YES;
    [sock readDataWithTimeout:-1 tag:0];
}

-(void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err {
    NSLog(@"连接断开--%@", err);
    socketState = NO;
    [clientSocket connectToHost:[ConfigData controlHost] onPort:[ConfigData controlPort] error:nil];
}

-(void)socket:(GCDAsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag {
    NSString *ip = [sock connectedHost];
    
    uint16_t port = [sock connectedPort];
    
    NSString *s = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    NSLog(@"接收到服务器返回的数据 tcp [%@:%d] %@", ip, port, s);
    
    [sock readDataWithTimeout:-1 tag:tag];
}

@end
