//
//  HAMTourVC.m
//  HAMDriverlessCarController
//
//  Created by DaiYue’s Macbook on 2017/1/7.
//  Copyright © 2017年 Find the Lamp Studio. All rights reserved.
//

#import "HAMTourVC.h"
#import "CocoaAsyncSocket.h"

@interface HAMTourVC () <GCDAsyncSocketDelegate>

@property (nonatomic, strong) GCDAsyncSocket* socket;

@end

@implementation HAMTourVC

- (void)viewDidLoad {
    [super viewDidLoad];
    
    GCDAsyncSocket* socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
    NSError *error = nil;
    if (![socket acceptOnPort:5274 error:&error]) {
        NSLog(@"I goofed: %@", error);
    }
}

#pragma mark - Socket Delegate

- (void)socket:(GCDAsyncSocket *)sender didAcceptNewSocket:(GCDAsyncSocket *)newSocket {
}



@end
