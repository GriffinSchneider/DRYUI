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
#import "libextobjc-metamacros.h"
#import "DRYUIMetamacros.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Public macro interface

#define build_subviews(args...) _dryui_build_subviews(args)
#define add_subview(args...) _dryui_add_subview(args)

#define dryui_style(args...) _dryui_style(args)

#define dryui_public_style(args...) _dryui_public_style(args)
#define dryui_private_style(args...) _dryui_private_style(args)

#define dryui_parent_style(style) _dryui_apply_style(_, style)

#define dryui_apply_style(view, style) _dryui_apply_style(view, style)
#define dryui_apply_styles(view, styles...) _dryui_apply_styles(view, styles)


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _DRYUI_VIEW (DRYUIAdditions)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
// Everything below here is an implementation detail, and subject to change in a minor version.   //
// Don't write code that depends on anything below this line.                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Implementation Details


////////////////////////////////////////////////////////////////////////////////////////////////////
// The methods in this protocol are actually implemented via a category on UIView, but we don't
// want them to be exposed everywhere. So, our macros cast views to have this protocol when
// we need to use something declared here.
@protocol DRYUIViewAdditions<NSObject>

@property (nonatomic, strong) MASConstraintMaker *_dryuiConstraintMaker;

@end

#define _dryui_cast_for_additions(view) ((_DRYUI_VIEW<DRYUIViewAdditions> *) view)


FOUNDATION_EXTERN id _dryui_instantiate_from_encoding(char *);


////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -

// Define some macros that will generate 'unique' variable names using __LINE__.
// The names will be unique as long as the DRYUI macros aren't used twice on the same line.
#define _dryui_var_passed_instance_or_nil metamacro_concat(_dryui_passed_instance_or_nil_, __LINE__)
#define _dryui_var_view_and_superview_block metamacro_concat(_dryui_view_and_superview_block_, __LINE__)
#define _dryui_var_previous_toplevel_view metamacro_concat(_dryui_previous_toplevel_view_, __LINE__)
#define _dryui_goto_label metamacro_concat(_dryui_goto_label_, __LINE__)


// This is used to implement add_subview and build_subviews - we want to have nice syntax where
// the user puts a body of code in curly braces after the macro, but we need to run some code after
// their code runs. We also want to declare some variables that are only visible inside the user's
// body. So, we use multiple for loops that each run their bodies only once. Each for can declare
// a new variable, and we leave off the braces so all the fors are nested inside one another. One
// of the loop's conditions is a statement expression that we use to run the before/after code
//
// spot1 and spot2 are spots where additional code is inserted to implement add_subview.
#define _dryui_create_view_helper(viewArg, spot1, spot2) \
    spot1 \
    for (typeof(viewArg) _ __attribute__((unused)) = viewArg; ({ \
        NSAssert([NSThread isMainThread], @"DRYUI should only be used from the main thread!"); \
        static BOOL thing = false; \
        thing = !thing; \
        _DRYUI_VIEW<DRYUIViewAdditions> *casted = _dryui_cast_for_additions(_); \
        if (thing) { \
            /* Stuff in here runs before the user-provided code */ \
            casted._dryuiConstraintMaker = [[MASConstraintMaker alloc] initWithView:_]; \
            spot2 \
        } else { \
            /* Stuff in here runs after the user-provided code */ \
            [casted._dryuiConstraintMaker install]; \
            casted._dryuiConstraintMaker = nil; \
        } \
        thing; \
    });) \
    for (_DRYUI_VIEW *superview __attribute__((unused)) = _.superview; ({ \
        static BOOL thing = false; \
        thing = !thing; \
    });) \


#define _dryui_build_subviews(viewArg) \
    _dryui_create_view_helper(viewArg, ;, ;)


#define _dryui_add_subview(args...) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
    _Pragma("clang diagnostic ignored \"-Wunused-getter-return-value\"") \
    metamacro_if_eq(1, metamacro_argcount(args)) ( \
        _dryui_add_subview1(args) \
    ) ( \
        _dryui_add_subviewMore(args) \
    ) \
    _Pragma("clang diagnostic pop") \


#define _dryui_add_subview1(variableName) \
    _dryui_add_subview_helper_1(variableName, ;)
#define _dryui_add_subviewMore(variableName, styles...) \
    _dryui_add_subview_helper_1(variableName, dryui_apply_styles(variableName, styles))

#define _dryui_add_subview_helper_1(variableName, codeAfterVariableAssignment) \
    variableName ; \
    typeof(variableName) _dryui_var_passed_instance_or_nil = nil; \
    if (!variableName) { \
        variableName = _dryui_var_passed_instance_or_nil ?: _dryui_instantiate_from_encoding(@encode(typeof(*(variableName)))); \
    } \
    _dryui_create_view_helper(variableName, \
        for (_DRYUI_VIEW *_dryui_previous_view = _; ({ \
            static BOOL thing = false; \
            thing = !thing; \
            thing; \
        });) \
    , \
        [_dryui_previous_view addSubview:_]; \
        ({ codeAfterVariableAssignment }); \
    ) \


#pragma mark - Style Macro Implementation

////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro helpers for style macros

// Private styles are just the public style declaration and then the style implementation
#define _dryui_private_style(args...) \
    dryui_public_style(args) \
    dryui_style(args) \

// Passed an argument list like this:
//    (NSString *)firstArg, (NSString *)secondArg
// _dryui_extract_variable_names will expand to a list containing just the variable names:
//    firstArg, secondArg
#define _dryui_remove_arg_type(arg)
#define _dryui_extract_variable_name(idx, argWithType) \
    _dryui_remove_arg_type argWithType
#define _dryui_extract_variable_names(argsWithTypes...) \
    metamacro_if_no_args(argsWithTypes) ( \
        /* nothing */ \
    ) ( \
        metamacro_foreach_sep_macro(_dryui_extract_variable_name, metamacro_comma_sep , ##argsWithTypes ) \
    ) \

// Passed an argument list like this:
//    (NSString *)firstArg, (NSString *)secondArg
// _dryui_extract_arguments will expand to a normally-formatted list of function arguments with
// types, like this:
//    NSString *firstArg, NSString *secondArg
#define _dryui_remove_parens_from_arg_type(argWithType) \
    argWithType
#define _dryui_remove_parens_from_arg_type_iter(idx, argWithType) \
    _dryui_remove_parens_from_arg_type argWithType
#define _dryui_extract_arguments(argsWithTypes...) \
    metamacro_if_no_args(argsWithTypes) ( \
        /* nothing */ \
    ) ( \
        metamacro_foreach_sep_macro(_dryui_remove_parens_from_arg_type_iter, metamacro_comma_sep , ##argsWithTypes ) \
    )


////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of dryui_public_style
#define _dryui_public_style(args...) \
    metamacro_if_eq(1, metamacro_argcount(args)) ( \
        _dryui_public_style1(args) \
    ) ( \
        metamacro_if_eq(2, metamacro_argcount(args)) ( \
            _dryui_public_style2(args) \
        ) ( \
            _dryui_public_styleMore(args) \
        ) \
    ) \

#define _dryui_public_style1(styleName) _dryui_public_style2(styleName, _DRYUI_VIEW)
#define _dryui_public_style2(styleName, className) \
    typedef void (^metamacro_concat(styleName, BlockType)) (className *_); \
    FOUNDATION_EXTERN metamacro_concat(styleName, BlockType) styleName;

#define _dryui_public_styleMore(styleName, className, styleArgs...) \
    typedef void (^metamacro_concat(styleName, InnerBlockType)) (className *_); \
    typedef metamacro_concat(styleName, InnerBlockType) (^metamacro_concat(styleName, BlockType)) (_dryui_extract_arguments(styleArgs)); \
    FOUNDATION_EXTERN metamacro_concat(styleName, BlockType) styleName;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of dryui_style
//
// A style is really just a block. If it's a style without arguments, then the style itself is a
// block that takes a view _ as an argument. If the style does have arguments, then the style is
// a block that takes the styles arguments and returns a block that takes aview.
#define _dryui_style(args...) \
    metamacro_if_eq(1, metamacro_argcount(args)) ( \
        _dryui_style1(args) \
    ) ( \
        metamacro_if_eq(2, metamacro_argcount(args)) ( \
            _dryui_style2(args) \
        ) ( \
            _dryui_styleMore(args) \
        ) \
    )

#define _dryui_style1(styleName) _dryui_style2(styleName, _DRYUI_VIEW)
#define _dryui_style2(styleName, className) \
    metamacro_concat(styleName, BlockType) styleName = ^(className *_)

#define _dryui_styleMore(styleName, className, styleArgs...) \
    typedef void (^metamacro_concat(styleName, BlockThatYouWrite)) (className *_, _dryui_extract_arguments(styleArgs)); \
    static metamacro_concat(styleName, BlockThatYouWrite) metamacro_concat(_dryui_blockYouWrite_, styleName); \
    metamacro_concat(styleName, BlockType) styleName = ^(_dryui_extract_arguments(styleArgs)) { \
        return ^(className *_) { \
            metamacro_concat(_dryui_blockYouWrite_, styleName)(_, _dryui_extract_variable_names(styleArgs)); \
        }; \
    }; \
    static metamacro_concat(styleName, BlockThatYouWrite) metamacro_concat(_dryui_blockYouWrite_, styleName) = ^(className *_, _dryui_extract_arguments(styleArgs))


////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementations of dryui_apply_style and dryui_apply_style
#define _dryui_apply_style(view, style) \
    style(view)

#define _dryui_apply_styles(view, styles...) \
    metamacro_foreach_cxt_recursive(_dryui_apply_styles_iter, , view, styles)

#define _dryui_apply_styles_iter(idx, view, style) \
    _dryui_apply_style(view, style);

