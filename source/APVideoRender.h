//
//  APVideoRender.h
//  MediaStreamPlayer
//
//  Created by 胡校明 on 2019/3/1.
//  Copyright © 2019 freecoder. All rights reserved.
//

#import <GLKit/GLKit.h>
#import "MSGLHanlder.hpp"
#import "FFCodecContext.hpp"
#import "APCodecContext.hpp"

using namespace MS;
using namespace MS::APhard;

NS_ASSUME_NONNULL_BEGIN

@interface APVideoRender : NSObject<GLKViewDelegate>

@property (readonly) CGRect drawRect;

/**
 OpenGL es 渲染器构造器

 @param targetView 目标视图
 @param rect 范围
 @param lock EAGLContext 访问锁, 防止多个 EAGLContext 之间数据错误, 单个可以忽略
 @return APVideoRender
 */
+ (instancetype)renderTo:(UIView * MSNonnull)targetView
                drawRect:(CGRect)rect
                syncLock:(NSLock * MSNullable)lock;

/**
 刷新渲染范围

 @param rect 范围
 */
- (void)updateDrawRect:(CGRect)rect;

/**
 渲染 FFmpeg 软解视频数据

 @param frame AVFrame
 */
- (void)displayAVFrame:(AVFrame &)frame;

/**
 渲染 iOS 硬解视频数据

 @param frame APFrame
 */
- (void)displayAPFrame:(APFrame &)frame;

@end

NS_ASSUME_NONNULL_END