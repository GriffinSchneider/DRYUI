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

- (void)_fahrenheit_buildSubviews:(FahrenheitViewAndSuperviewBlock)block;
- (id)_fahrenheit_addViewFromBuildSubviews:(FAHRENHEIT_VIEW *)view withSuperview:(FAHRENHEIT_VIEW *)superview andBlock:(FahrenheitViewAndSuperviewBlock)block;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (Fahrenheit)
+ (instancetype)_fahrenheit_selfOrInstanceOfSelf;
- (instancetype)_fahrenheit_selfOrInstanceOfSelf;
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
static FAHRENHEIT_VIEW *_fahrenheit_current_view = nil;
static FAHRENHEIT_VIEW *_fahrenheit_current_toplevel_view = nil;

id _fahrenheit_instantiate_from_encoding(char *);

// The _FAHRENHEIT_UNIQUE macro expands to the string "_FAHRENHEIT" with the current line number appended.
// Used to generate identifiers that are "unique" to a given Fahrenheit macro invocation (assuming
// you don't invoke the macro multiple times on the same line).
#define _FAHRENHEIT_CONCATENATE_DETAIL(x, y) x##y
#define _FAHRENHEIT_CONCATENATE(x, y) _FAHRENHEIT_CONCATENATE_DETAIL(x, y)
#define _FAHRENHEIT_UNIQUE _FAHRENHEIT_CONCATENATE(_FAHRENHEIT, __LINE__)

#define _FAHRENHEIT_VIEW_TYPE(viewArg) typeof([viewArg _fahrenheit_selfOrInstanceOfSelf])

// body_after_statement_after_macro will get run *after* the statement formed by the end of this macro and whatever
// the user puts after the macro within {}.
// When body_after_statement_after_macro runs, this macro will have definied a variable with a name created by
// _FAHRENHEIT_UNIQUE, to which will be assigned a block of type FahrenheitViewAndSuperviewBlock containing the code
// that came after the macro.
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
            _FAHRENHEIT_UNIQUE: _FAHRENHEIT_UNIQUE = ^(_FAHRENHEIT_VIEW_TYPE(viewArg) FAHRENHEIT_VIEW_NAME, FAHRENHEIT_VIEW *superview) \
            // We couldn't get execution here without GOTOing here, but once we do and this statement finishes,
            // execution will jump back up to the while(1) and then into body_after_statement_after_macro.

#define FAHRENHEIT_TOPLEVEL(viewArg) \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-value\"") \
({ \
_Pragma("clang diagnostic pop") \
    NSAssert(_fahrenheit_current_toplevel_view == nil, @"Calls to FAHRENHEIT_TOPLEVEL should not be nested."); \
    _fahrenheit_current_toplevel_view = [viewArg _fahrenheit_selfOrInstanceOfSelf]; \
    (_FAHRENHEIT_VIEW_TYPE(viewArg))_fahrenheit_current_toplevel_view; \
}); \
_FAHRENHEIT_GOTO_HELPER(viewArg, \
    [_fahrenheit_current_toplevel_view _fahrenheit_buildSubviews:_FAHRENHEIT_UNIQUE]; \
    _fahrenheit_current_toplevel_view = nil; \
)

#define FAHRENHEIT(variableNameArg) \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-value\"") \
variableNameArg = ({ \
_Pragma("clang diagnostic pop") \
    NSAssert(_fahrenheit_current_toplevel_view, @"Calls to FAHRENHEIT must be inside a call to FAHRENHEIT_TOPLEVEL."); \
    _fahrenheit_current_view = _fahrenheit_instantiate_from_encoding(@encode(typeof(*(variableNameArg)))); \
    (typeof(variableNameArg))_fahrenheit_current_view; \
}); \
_FAHRENHEIT_GOTO_HELPER(variableNameArg, \
    [_fahrenheit_current_toplevel_view _fahrenheit_addViewFromBuildSubviews:_fahrenheit_current_view withSuperview:FAHRENHEIT_VIEW_NAME andBlock:_FAHRENHEIT_UNIQUE]; \
    _fahrenheit_current_view = nil; \
)
