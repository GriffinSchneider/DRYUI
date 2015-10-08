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
#import <libextobjc/metamacros.h>
#import <objc/runtime.h>
#import "DRYUIMetamacros.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Typedefs
@class DRYUIStyle;

typedef void (^DRYUIViewAndSuperviewBlock)(id _, _DRYUI_VIEW *superview);
typedef void (^DRYUIParentStyleBlock)(DRYUIStyle *parent_style);
typedef void (^DRYUIStyleBlock)(id _, _DRYUI_VIEW *superview, DRYUIParentStyleBlock parent_style, id self);

////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _DRYUI_VIEW (DRYUI)


@property (nonatomic, strong, readonly) MASConstraintMaker *make;

- (void)applyStyle:(DRYUIStyle *)style;
- (void)applyStyle:(DRYUIStyle *)style withSelf:(id)self;

- (void)_dryui_buildSubviews:(DRYUIViewAndSuperviewBlock)block;

@property (nonatomic, strong) MASConstraintMaker *constraintMaker;
@property (nonatomic, strong) NSMutableArray *wrappedAddBlocks;
- (void)runAllWrappedAddBlocks;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface DRYUIStyle : NSObject
@property (nonatomic, readonly) NSString *name;
@property (nonatomic, readonly) NSString *viewClassName;
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (DRYUI)
+ (instancetype)_dryui_selfOrInstanceOfSelf;
- (instancetype)_dryui_selfOrInstanceOfSelf;
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Hierarchy Building Macros

FOUNDATION_EXTERN id _dryui_instantiate_from_encoding(char *);

FOUNDATION_EXTERN void _dryui_addViewFromBuildSubviews(_DRYUI_VIEW *view, _DRYUI_VIEW *superview, DRYUIViewAndSuperviewBlock block);

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Hierarchy Building Macros

#define build_subviews(args...) DRYUI_TOPLEVEL(args)
#define add_subview(args...) DRYUI(args)

#define _DRYUI_CONCATENATE_DETAIL(x, y) x##y
#define _DRYUI_CONCATENATE(x, y) _DRYUI_CONCATENATE_DETAIL(x, y)

// Define some macros that will generate 'unique' variable names using __LINE__.
// The names will be unique as long as the DRYUI macros aren't used twice on the same line.
#define _DRYUI_PASSED_INSTANCE_OR_NIL _DRYUI_CONCATENATE(_dryui_passedInstanceOrNil, __LINE__)
#define _DRYUI_FIRST_STYLE_OR_NONE _DRYUI_CONCATENATE(_dryui_firstStyleOrNone, __LINE__)
#define _DRYUI_VIEW_AND_SUPERVIEW_BLOCK _DRYUI_CONCATENATE(_dryui_viewAndSuperviewBlockk, __LINE__)
#define _DRYUI_GOTO_LABEL _DRYUI_CONCATENATE(_dryui_gotoLabel, __LINE__)
#define _DRYUI_PREVIOUS_TOPLEVEL_VIEW _DRYUI_CONCATENATE(_dryui_previousTopLevelView, __LINE__)
#define _DRYUI_SAVED_SECOND_ARGUMENT _DRYUI_CONCATENATE(_dryui_savedSecondArgument, __LINE__)

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
            _DRYUI_GOTO_LABEL: _DRYUI_VIEW_AND_SUPERVIEW_BLOCK = ^(_DRYUI_VIEW_TYPE(viewArg) _, _DRYUI_VIEW *superview) \
            // We couldn't get execution here without GOTOing here, but once we do and this statement finishes,
            // execution will jump back up to the while(1) and then into body_after_statement_after_macro.

#define DRYUI_TOPLEVEL(viewArg) \
UIView *_DRYUI_PREVIOUS_TOPLEVEL_VIEW = _dryui_current_toplevel_view; \
_dryui_current_toplevel_view = [viewArg _dryui_selfOrInstanceOfSelf]; \
_DRYUI_GOTO_HELPER(viewArg, \
    [_dryui_current_toplevel_view _dryui_buildSubviews:_DRYUI_VIEW_AND_SUPERVIEW_BLOCK]; \
    _dryui_current_toplevel_view = _DRYUI_PREVIOUS_TOPLEVEL_VIEW; \
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
//    _dryui_addStyleToView(view, style(), self);
// to the 'afterAssignment' parameter for each style in the arguments list.
// The second argument, y, could be wither a pre-made UIView instance geggtting passed to DRYUI or a style,
// so it gets handled specially by ___DRYUI_HELPER_2.
#define ___DRYUI_HELPER_32(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_31(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_31(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_30(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_30(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_29(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_29(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_28(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_28(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_27(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_27(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_26(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_26(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_25(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_25(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_24(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_24(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_23(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_23(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_22(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_22(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_21(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_21(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_20(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_20(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_19(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_19(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_18(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_18(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_17(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_17(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_16(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_16(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_15(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_15(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_14(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_14(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_13(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_13(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_12(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_12(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_11(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_11(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_10(x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_10(x, y, afterAssignment, z, ...) ___DRYUI_HELPER_9 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_9( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_8 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_8( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_7 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_7( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_6 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_6( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_5 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_5( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_4 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_4( x, y, afterAssignment, z, ...) ___DRYUI_HELPER_3 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); , ## __VA_ARGS__)
#define ___DRYUI_HELPER_3( x, y, afterAssignment, z)      ___DRYUI_HELPER_2 (x, y, afterAssignment; _dryui_addStyleToView(x, z(), self); )

// This macro passes through the first argument and codeAfterVariableAssignment to ___DRYUI_HELPER_1,
// while determining whether the second argument is a pre-made UIView instance or the first style to apply.
// The variable whose name comes from _DRYUI_PASSED_INSTANCE_OR_NIL will be set to the given UIView instance
// if the second argument is a UIView instance, or nil if it isn't. The variable whose name comes from
// _DRYUI_FIRST_STYLE_OR_NONE will be set to the given style if the second argument is a style, or the
// empty style if it isn't.
#define ___DRYUI_HELPER_2( x, y, codeAfterVariableAssignment) \
___DRYUI_HELPER_1(x, \
  \
    typeof(y) _DRYUI_SAVED_SECOND_ARGUMENT = y; \
    _DRYUI_PASSED_INSTANCE_OR_NIL = _dryui_returnGivenViewOrNil(_DRYUI_SAVED_SECOND_ARGUMENT); \
, \
    _dryui_addStyleToView_acceptView(x, _DRYUI_SAVED_SECOND_ARGUMENT, self); \
    codeAfterVariableAssignment \
)

FOUNDATION_EXTERN _DRYUI_VIEW *_dryui_current_view;
FOUNDATION_EXTERN _DRYUI_VIEW *_dryui_current_toplevel_view;

#define ___DRYUI_HELPER_1(variableName, codeAfterVariableDeclarations, codeAfterVariableAssignment) \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-value\"") \
_Pragma("clang diagnostic ignored \"-Wunused-getter-return-value\"") \
variableName; \
_Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
_Pragma("clang diagnostic pop") \
typeof(variableName) _DRYUI_PASSED_INSTANCE_OR_NIL = nil; \
codeAfterVariableDeclarations \
if (!variableName) { \
    variableName = _DRYUI_PASSED_INSTANCE_OR_NIL ?: _dryui_instantiate_from_encoding(@encode(typeof(*(variableName)))); \
} \
NSAssert(_dryui_current_toplevel_view, @"Calls to DRYUI must be inside a call to DRYUI_TOPLEVEL."); \
_dryui_current_view = variableName; \
_DRYUI_GOTO_HELPER(variableName, \
    _dryui_addViewFromBuildSubviews(_dryui_current_view, _, _DRYUI_VIEW_AND_SUPERVIEW_BLOCK); \
    ({ codeAfterVariableAssignment }); \
    _dryui_current_view = nil; \
)



////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Style Macros

// Helper macros that generate static variable names that store all the actual style data
#define _DRYUI_STYLE_CLASS_NAME(styleName) _DRYUI_Style_ ## styleName
#define _DRYUI_INSTANCE_OF_CLASS_STYLE_NAME(styleName) _DRYUI_InstanceOfClassForStyle_ ## styleName
#define _DRYUI_STYLE_APPLICATION_BLOCK_VARIABLE_NAME(styleName) _DRYUI_Style_ ## styleName ## _applicationBlock


#define _DRYUI_EXTRACT_VARIABLE_NAME(idx, something) metamacro_if_eq(metamacro_is_even(idx), 1)()(something)
#define _DRYUI_EXTRACT_VARIABLE_NAMES(...) \
metamacro_if_eq(1, metamacro_argcount( bogus , ##__VA_ARGS__ ))()(dryui_metamacro_foreach_even_comma(_DRYUI_EXTRACT_VARIABLE_NAME , ##__VA_ARGS__ ))

#define _DRYUI_NOTHING(idx, x) x
#define _DRYUI_EXTRACT_ARGUMENTS(...) \
metamacro_if_eq(1, metamacro_argcount( bogus , ##__VA_ARGS__ ))()(dryui_metamacro_foreach_even_comma(_DRYUI_NOTHING , ##__VA_ARGS__ ))

// Expands to a comma (,) if there are any arguments. Otherwise, expands to nothing.
#define _DRYUI_COMMA_IF_ANY_ARGS(...) metamacro_if_eq(1, metamacro_argcount(bogus , ##__VA_ARGS__))()(,)

// Private styles are just the public style declaration and then the style implementation
#define dryui_private_style(...) \
dryui_public_style(__VA_ARGS__) \
dryui_style(__VA_ARGS__)

// Public style declaration - expands to an @interface declaration for a subclass of DRYUIStyle, and
// an extern-ed variable that holds an instance of the subclass. This singleton instance is what you use
// to actually refer to the class.
// Also declares the overloaded functions used to apply styles to views by add_subview. See 'Style implementation'
// below for an explnation.

#define _dryui_style_block(name, ...)void (^name)(id _, _DRYUI_VIEW *superview, DRYUIParentStyleBlock parent_style, id self _DRYUI_COMMA_IF_ANY_ARGS( __VA_ARGS__ ) _DRYUI_EXTRACT_ARGUMENTS( __VA_ARGS__ ))

#define dryui_public_style(args...) \
metamacro_if_eq(1, metamacro_argcount(args))(dryui_public_style1(args))(metamacro_if_eq(2, metamacro_argcount(args))(dryui_public_style2(args))(dryui_public_styleMore(args)))

#define dryui_public_style1(styleName) dryui_public_style2(styleName, _DRYUI_VIEW)
#define dryui_public_style2(styleName, className) dryui_public_styleMore(styleName, className)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Main entry point for public style
#define dryui_public_styleMore(styleName, className, ...) \
@interface _DRYUI_STYLE_CLASS_NAME(styleName) : DRYUIStyle \
@end  \
\
typedef _dryui_style_block(_DRYUI_applicationBlockForStyle_##styleName , ##__VA_ARGS__ ); \
static _DRYUI_STYLE_CLASS_NAME(styleName)* _DRYUI_INSTANCE_OF_CLASS_STYLE_NAME(styleName); \
static _DRYUI_applicationBlockForStyle_##styleName _DRYUI_STYLE_APPLICATION_BLOCK_VARIABLE_NAME(styleName); \
_DRYUI_TYPEDEFS(styleName, className , ##__VA_ARGS__ )\
\
static _DRYUI_blockForStyle_##styleName styleName; \



#define _DRYUI_TYPEDEFS(styleName, className, ...) metamacro_if_eq(1, metamacro_argcount(bogus , ##__VA_ARGS__ ))(_DRYUI_TYPEDEFS_NO_ARGS(styleName, className))(_DRYUI_TYPEDEFS_SOME_ARGS(styleName, className , ##__VA_ARGS__ ))

#define _DRYUI_TYPEDEFS_NO_ARGS(styleName, className) \
\
typedef void (^_DRYUI_blockThatGetsPassedByAddStyleToView_##styleName )(); \
typedef void                                            (^_DRYUI_blockReturnedByBlockForStyle_##styleName)( _DRYUI_STYLE_CLASS_NAME(styleName)*, _DRYUI_blockThatGetsPassedByAddStyleToView_##styleName blockFromAddStyleToView); \
typedef _DRYUI_blockReturnedByBlockForStyle_##styleName (^_DRYUI_blockForStyle_##styleName)(); \
static inline id __attribute((overloadable, unused)) _dryui_returnGivenViewOrNil(_DRYUI_blockForStyle_##styleName notAView) { \
    return nil; \
} \
static inline id __attribute((overloadable, unused)) _dryui_returnGivenStyleOrEmptyStyle(_DRYUI_blockForStyle_##styleName style) { \
    return style; \
} \
static inline void __attribute__((overloadable, unused)) _dryui_addStyleToView(className *view, _DRYUI_blockReturnedByBlockForStyle_##styleName firstLevelBlock, id selfForBlock) { \
    view.constraintMaker = [[MASConstraintMaker alloc] initWithView:view]; \
    view.wrappedAddBlocks = [NSMutableArray array]; \
    \
    firstLevelBlock(_DRYUI_INSTANCE_OF_CLASS_STYLE_NAME(styleName), ^() { \
        _DRYUI_STYLE_APPLICATION_BLOCK_VARIABLE_NAME(styleName)(view, view.superview, nil, selfForBlock); \
    }); \
    \
    [view.constraintMaker install]; \
    view.constraintMaker = nil; \
    [view runAllWrappedAddBlocks]; \
} \
static inline void __attribute__((overloadable, unused)) _dryui_addStyleToView_acceptView(className *view, _DRYUI_blockForStyle_##styleName firstLevelBlock, id selfForBlock) { \
    _dryui_addStyleToView(view, firstLevelBlock(), selfForBlock);\
} \

#define _DRYUI_TYPEDEFS_SOME_ARGS(styleName, className, ...) \
\
typedef void (^_DRYUI_blockThatGetsPassedByAddStyleToView_##styleName )(_DRYUI_EXTRACT_ARGUMENTS( __VA_ARGS__)); \
typedef void                                                           (^_DRYUI_blockReturnedByBlockReturnedByBlockForStyle_##styleName)( _DRYUI_STYLE_CLASS_NAME(styleName)*, _DRYUI_blockThatGetsPassedByAddStyleToView_##styleName blockFromAddStyleToView); \
typedef _DRYUI_blockReturnedByBlockReturnedByBlockForStyle_##styleName (^_DRYUI_blockReturnedByBlockForStyle_##styleName)(); \
typedef _DRYUI_blockReturnedByBlockForStyle_##styleName                (^_DRYUI_blockForStyle_##styleName)(_DRYUI_EXTRACT_ARGUMENTS( __VA_ARGS__)); \
static inline id __attribute((overloadable, unused)) _dryui_returnGivenViewOrNil(_DRYUI_blockReturnedByBlockForStyle_##styleName notAView) { \
    return nil; \
} \
static inline id  __attribute((overloadable, unused)) _dryui_returnGivenStyleOrEmptyStyle(_DRYUI_blockReturnedByBlockForStyle_##styleName style) { \
    return style(); \
} \
static inline void __attribute__((overloadable, unused)) _dryui_addStyleToView(className *view, _DRYUI_blockReturnedByBlockReturnedByBlockForStyle_##styleName secondLevelBlock, id selfForBlock) { \
    view.constraintMaker = [[MASConstraintMaker alloc] initWithView:view]; \
    view.wrappedAddBlocks = [NSMutableArray array]; \
    \
    secondLevelBlock(_DRYUI_INSTANCE_OF_CLASS_STYLE_NAME(styleName), ^(_DRYUI_EXTRACT_ARGUMENTS( __VA_ARGS__ )) { \
        _DRYUI_STYLE_APPLICATION_BLOCK_VARIABLE_NAME(styleName)(view, view.superview, nil, selfForBlock _DRYUI_COMMA_IF_ANY_ARGS( __VA_ARGS__ ) _DRYUI_EXTRACT_VARIABLE_NAMES( __VA_ARGS__ )); \
    }); \
    \
    [view.constraintMaker install]; \
    view.constraintMaker = nil; \
    [view runAllWrappedAddBlocks]; \
    \
} \
static inline void __attribute__((overloadable, unused)) _dryui_addStyleToView_acceptView(className *view, _DRYUI_blockReturnedByBlockForStyle_##styleName secondLevelBlock, id selfForBlock) { \
    _dryui_addStyleToView(view, secondLevelBlock(), selfForBlock); \
} \


static inline id __attribute((overloadable, unused)) _dryui_returnGivenViewOrNil(_DRYUI_VIEW *view) {
    return view;
}

static inline void __attribute__((overloadable, unused)) _dryui_addStyleToView_acceptView(_DRYUI_VIEW *view, _DRYUI_VIEW *notAStyle, id selfForBlock) {
    
}

static inline id __attribute((overloadable, unused)) _dryui_returnGivenStyleOrEmptyStyle(_DRYUI_VIEW *notAStyle) {
    return nil;
}

// Define the empty style. Attempting to apply the empty style will result in nothing happening as fast as possible.
dryui_public_style(DRYUIEmptyStyle, _DRYUI_VIEW);
dryui_public_style(DRYUIEmptyStyleWithArg, _DRYUI_VIEW, id, bogusArg);

// Style implementation
//
// In order to get the nice syntax where you just put curly braces after the macro, this has to expand to a
// bunch of stuff and then the beginning of a block literal. So, declare a static variable for the block,
// override the applicationBlock getter to return that static variable, and then actually assign the
// block to the var at the very end.
//
// When you refer to the style, (i.e. 'DRYUIEmptyStyle'), you're actually referencing a static variable
// that contains a singleton instance of the class implemented here. We can't assign to that variable statically,
// so instead we assign it when our class gets the 'load' message.
//
// Styles make heavy use of __attribute__((overloadable)) to get type safety for style application. Every style
// declares additional overloaded versions of the _dryui_addStyleToView and _dryui_addStyleToView_acceptView
// functions that take the UIView subclass that the style applies to as the first argument, and an instance of the
// style's class as the second argument. This means that, for example, if you declare a style named 'ButtonStyle'
// that applies only to 'UIButton*', then there is no version of _dryui_addStyleToView that takes a 'UIView*' and
// a 'ButtonStyle', which will cause a compiler warning if you try to apply 'ButtonStyle' to a UIView!
//
// The _dryui_addStyleToView_acceptView function is a hack to support the case where the second argument to add_subview
// is a UIView instance - this function has 1 additional overloaded version where the first and second arguments are
// UIViews, so that you won't get a compiler warning about there being a UIView where we expect a DRYUIStyle.

static const char dryui_thingOnBlockKey = 0;


#define dryui_style(args...) \
metamacro_if_eq(1, metamacro_argcount(args))(dryui_style1(args))(metamacro_if_eq(2, metamacro_argcount(args))(dryui_style2(args))(dryui_styleMore(args)))



#define dryui_style1(styleName) dryui_style2(styleName, _DRYUI_VIEW)
#define dryui_style2(styleName, className) dryui_styleMore(styleName, className)

#define dryui_styleMore(styleName, className, ...) \
\
\
@implementation _DRYUI_STYLE_CLASS_NAME(styleName) \
+ (void)load { \
    objc_setAssociatedObject(styleName, &dryui_thingOnBlockKey, @(444), OBJC_ASSOCIATION_RETAIN); \
    _DRYUI_INSTANCE_OF_CLASS_STYLE_NAME(styleName) = [self new]; \
} \
- (NSString *)name {return @ # styleName;} \
- (NSString *)viewClassName {return @ # className;} \
@end \
\
dryui_splitoutthing(styleName, className , ##__VA_ARGS__ ); \
\
static _DRYUI_applicationBlockForStyle_##styleName _DRYUI_STYLE_APPLICATION_BLOCK_VARIABLE_NAME(styleName) = ^(className *_, _DRYUI_VIEW *superview, DRYUIParentStyleBlock parent_style, id self _DRYUI_COMMA_IF_ANY_ARGS( __VA_ARGS__ ) _DRYUI_EXTRACT_ARGUMENTS( __VA_ARGS__ )) \




#define dryui_splitoutthing(styleName, className, ...) metamacro_if_eq(1, metamacro_argcount(bogus , ##__VA_ARGS__ ))(dryui_splitoutthing_noargs(styleName, className))(dryui_splitoutthing_someargs(styleName, className , ##__VA_ARGS__ ))


#define dryui_splitoutthing_noargs(styleName, className) \
\
static _DRYUI_blockForStyle_##styleName styleName = ^_DRYUI_blockReturnedByBlockForStyle_##styleName() { \
    return ^(_DRYUI_STYLE_CLASS_NAME(styleName) *style, _DRYUI_blockThatGetsPassedByAddStyleToView_##styleName blockFromAddStyleToView) { \
        return blockFromAddStyleToView(); \
    }; \
}; \



#define dryui_splitoutthing_someargs(styleName, className, ...) \
\
static _DRYUI_blockForStyle_##styleName styleName = ^_DRYUI_blockReturnedByBlockForStyle_##styleName( _DRYUI_EXTRACT_ARGUMENTS( __VA_ARGS__ ) ) { \
    return ^_DRYUI_blockReturnedByBlockReturnedByBlockForStyle_##styleName() { \
        return ^(_DRYUI_STYLE_CLASS_NAME(styleName) *style, _DRYUI_blockThatGetsPassedByAddStyleToView_##styleName blockFromAddStyleToView) { \
            return blockFromAddStyleToView(_DRYUI_EXTRACT_VARIABLE_NAMES( __VA_ARGS__ )); \
        }; \
    }; \
}; \



