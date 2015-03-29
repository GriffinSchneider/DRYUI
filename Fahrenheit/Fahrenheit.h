//
//  Fahrenheit.h
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//


#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#define FAHRENHEIT_VIEW UIView
#elif TARGET_OS_MAC
#import <AppKit/AppKit.h>
#define FAHRENHEIT_VIEW_NAME NSView
#endif


#import <Foundation/Foundation.h>
#import <Masonry/Masonry.h>


#ifndef FAHRENHEIT_VIEW_NAME
#define FAHRENHEIT_VIEW_NAME _
#endif

#ifndef FAHRENHEIT_CUSTOM_ARGS_MACRO
#define º(args...) FAHRENHEIT(args)
#endif


/*! Type of the block that gets passed as an argument to add */
typedef void (^FahrenheitAddArgumentBlock)(id FAHRENHEIT_VIEW_NAME, FAHRENHEIT_VIEW *superview);
/*! Type of the block that's bound to 'add' inside the block passed to buildSubviews */
typedef id (^FahrenheitAddBlock)(FAHRENHEIT_VIEW *view, FAHRENHEIT_VIEW *superview, FahrenheitAddArgumentBlock block);
/*! Type of the block that's passed to buildSubviews */
typedef void (^FahrenheitBuildSubviewsBlock)(FAHRENHEIT_VIEW *FAHRENHEIT_VIEW_NAME, FahrenheitAddBlock add);


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface FAHRENHEIT_VIEW (Fahrenheit)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;

- (void)buildSubviews:(FahrenheitBuildSubviewsBlock)block;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (Fahrenheit)
+ (instancetype)fahrenheit_selfOrInstanceOfSelf;
- (instancetype)fahrenheit_selfOrInstanceOfSelf;
@end


#define FAHRENHEIT(args...) __FAHRENHEIT_HELPER(BOGUS, ## args)
#define __FAHRENHEIT_OVERLOADED_MACRO(_1,_2,_3,NAME,...) NAME
#define __FAHRENHEIT_HELPER(args...) __FAHRENHEIT_OVERLOADED_MACRO(args, __FAHRENHEIT_HELPER_3, __FAHRENHEIT_HELPER_2, __FAHRENHEIT_HELPER_1)(args)
#define __FAHRENHEIT_HELPER_1(BOGUS) ^(FAHRENHEIT_VIEW *FAHRENHEIT_VIEW_NAME, FahrenheitAddBlock add)
#define __FAHRENHEIT_HELPER_2(BOGUS, viewArg) __FAHRENHEIT_HELPER_3(BOGUS, viewArg, typeof([viewArg fahrenheit_selfOrInstanceOfSelf]))
#define __FAHRENHEIT_HELPER_3(BOGUS, viewArg, viewArgType) [viewArg fahrenheit_selfOrInstanceOfSelf], FAHRENHEIT_VIEW_NAME, ^(viewArgType FAHRENHEIT_VIEW_NAME, FAHRENHEIT_VIEW *superview)
