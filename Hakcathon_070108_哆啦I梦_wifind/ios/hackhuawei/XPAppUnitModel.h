//
//  XPAppUnitModel.h
//  LUniteSDK
//
//  Created by 搜影科技 on 16/9/20.
//  Copyright © 2016年 sykj. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface XPAppUnitModel : NSObject

//bundid
@property(nonatomic,copy)NSString *bundleid;

//可执行文件名
@property(nonatomic,copy)NSString *bExecutable;

//app名称－－－注：有的app不存在，获取不到
@property(nonatomic,copy)NSString *iName;


@end
