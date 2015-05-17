//
//  DRYUI.h
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//


#if TARGET_OS_IPHONE
  #import <UIKit/UIKit.h>
  #define _DRYUI_VIEW UIView
#elif TARGET_OS_MAC
  #import <AppKit/AppKit.h>
  #define _DRYUI_VIEW NSView
#endif


#import <Foundation/Foundation.h>
#import <Masonry/Masonry.h>


#ifndef DRYUI_VIEW_NAME
#define DRYUI_VIEW_NAME _
#endif

#ifndef DRYUI_CUSTOM_MACRO_NAMES
#define build_subviews(args...) DRYUI_TOPLEVEL(args)
#define add_subview(args...) DRYUI(args)
#endif

typedef void (^DRYUIViewAndSuperviewBlock)(id DRYUI_VIEW_NAME, _DRYUI_VIEW *superview);

typedef struct {
    const char *name;
    const char *viewClassName;
    __unsafe_unretained DRYUIViewAndSuperviewBlock applicationBlock;
} _DRYUIStyle;
typedef const _DRYUIStyle * DRYUIStyle;

#define _DRYUI_APPLICATION_BLOCK_NAME_FOR_STYLE(styleName) _DRYUI_applyStyle_##styleName

#define DRYUI_DECLARE_STYLE(styleName) \
extern DRYUIStyle styleName; \

#define _DRYUI_IMPLEMENT_STYLE_OVERLOADED_MACRO(_1,_2,NAME,...) NAME
#define DRYUI_IMPLEMENT_STYLE(args...) _DRYUI_IMPLEMENT_STYLE_OVERLOADED_MACRO(args, _DRYUI_IMPLEMENT_STYLE_2, _DRYUI_IMPLEMENT_STYLE_1)(args)

#define _DRYUI_IMPLEMENT_STYLE_1(styleName) \
_DRYUI_IMPLEMENT_STYLE_2(styleName, _DRYUI_VIEW)

// We're going to stringify the className, so add another macro expansion pass
// to let className expand (i.e. if it's _DRYUI_VIEW).
#define _DRYUI_IMPLEMENT_STYLE_2(styleName, className) __DRYUI_IMPLEMENT_STYLE_2(styleName, className)
#define __DRYUI_IMPLEMENT_STYLE_2(styleName, className) \
static DRYUIViewAndSuperviewBlock _DRYUI_APPLICATION_BLOCK_NAME_FOR_STYLE(styleName); \
static const _DRYUIStyle _DRYUIStyle_##styleName = { \
    .name = #styleName, \
    .viewClassName = #className, \
    .applicationBlock = ^void(_DRYUI_VIEW *_, _DRYUI_VIEW *superview) { \
        _DRYUI_APPLICATION_BLOCK_NAME_FOR_STYLE(styleName)(_, superview); \
    } \
}; \
DRYUIStyle styleName = &_DRYUIStyle_##styleName; \
static DRYUIViewAndSuperviewBlock _DRYUI_APPLICATION_BLOCK_NAME_FOR_STYLE(styleName) = ^(className *_, _DRYUI_VIEW *superview)

DRYUI_DECLARE_STYLE(DRYUIEmptyStyle);


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _DRYUI_VIEW (DRYUI)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;
@property (nonatomic, strong, readonly) NSArray *styleNames;

- (void)_dryui_buildSubviews:(DRYUIViewAndSuperviewBlock)block;
- (id)_dryui_addViewFromBuildSubviews:(_DRYUI_VIEW *)view withSuperview:(_DRYUI_VIEW *)superview andBlock:(DRYUIViewAndSuperviewBlock)block;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (DRYUI)
+ (instancetype)_dryui_selfOrInstanceOfSelf;
- (instancetype)_dryui_selfOrInstanceOfSelf;
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
FOUNDATION_EXTERN id _dryui_instantiate_from_encoding(char *);

FOUNDATION_EXTERN id _dryui_takeStyleAndReturnNil(DRYUIStyle notAView);
FOUNDATION_EXTERN id _dryui_returnGivenView(_DRYUI_VIEW *view);

FOUNDATION_EXTERN DRYUIStyle _dryui_returnGivenStyle(DRYUIStyle style);
FOUNDATION_EXTERN DRYUIStyle _dryui_takeViewAndReturnEmptyStyle(_DRYUI_VIEW *notAStyle);

FOUNDATION_EXTERN void _dryui_addStyleToView(_DRYUI_VIEW *view, DRYUIStyle style);

#define _DRYUI_CONCATENATE_DETAIL(x, y) x##y
#define _DRYUI_CONCATENATE(x, y) _DRYUI_CONCATENATE_DETAIL(x, y)

// Define some macros that will generate 'unique' variable names using __LINE__.
// The names will be unique as long as the DRYUI macros aren't used twice on the same line.
#define _DRYUI_PASSED_INSTANCE_OR_NIL _DRYUI_CONCATENATE(_dryui_passedInstanceOrNil, __LINE__)
#define _DRYUI_FIRST_STYLE_OR_NONE _DRYUI_CONCATENATE(_dryui_firstStyleOrNone, __LINE__)
#define _DRYUI_VIEW_AND_SUPERVIEW_BLOCK _DRYUI_CONCATENATE(_dryui_viewAndSuperviewBlockk, __LINE__)
#define _DRYUI_GOTO_LABEL _DRYUI_CONCATENATE(_dryui_gotoLabel, __LINE__)

#define _DRYUI_VIEW_TYPE(viewArg) typeof([viewArg _dryui_selfOrInstanceOfSelf])

// body_after_statement_after_macro will get run *after* the statement formed by the end of this macro and whatever
// the user puts after the macro within {}.
// When body_after_statement_after_macro runs, this macro will have definied a variable with a name created by
// _DRYUI_VIEW_AND_SUPERVIEW_BLOCK, to which will be assigned a block of type DRYUIViewAndSuperviewBlock
// containing the code that came after the macro.
#define _DRYUI_GOTO_HELPER(viewArg, body_after_statement_after_macro) \
DRYUIViewAndSuperviewBlock _DRYUI_VIEW_AND_SUPERVIEW_BLOCK; \
if (1) { \
    NSAssert([NSThread isMainThread], @"DRYUI should only be used from the main thread!"); \
    goto _DRYUI_GOTO_LABEL; \
} else \
    while (1) \
        if (1) { \
            body_after_statement_after_macro \
            _DRYUI_VIEW_AND_SUPERVIEW_BLOCK = nil; \
            break; \
        } else \
            _DRYUI_GOTO_LABEL: _DRYUI_VIEW_AND_SUPERVIEW_BLOCK = ^(_DRYUI_VIEW_TYPE(viewArg) DRYUI_VIEW_NAME, _DRYUI_VIEW *superview) \
            // We couldn't get execution here without GOTOing here, but once we do and this statement finishes,
            // execution will jump back up to the while(1) and then into body_after_statement_after_macro.

#define DRYUI_TOPLEVEL(viewArg) \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-value\"") \
({ \
_Pragma("clang diagnostic pop") \
    NSAssert(_dryui_current_toplevel_view == nil, @"Calls to DRYUI_TOPLEVEL should not be nested."); \
    _dryui_current_toplevel_view = [viewArg _dryui_selfOrInstanceOfSelf]; \
    (_DRYUI_VIEW_TYPE(viewArg))_dryui_current_toplevel_view; \
}); \
_DRYUI_GOTO_HELPER(viewArg, \
    [_dryui_current_toplevel_view _dryui_buildSubviews:_DRYUI_VIEW_AND_SUPERVIEW_BLOCK]; \
    _dryui_current_toplevel_view = nil; \
)

// DRYUI expands to one of the two-underscore macros below, depending on how many arguments it's called with.
// Passing more than 32 arguments will not work.
#define __DRYUI_OVERLOADED_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14, \
_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,NAME,...) NAME
#define DRYUI(args...) \
__DRYUI_OVERLOADED_MACRO(args, \
__DRYUI_HELPER_32, \
__DRYUI_HELPER_31, \
__DRYUI_HELPER_30, \
__DRYUI_HELPER_29, \
__DRYUI_HELPER_28, \
__DRYUI_HELPER_27, \
__DRYUI_HELPER_26, \
__DRYUI_HELPER_25, \
__DRYUI_HELPER_24, \
__DRYUI_HELPER_23, \
__DRYUI_HELPER_22, \
__DRYUI_HELPER_21, \
__DRYUI_HELPER_20, \
__DRYUI_HELPER_19, \
__DRYUI_HELPER_18, \
__DRYUI_HELPER_17, \
__DRYUI_HELPER_16, \
__DRYUI_HELPER_15, \
__DRYUI_HELPER_14, \
__DRYUI_HELPER_13, \
__DRYUI_HELPER_12, \
__DRYUI_HELPER_11, \
__DRYUI_HELPER_10, \
__DRYUI_HELPER_9,  \
__DRYUI_HELPER_8,  \
__DRYUI_HELPER_7,  \
__DRYUI_HELPER_6,  \
__DRYUI_HELPER_5,  \
__DRYUI_HELPER_4,  \
__DRYUI_HELPER_3,  \
__DRYUI_HELPER_2,  \
__DRYUI_HELPER_1)(args)

// These macros just pass through to the three-underscore macros while setting the initial
// value for the 'codeAfterVariableAssignment' accumulator.
#define __DRYUI_HELPER_32(x, y, z, ...) ___DRYUI_HELPER_32(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_31(x, y, z, ...) ___DRYUI_HELPER_31(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_30(x, y, z, ...) ___DRYUI_HELPER_30(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_29(x, y, z, ...) ___DRYUI_HELPER_29(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_28(x, y, z, ...) ___DRYUI_HELPER_28(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_27(x, y, z, ...) ___DRYUI_HELPER_27(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_26(x, y, z, ...) ___DRYUI_HELPER_26(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_25(x, y, z, ...) ___DRYUI_HELPER_25(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_24(x, y, z, ...) ___DRYUI_HELPER_24(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_23(x, y, z, ...) ___DRYUI_HELPER_23(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_22(x, y, z, ...) ___DRYUI_HELPER_22(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_21(x, y, z, ...) ___DRYUI_HELPER_21(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_20(x, y, z, ...) ___DRYUI_HELPER_20(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_19(x, y, z, ...) ___DRYUI_HELPER_19(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_18(x, y, z, ...) ___DRYUI_HELPER_18(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_17(x, y, z, ...) ___DRYUI_HELPER_17(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_16(x, y, z, ...) ___DRYUI_HELPER_16(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_15(x, y, z, ...) ___DRYUI_HELPER_15(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_14(x, y, z, ...) ___DRYUI_HELPER_14(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_13(x, y, z, ...) ___DRYUI_HELPER_13(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_12(x, y, z, ...) ___DRYUI_HELPER_12(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_11(x, y, z, ...) ___DRYUI_HELPER_11(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_10(x, y, z, ...) ___DRYUI_HELPER_10(x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_9( x, y, z, ...) ___DRYUI_HELPER_9 (x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_8( x, y, z, ...) ___DRYUI_HELPER_8 (x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_7( x, y, z, ...) ___DRYUI_HELPER_7 (x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_6( x, y, z, ...) ___DRYUI_HELPER_6 (x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_5( x, y, z, ...) ___DRYUI_HELPER_5 (x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_4( x, y, z, ...) ___DRYUI_HELPER_4 (x, y, ;, z , ## __VA_ARGS__)
#define __DRYUI_HELPER_3( x, y, z)      ___DRYUI_HELPER_3 (x, y, ;, z )
#define __DRYUI_HELPER_2( x, y)         ___DRYUI_HELPER_2 (x, y, ;)
#define __DRYUI_HELPER_1( x)            ___DRYUI_HELPER_1 (x, ;, ;)

// These macros add a statement like this:
//    _dryui_addStyleToView(view, style);
// to the 'afterAssignment' parameter for each style in the arguments list.
// The second argument, y, could be wither a pre-made UIView instance geggtting passed to DRYUI or a style,
// so it gets handled specially by ___DRYUI_HELPER_2.
#define ___DRYUI_HELPER_32(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_31(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_31(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_30(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_30(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_29(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_29(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_28(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_28(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_27(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_27(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_26(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_26(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_25(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_25(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_24(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_24(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_23(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_23(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_22(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_22(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_21(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_21(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_20(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_20(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_19(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_19(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_18(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_18(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_17(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_17(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_16(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_16(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_15(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_15(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_14(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_14(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_13(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_13(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_12(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_12(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_11(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_11(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_10(x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_10(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_9 (x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_9( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_8 (x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_8( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_7 (x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_7( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_6 (x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_6( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_5 (x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_5( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_4 (x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_4( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_3 (x, y, afterAssignment; _dryui_addStyleToView(x, z); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_3( x, y, afterAssignment, z)      ___DRYUI_HELPER_2 (x, y, afterAssignment; _dryui_addStyleToView(x, z); )

// This macro passes through the first argument and codeAfterVariableAssignment to ___DRYUI_HELPER_1,
// while determining whether the second argument is a pre-made UIView instance or the first style to apply.
// The variable whose name comes from _DRYUI_PASSED_INSTANCE_OR_NIL will be set to the given UIView instance
// if the second argument is a UIView instance, or nil if it isn't. The variable whose name comes from
// _DRYUI_FIRST_STYLE_OR_NONE will be set to the given style if the second argument is a style, or the
// empty style if it isn't.
#define ___DRYUI_HELPER_2( x, y, codeAfterVariableAssignment) \
___DRYUI_HELPER_1(x, \
    typeof(y) _dryui_y = y; \
    _DRYUI_PASSED_INSTANCE_OR_NIL = _Generic(_dryui_y, \
        DRYUIStyle: _dryui_takeStyleAndReturnNil, \
        default: _dryui_returnGivenView \
    )(_dryui_y); \
    _DRYUI_FIRST_STYLE_OR_NONE = _Generic(_dryui_y, \
        DRYUIStyle: _dryui_returnGivenStyle, \
        default: _dryui_takeViewAndReturnEmptyStyle \
    )(_dryui_y);, \
    codeAfterVariableAssignment \
)

FOUNDATION_EXTERN _DRYUI_VIEW *_dryui_current_view;
FOUNDATION_EXTERN _DRYUI_VIEW *_dryui_current_toplevel_view;

#define ___DRYUI_HELPER_1(variableName, codeAfterVariableDeclarations, codeAfterVariableAssignment) \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-value\"") \
variableName; \
_Pragma("clang diagnostic pop") \
typeof(variableName) _DRYUI_PASSED_INSTANCE_OR_NIL = nil; \
DRYUIStyle _DRYUI_FIRST_STYLE_OR_NONE = DRYUIEmptyStyle; \
({ codeAfterVariableDeclarations }); \
if (!variableName) { \
    variableName = ({ \
        NSAssert(_dryui_current_toplevel_view, @"Calls to DRYUI must be inside a call to DRYUI_TOPLEVEL."); \
        _dryui_current_view = _DRYUI_PASSED_INSTANCE_OR_NIL ?: _dryui_instantiate_from_encoding(@encode(typeof(*(variableName)))); \
        (typeof(variableName))_dryui_current_view; \
    }); \
} \
_DRYUI_GOTO_HELPER(variableName, \
    [_dryui_current_toplevel_view _dryui_addViewFromBuildSubviews:_dryui_current_view \
                                                              withSuperview:DRYUI_VIEW_NAME \
                                                                   andBlock:_DRYUI_VIEW_AND_SUPERVIEW_BLOCK]; \
    _dryui_addStyleToView(variableName, _DRYUI_FIRST_STYLE_OR_NONE); \
    ({ codeAfterVariableAssignment }); \
    _dryui_current_view = nil; \
)
