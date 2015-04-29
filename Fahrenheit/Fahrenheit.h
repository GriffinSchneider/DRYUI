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
  #define FAHRENHEIT_VIEW NSView
#endif


#import <Foundation/Foundation.h>
#import <Masonry/Masonry.h>


#ifndef FAHRENHEIT_VIEW_NAME
#define FAHRENHEIT_VIEW_NAME _
#endif

#ifndef FAHRENHEIT_CUSTOM_MACRO_NAMES
#define ºº(args...) FAHRENHEIT_TOPLEVEL(args)
#define º(args...) FAHRENHEIT(args)
#endif


typedef void (^FahrenheitViewAndSuperviewBlock)(id FAHRENHEIT_VIEW_NAME, FAHRENHEIT_VIEW *superview);

////////////////////////////////////////////////////////////////////////////////////////////////////
@interface FAHRENHEIT_VIEW (Fahrenheit)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;

- (void)fahrenheit_buildSubviews:(FahrenheitViewAndSuperviewBlock)block;
- (id)fahrenheit_addViewFromBuildSubviews:(FAHRENHEIT_VIEW *)view withSuperview:(FAHRENHEIT_VIEW *)superview andBlock:(FahrenheitViewAndSuperviewBlock)block;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (Fahrenheit)
+ (instancetype)fahrenheit_selfOrInstanceOfSelf;
- (instancetype)fahrenheit_selfOrInstanceOfSelf;
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
static UIView *_fahrenheit_current_view = nil;
static UIView *_fahrenheit_current_toplevel_view = nil;

// The _FAHRENHEIT_UNIQUE macro expands to the string "_FAHRENHEIT" with the current line number appended.
// Used to generate identifiers that are "unique" to a given Fahrenheit macro invocation (assuming
// you don't invoke the macro multiple times on the same line).
#define _FAHRENHEIT_CONCATENATE_DETAIL(x, y) x##y
#define _FAHRENHEIT_CONCATENATE(x, y) _FAHRENHEIT_CONCATENATE_DETAIL(x, y)
#define _FAHRENHEIT_UNIQUE _FAHRENHEIT_CONCATENATE(_FAHRENHEIT, __LINE__)

// First, the statement formed at the end of this macro expansion will run, and *then* body_after_statement_after_macro will run.
#define _FAHRENHEIT_GOTO_HELPER(viewArg, body_after_statement_after_macro) \
FahrenheitViewAndSuperviewBlock _FAHRENHEIT_UNIQUE; \
if (1) { \
    NSAssert([NSThread isMainThread], @"Fahrenheit should only be used from the main thread!"); \
    goto _FAHRENHEIT_UNIQUE; \
} else \
    while (1) \
        if (1) { \
            body_after_statement_after_macro \
            _FAHRENHEIT_UNIQUE = nil; \
            break; \
        } else \
            _FAHRENHEIT_UNIQUE: _FAHRENHEIT_UNIQUE = ^(typeof([viewArg fahrenheit_selfOrInstanceOfSelf]) FAHRENHEIT_VIEW_NAME, FAHRENHEIT_VIEW *superview) \
            // We couldn't get execution here without GOTOing here, but once we do and this statement finishes,
            // execution will jump back up to the while(1) and then into body_after_statement_after_macro.


#define FAHRENHEIT_TOPLEVEL(viewArg) \
(^() { \
    NSAssert(_fahrenheit_current_toplevel_view == nil, @"Calls to FAHRENHEIT_TOPLEVEL should not be nested."); \
    _fahrenheit_current_toplevel_view = [viewArg fahrenheit_selfOrInstanceOfSelf]; \
    return _fahrenheit_current_toplevel_view; \
})(); \
_FAHRENHEIT_GOTO_HELPER(viewArg, \
    [_fahrenheit_current_toplevel_view fahrenheit_buildSubviews:_FAHRENHEIT_UNIQUE]; \
    _fahrenheit_current_toplevel_view = nil; \
)


#define FAHRENHEIT(viewArg) \
(^() { \
    NSAssert(_fahrenheit_current_toplevel_view, @"Calls to FAHRENHEIT must be inside a call to FAHRENHEIT_TOPLEVEL."); \
    _fahrenheit_current_view = [viewArg fahrenheit_selfOrInstanceOfSelf]; \
    return _fahrenheit_current_view; \
})(); \
_FAHRENHEIT_GOTO_HELPER(viewArg, \
    [_fahrenheit_current_toplevel_view fahrenheit_addViewFromBuildSubviews:_fahrenheit_current_view withSuperview:FAHRENHEIT_VIEW_NAME andBlock:_FAHRENHEIT_UNIQUE]; \
    _fahrenheit_current_view = nil; \
)
