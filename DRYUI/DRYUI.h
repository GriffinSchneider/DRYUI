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
#define dryui_parent_styles(styles...) _dryui_apply_styles(_, styles)

#define dryui_apply_style(view, style) _dryui_apply_style(view, style)
#define dryui_apply_styles(view, styles...) _dryui_apply_styles(view, styles)


////////////////////////////////////////////////////////////////////////////////////////////////////
// Everything below here is an implementation detail, and subject to change in a minor version.   //
// Don't write code that depends on anything below this line.                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - Implementation Details


FOUNDATION_EXTERN id _dryui_instantiate_from_encoding(char *);


////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -

// Macros that generate unique variable names
#define _dryui_goto_label metamacro_concat(_dryui_goto_label_, __LINE__)
#define _dryui_inner_block_type(styleName) metamacro_concat(_DRYUI_InnerBlockType_, styleName)
#define _dryui_block_type(styleName) metamacro_concat(styleName, BlockType)


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
        static BOOL thing = false; \
        thing = !thing; \
    });) \
    for (MASConstraintMaker* make __attribute__((unused)) = [[MASConstraintMaker alloc] initWithView:_]; ({ \
        NSCAssert([NSThread isMainThread], @"DRYUI should only be used from the main thread!"); \
        static BOOL thing = false; \
        thing = !thing; \
        if (thing) { \
            /* Stuff in here runs before the user-provided code */ \
            spot2 \
        } else { \
            /* Stuff in here runs after the user-provided code */ \
            NSArray *installed = [make install]; \
            if (installed && installed.count) { \
                _.translatesAutoresizingMaskIntoConstraints = NO; \
            } \
        } \
        thing; \
    });) \
    for (_DRYUI_VIEW *superview __attribute__((unused)) = _.superview; ({ \
        static BOOL thing = false; \
        thing = !thing; \
    });) \


#define _dryui_build_subviews(args...) \
    metamacro_if_eq(1, metamacro_argcount(args)) ( \
        _dryui_create_view_helper(args, ;, ;) \
    ) ( \
        _dryui_build_subviewsMore(args) \
    )
#define _dryui_build_subviewsMore(viewArg, styles...) \
    _dryui_create_view_helper(viewArg, ;, dryui_apply_styles(viewArg, styles)) \

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
    if (!variableName) { \
        variableName = _dryui_instantiate_from_encoding(@encode(typeof(*(variableName)))); \
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
    typedef void (^_dryui_block_type(styleName)) (className *_); \
    FOUNDATION_EXTERN _dryui_block_type(styleName) styleName;

#define _dryui_public_styleMore(styleName, className, styleArgs...) \
    typedef void (^_dryui_inner_block_type(styleName)) (className *_); \
    typedef _dryui_inner_block_type(styleName) (^_dryui_block_type(styleName)) (_dryui_extract_arguments(styleArgs)); \
    FOUNDATION_EXTERN _dryui_block_type(styleName) styleName;


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
    _dryui_block_type(styleName) styleName = ^(className *_)

#define _dryui_styleMore(styleName, className, styleArgs...) \
    typedef void (^metamacro_concat(styleName, BlockThatYouWrite)) (className *_, _dryui_extract_arguments(styleArgs)); \
    static metamacro_concat(styleName, BlockThatYouWrite) metamacro_concat(_dryui_blockYouWrite_, styleName); \
    _dryui_block_type(styleName) styleName = ^(_dryui_extract_arguments(styleArgs)) { \
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

