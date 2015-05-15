//
//  Fahrenheit.h
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//


#if TARGET_OS_IPHONE
  #import <UIKit/UIKit.h>
  #define _FAHRENHEIT_VIEW UIView
#elif TARGET_OS_MAC
  #import <AppKit/AppKit.h>
  #define _FAHRENHEIT_VIEW NSView
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


typedef void (^FahrenheitViewAndSuperviewBlock)(id FAHRENHEIT_VIEW_NAME, _FAHRENHEIT_VIEW *superview);

////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _FAHRENHEIT_VIEW (Fahrenheit)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;

- (void)_fahrenheit_buildSubviews:(FahrenheitViewAndSuperviewBlock)block;
- (id)_fahrenheit_addViewFromBuildSubviews:(_FAHRENHEIT_VIEW *)view withSuperview:(_FAHRENHEIT_VIEW *)superview andBlock:(FahrenheitViewAndSuperviewBlock)block;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (Fahrenheit)
+ (instancetype)_fahrenheit_selfOrInstanceOfSelf;
- (instancetype)_fahrenheit_selfOrInstanceOfSelf;
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
static _FAHRENHEIT_VIEW *_fahrenheit_current_view = nil;
static _FAHRENHEIT_VIEW *_fahrenheit_current_toplevel_view = nil;

id _fahrenheit_instantiate_from_encoding(char *);

#define _FAHRENHEIT_CONCATENATE_DETAIL(x, y) x##y
#define _FAHRENHEIT_CONCATENATE(x, y) _FAHRENHEIT_CONCATENATE_DETAIL(x, y)

// Define some macros that will generate 'unique' variable names using __LINE__.
// The names will be unique as long as the FAHRENHEIT macros aren't used twice on the same line.
#define _FAHRENHEIT_PASSED_INSTANCE_OR_NIL _FAHRENHEIT_CONCATENATE(_fahrenheit_passedInstanceOrNil, __LINE__)
#define _FAHRENHEIT_FIRST_STYLE_OR_NONE _FAHRENHEIT_CONCATENATE(_fahrenheit_firstStyleOrNone, __LINE__)
#define _FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK _FAHRENHEIT_CONCATENATE(_fahrenheit_viewAndSuperviewBlockk, __LINE__)
#define _FAHRENHEIT_GOTO_LABEL _FAHRENHEIT_CONCATENATE(_fahrenheit_gotoLabel, __LINE__)

#define _FAHRENHEIT_VIEW_TYPE(viewArg) typeof([viewArg _fahrenheit_selfOrInstanceOfSelf])

// body_after_statement_after_macro will get run *after* the statement formed by the end of this macro and whatever
// the user puts after the macro within {}.
// When body_after_statement_after_macro runs, this macro will have definied a variable with a name created by
// _FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK, to which will be assigned a block of type FahrenheitViewAndSuperviewBlock
// containing the code that came after the macro.
#define _FAHRENHEIT_GOTO_HELPER(viewArg, body_after_statement_after_macro) \
FahrenheitViewAndSuperviewBlock _FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK; \
if (1) { \
    NSAssert([NSThread isMainThread], @"Fahrenheit should only be used from the main thread!"); \
    goto _FAHRENHEIT_GOTO_LABEL; \
} else \
    while (1) \
        if (1) { \
            body_after_statement_after_macro \
            _FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK = nil; \
            break; \
        } else \
            _FAHRENHEIT_GOTO_LABEL: _FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK = ^(_FAHRENHEIT_VIEW_TYPE(viewArg) FAHRENHEIT_VIEW_NAME, _FAHRENHEIT_VIEW *superview) \
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
    [_fahrenheit_current_toplevel_view _fahrenheit_buildSubviews:_FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK]; \
    _fahrenheit_current_toplevel_view = nil; \
)

#define __FAHRENHEIT_OVERLOADED_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,NAME,...) NAME
#define FAHRENHEIT(args...) \
__FAHRENHEIT_OVERLOADED_MACRO( \
args, \
__FAHRENHEIT_HELPER_16, \
__FAHRENHEIT_HELPER_15, \
__FAHRENHEIT_HELPER_14, \
__FAHRENHEIT_HELPER_13, \
__FAHRENHEIT_HELPER_12, \
__FAHRENHEIT_HELPER_11, \
__FAHRENHEIT_HELPER_10, \
__FAHRENHEIT_HELPER_9,  \
__FAHRENHEIT_HELPER_8,  \
__FAHRENHEIT_HELPER_7,  \
__FAHRENHEIT_HELPER_6,  \
__FAHRENHEIT_HELPER_5,  \
__FAHRENHEIT_HELPER_4,  \
__FAHRENHEIT_HELPER_3,  \
__FAHRENHEIT_HELPER_2,  \
__FAHRENHEIT_HELPER_1)(args)

#define __FAHRENHEIT_PRE(x)

typedef struct {
    char *name;
} DRYUIStyle;

#define DRYUI_STYLE(styleName) static const DRYUIStyle styleName = {#styleName};
DRYUIStyle(_DRYUI_EmptyStyle);

id _fahrenheit_takeStyleAndReturnNil(DRYUIStyle notAView);
id _fahrenheit_returnGivenView(UIView *view);

DRYUIStyle _fahrenheit_returnGivenStyle(DRYUIStyle style);
DRYUIStyle _fahrenheit_takeViewAndReturnEmptyStyle(UIView *notAStyle);

void _fahrenheit_applyStyleToView(UIView *view, DRYUIStyle style);

#define __FAHRENHEIT_HELPER_16(x, y, z, ...) ___FAHRENHEIT_HELPER_16(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_15(x, y, z, ...) ___FAHRENHEIT_HELPER_15(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_14(x, y, z, ...) ___FAHRENHEIT_HELPER_14(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_13(x, y, z, ...) ___FAHRENHEIT_HELPER_13(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_12(x, y, z, ...) ___FAHRENHEIT_HELPER_12(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_11(x, y, z, ...) ___FAHRENHEIT_HELPER_11(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_10(x, y, z, ...) ___FAHRENHEIT_HELPER_10(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_9( x, y, z, ...) ___FAHRENHEIT_HELPER_9 (x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_8( x, y, z, ...) ___FAHRENHEIT_HELPER_8 (x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_7( x, y, z, ...) ___FAHRENHEIT_HELPER_7 (x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_6( x, y, z, ...) ___FAHRENHEIT_HELPER_6 (x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_5( x, y, z, ...) ___FAHRENHEIT_HELPER_5 (x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_4( x, y, z, ...) ___FAHRENHEIT_HELPER_4 (x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_3( x, y, z)      ___FAHRENHEIT_HELPER_3 (x, y, ;, z )
#define __FAHRENHEIT_HELPER_2( x, y)         ___FAHRENHEIT_HELPER_2 (x, y, ;)
#define __FAHRENHEIT_HELPER_1( x)            ___FAHRENHEIT_HELPER   (x, ;)

#define ___FAHRENHEIT_HELPER_16(x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_15(x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_15(x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_14(x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_14(x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_13(x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_13(x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_12(x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_12(x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_11(x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_11(x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_10(x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_10(x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_9 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_9( x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_8 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_8( x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_7 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_7( x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_6 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_6( x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_5 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_5( x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_4 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_4( x, y, codeAfterVariableDeclarations, z, ...) ___FAHRENHEIT_HELPER_3 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_3( x, y, codeAfterVariableDeclarations, z)      ___FAHRENHEIT_HELPER_2 (x, y, codeAfterVariableDeclarations; _fahrenheit_applyStyleToView(x, z); )

#define ___FAHRENHEIT_HELPER_2( x, y, codeAfterVariableDeclarations) \
___FAHRENHEIT_HELPER(x, \
    typeof(y) _fahrenheit_y = y;\
    _FAHRENHEIT_PASSED_INSTANCE_OR_NIL = _Generic(_fahrenheit_y, \
        DRYUIStyle: _fahrenheit_takeStyleAndReturnNil, \
        default: _fahrenheit_returnGivenView \
    )(_fahrenheit_y); \
    _FAHRENHEIT_FIRST_STYLE_OR_NONE = _Generic(_fahrenheit_y, \
        DRYUIStyle: _fahrenheit_returnGivenStyle, \
        default: _fahrenheit_takeViewAndReturnEmptyStyle \
    )(_fahrenheit_y); \
    codeAfterVariableDeclarations \
)

#define ___FAHRENHEIT_HELPER(variableName, codeAfterVariableDeclarations) \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-value\"") \
variableName; \
_Pragma("clang diagnostic pop") \
typeof(variableName) _FAHRENHEIT_PASSED_INSTANCE_OR_NIL = nil; \
DRYUIStyle _FAHRENHEIT_FIRST_STYLE_OR_NONE; \
({ \
codeAfterVariableDeclarations \
}); \
if (!variableName) { \
    variableName = ({ \
        NSAssert(_fahrenheit_current_toplevel_view, @"Calls to FAHRENHEIT must be inside a call to FAHRENHEIT_TOPLEVEL."); \
        _fahrenheit_current_view = _FAHRENHEIT_PASSED_INSTANCE_OR_NIL ?: _fahrenheit_instantiate_from_encoding(@encode(typeof(*(variableName)))); \
        (typeof(variableName))_fahrenheit_current_view; \
    }); \
} \
_fahrenheit_applyStyleToView(variableName, _FAHRENHEIT_FIRST_STYLE_OR_NONE); \
_FAHRENHEIT_GOTO_HELPER(variableName, \
    [_fahrenheit_current_toplevel_view _fahrenheit_addViewFromBuildSubviews:_fahrenheit_current_view withSuperview:FAHRENHEIT_VIEW_NAME andBlock:_FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK]; \
    _fahrenheit_current_view = nil; \
)
