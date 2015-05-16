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

typedef struct {
    const char *name;
} _DRYUIStyle;

#define DRYUI_STYLE(styleName) \
static const _DRYUIStyle _DRYUIStyle_##styleName = { .name = #styleName };  \
static const _DRYUIStyle *styleName = &_DRYUIStyle_##styleName;
DRYUI_STYLE(DRYUIEmptyStyle);


typedef void (^FahrenheitViewAndSuperviewBlock)(id FAHRENHEIT_VIEW_NAME, _FAHRENHEIT_VIEW *superview);

////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _FAHRENHEIT_VIEW (Fahrenheit)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;
@property (nonatomic, strong, readonly) NSArray *styleNames;

- (void)_fahrenheit_buildSubviews:(FahrenheitViewAndSuperviewBlock)block;
- (id)_fahrenheit_addViewFromBuildSubviews:(_FAHRENHEIT_VIEW *)view withSuperview:(_FAHRENHEIT_VIEW *)superview andBlock:(FahrenheitViewAndSuperviewBlock)block;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (Fahrenheit)
+ (instancetype)_fahrenheit_selfOrInstanceOfSelf;
- (instancetype)_fahrenheit_selfOrInstanceOfSelf;
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
id _fahrenheit_instantiate_from_encoding(char *);

id _fahrenheit_takeStyleAndReturnNil(const _DRYUIStyle *notAView);
id _fahrenheit_returnGivenView(UIView *view);

const _DRYUIStyle * _fahrenheit_returnGivenStyle(const _DRYUIStyle *style);
const _DRYUIStyle * _fahrenheit_takeViewAndReturnEmptyStyle(UIView *notAStyle);

void _fahrenheit_addStyleToView(UIView *view, const _DRYUIStyle *style);
void _fahrenheit_applyStyles(UIView *view);

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

// FAHRENHEIT expands to one of the two-underscore macros below, depending on how many arguments it's called with.
// Passing more than 32 arguments will not work.
#define __FAHRENHEIT_OVERLOADED_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,NAME,...) NAME
#define FAHRENHEIT(args...) \
__FAHRENHEIT_OVERLOADED_MACRO( \
args, \
__FAHRENHEIT_HELPER_32, \
__FAHRENHEIT_HELPER_31, \
__FAHRENHEIT_HELPER_30, \
__FAHRENHEIT_HELPER_29, \
__FAHRENHEIT_HELPER_28, \
__FAHRENHEIT_HELPER_27, \
__FAHRENHEIT_HELPER_26, \
__FAHRENHEIT_HELPER_25, \
__FAHRENHEIT_HELPER_24, \
__FAHRENHEIT_HELPER_23, \
__FAHRENHEIT_HELPER_22, \
__FAHRENHEIT_HELPER_21, \
__FAHRENHEIT_HELPER_20, \
__FAHRENHEIT_HELPER_19, \
__FAHRENHEIT_HELPER_18, \
__FAHRENHEIT_HELPER_17, \
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

// These macros just pass through to the three-underscore macros while setting the initial
// value for the 'codeAfterVariableAssignment' accumulator.
#define __FAHRENHEIT_HELPER_32(x, y, z, ...) ___FAHRENHEIT_HELPER_32(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_31(x, y, z, ...) ___FAHRENHEIT_HELPER_31(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_30(x, y, z, ...) ___FAHRENHEIT_HELPER_30(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_29(x, y, z, ...) ___FAHRENHEIT_HELPER_29(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_28(x, y, z, ...) ___FAHRENHEIT_HELPER_28(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_27(x, y, z, ...) ___FAHRENHEIT_HELPER_27(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_26(x, y, z, ...) ___FAHRENHEIT_HELPER_26(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_25(x, y, z, ...) ___FAHRENHEIT_HELPER_25(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_24(x, y, z, ...) ___FAHRENHEIT_HELPER_24(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_23(x, y, z, ...) ___FAHRENHEIT_HELPER_23(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_22(x, y, z, ...) ___FAHRENHEIT_HELPER_22(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_21(x, y, z, ...) ___FAHRENHEIT_HELPER_21(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_20(x, y, z, ...) ___FAHRENHEIT_HELPER_20(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_19(x, y, z, ...) ___FAHRENHEIT_HELPER_19(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_18(x, y, z, ...) ___FAHRENHEIT_HELPER_18(x, y, ;, z , ## __VA_ARGS__)
#define __FAHRENHEIT_HELPER_17(x, y, z, ...) ___FAHRENHEIT_HELPER_17(x, y, ;, z , ## __VA_ARGS__)
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
#define __FAHRENHEIT_HELPER_1( x)            ___FAHRENHEIT_HELPER_1 (x, ;, ;)

// These macros add a statement like this:
//    _fahrenheit_addStyleToView(view, style);
// to the 'codeAfterVariableAssignment' parameter for each style in the arguments list.
// The second argument, y, could be wither a pre-made UIView instance getting passed to FAHRENHEIT or a style,
// so it gets handled specially by ___FAHRENHEIT_HELPER_2.
#define ___FAHRENHEIT_HELPER_32(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_31(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_31(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_30(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_30(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_29(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_29(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_28(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_28(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_27(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_27(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_26(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_26(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_25(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_25(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_24(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_24(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_23(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_23(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_22(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_22(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_21(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_21(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_20(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_20(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_19(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_19(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_18(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_18(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_17(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_17(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_16(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_16(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_15(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_15(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_14(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_14(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_13(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_13(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_12(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_12(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_11(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_11(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_10(x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_10(x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_9 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_9( x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_8 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_8( x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_7 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_7( x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_6 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_6( x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_5 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_5( x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_4 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_4( x, y, codeAfterVariableAssignment, z, ...) ___FAHRENHEIT_HELPER_3 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___FAHRENHEIT_HELPER_3( x, y, codeAfterVariableAssignment, z)      ___FAHRENHEIT_HELPER_2 (x, y, codeAfterVariableAssignment; _fahrenheit_addStyleToView(x, z); )

// This macro passes through the first argument and codeAfterVariableAssignment to ___FAHRENHEIT_HELPER_1,
// while determining whether the second argument is a pre-made UIView instance or the first style to apply.
// The variable whose name comes from _FAHRENHEIT_PASSED_INSTANCE_OR_NIL will be set to the given UIView instance
// if the second argument is a UIView instance, or nil if it isn't. The variable whose name comes from
// _FAHRENHEIT_FIRST_STYLE_OR_NONE will be set to the given style if the second argument is a style, or the
// empty style if it isn't.
#define ___FAHRENHEIT_HELPER_2( x, y, codeAfterVariableAssignment) \
___FAHRENHEIT_HELPER_1(x, \
    typeof(y) _fahrenheit_y = y; \
    _FAHRENHEIT_PASSED_INSTANCE_OR_NIL = _Generic(_fahrenheit_y, \
        const _DRYUIStyle*: _fahrenheit_takeStyleAndReturnNil, \
        default: _fahrenheit_returnGivenView \
    )(_fahrenheit_y); \
    _FAHRENHEIT_FIRST_STYLE_OR_NONE = _Generic(_fahrenheit_y, \
        const _DRYUIStyle*: _fahrenheit_returnGivenStyle, \
        default: _fahrenheit_takeViewAndReturnEmptyStyle \
    )(_fahrenheit_y);, \
    codeAfterVariableAssignment \
)

static _FAHRENHEIT_VIEW *_fahrenheit_current_view = nil;
static _FAHRENHEIT_VIEW *_fahrenheit_current_toplevel_view = nil;

#define ___FAHRENHEIT_HELPER_1(variableName, codeAfterVariableDeclarations, codeAfterVariableAssignment) \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-value\"") \
variableName; \
_Pragma("clang diagnostic pop") \
{ \
    typeof(variableName) _FAHRENHEIT_PASSED_INSTANCE_OR_NIL = nil; \
    const _DRYUIStyle *_FAHRENHEIT_FIRST_STYLE_OR_NONE = DRYUIEmptyStyle; \
    ({ codeAfterVariableDeclarations }); \
    if (!variableName) { \
        variableName = ({ \
            NSAssert(_fahrenheit_current_toplevel_view, @"Calls to FAHRENHEIT must be inside a call to FAHRENHEIT_TOPLEVEL."); \
            _fahrenheit_current_view = _FAHRENHEIT_PASSED_INSTANCE_OR_NIL ?: _fahrenheit_instantiate_from_encoding(@encode(typeof(*(variableName)))); \
            (typeof(variableName))_fahrenheit_current_view; \
        }); \
    } \
    _fahrenheit_addStyleToView(variableName, _FAHRENHEIT_FIRST_STYLE_OR_NONE); \
    ({ codeAfterVariableAssignment }); \
    _fahrenheit_applyStyles(variableName); \
} \
_FAHRENHEIT_GOTO_HELPER(variableName, \
    [_fahrenheit_current_toplevel_view _fahrenheit_addViewFromBuildSubviews:_fahrenheit_current_view withSuperview:FAHRENHEIT_VIEW_NAME andBlock:_FAHRENHEIT_VIEW_AND_SUPERVIEW_BLOCK]; \
    _fahrenheit_current_view = nil; \
)
