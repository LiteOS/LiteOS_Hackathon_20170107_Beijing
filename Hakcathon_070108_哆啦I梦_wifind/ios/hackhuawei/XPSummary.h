//
//  CLSummary.h
//  LUniteSDK
//
//  Created by 搜影科技 on 16/9/20.
//  Copyright © 2016年 sykj. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface XPSummary : NSObject


///验证方法
//+(void)XPVerify:(NSString *)vkey;

/**
 *  所有安装列表
 *
 *  @return 列表信息-XPAppUnitModel
 */
+(NSArray *)appUnitArray;

/**
 *  通过bundid打开app（跟通过URL Scheme打开略有不同自己体会）
 *
 *  @param bundstring bundid
 *
 *  @return 是否成功
 */
+ (BOOL)talkAppBid:(NSString *)bundstring;

/**
 *  获取mac地址 <⚠️：仅在wifi下可检测mac，但不敢保证完全正常>
 *
 *  @param resultBlock 回调ip地址和对应mac地址字符串
 */
+(void)startSln:(void (^)(NSString *ipStr,NSString *macStr))resultBlock;






@end
