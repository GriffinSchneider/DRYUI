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

#define dryui_parent_style(style) _dryui_add_style_to_view_internal(_, style(nil))

#define dryui_apply_style(view, style) _dryui_apply_style(view, style)
#define dryui_apply_styles(view, styles...) _dryui_apply_styles(view, styles)


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _DRYUI_VIEW (DRYUIAdditions)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;
@property (nonatomic, strong, readonly) NSArray *dryuiStyles;

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
#define _dryui_var_saved_style_or_view metamacro_concat(_dryui_saved_style_or_view_, __LINE__)
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
        metamacro_if_eq(2, metamacro_argcount(args)) ( \
            _dryui_add_subview2(args) \
        ) ( \
           _dryui_add_subviewMore(args) \
        ) \
    ) \
    _Pragma("clang diagnostic pop") \


#define _dryui_add_subview1(variableName) \
    _dryui_add_subview_helper_1(variableName, ;, ;)
#define _dryui_add_subview2(variableName, styleOrView) \
    _dryui_add_subview_helper_2(variableName, styleOrView, ;)
#define _dryui_add_subviewMore(variableName, styleOrView, styles...) \
    _dryui_add_subview_helper_2(variableName, styleOrView, \
                                dryui_apply_styles(variableName, styles))


// This macro passes through the first argument and codeAfterVariableAssignment to _dryui_add_subview_helper_1,
// while determining whether the second argument is a pre-made UIView instance or the first style to apply.
// The variable whose name comes from _dryui_var_passed_instance_or_nil will be set to the given UIView instance
// if the second argument is a UIView instance, or nil if it isn't. The variable whose name comes from
// _DRYUI_FIRST_STYLE_OR_NONE will be set to the given style if the second argument is a style, or the
// empty style if it isn't.
#define _dryui_add_subview_helper_2(variableName, styleOrView, codeAfterVariableAssignment) \
    _dryui_add_subview_helper_1(variableName, \
        /* Save styleOrView to a variable, since it might be an expression like '[UIView new]' that we should only evaluate once */ \
        typeof(styleOrView) _dryui_var_saved_style_or_view = styleOrView; \
        _dryui_var_passed_instance_or_nil = _dryui_returnGivenViewOrNil(_dryui_var_saved_style_or_view); \
    , \
        _dryui_add_style_to_view_acceptView(variableName, _dryui_var_saved_style_or_view); \
        codeAfterVariableAssignment \
    )

#define _dryui_add_subview_helper_1(variableName, codeAfterVariableDeclarations, codeAfterVariableAssignment) \
    variableName ; \
    typeof(variableName) _dryui_var_passed_instance_or_nil = nil; \
    codeAfterVariableDeclarations \
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


static inline id __attribute((overloadable, unused))
_dryui_returnGivenViewOrNil(_DRYUI_VIEW *view) {
    return view;
}

static inline void __attribute__((overloadable, unused))
_dryui_add_style_to_view_acceptView(_DRYUI_VIEW *view, _DRYUI_VIEW *notAStyle) {
    
}

static inline id __attribute((overloadable, unused))
_dryui_returnGivenStyleOrNil(_DRYUI_VIEW *notAStyle) {
    return nil;
}


#pragma mark - Style Macro Implementation

////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro helpers for style macros

// Private styles are just the public style declaration and then the style implementation
#define _dryui_private_style(args...) \
    dryui_public_style(args) \
    dryui_style(args) \

// The name of the struct type that's declared every time you declare a style
#define _dryui_style_struct_name(styleName) \
    struct metamacro_concat(_DRYUI_Style_, styleName)

// The name of the variable holding the styles 'application block' - this is the block whose body
// is the code you write after the dryui_style macro.
#define _dryui_style_application_block_variable_name(styleName) \
    metamacro_concat(_DRYUI_StyleAplicationBlock_, styleName)

// 'nil', casted to an instance of the struct for the given style.
// This is used to select the correct overloaded function for our style.
#define _dryui_nil_casted_to_instance_of_style_struct(styleName) \
    ((_dryui_style_struct_name(styleName)*)nil)

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
//
// Styles make heavy use of __attribute__((overloadable)) to get type safety for style application.
// Every style declares additional overloaded versions of the _dryui_add_style_to_view and
//  _dryui_add_style_to_view_acceptView functions that take the UIView subclass that the style
// applies to as the first argument, and an instance of the style's struct as the second argument.
// This means that, for example, if you declare a style named 'ButtonStyle' that applies only to
// 'UIButton*', then there is no version of _dryui_add_style_to_view that takes a 'UIView*' and
// a 'ButtonStyle', which will cause a compiler warning if you try to apply 'ButtonStyle' to a
// UIView!
//
// The _dryui_add_style_to_view_acceptView function is a hack to support the case where the second
// argument to add_subview is a UIView instance - this function has 1 additional overloaded version
// where the first and second arguments are UIViews, so that you won't get a compiler warning about
// there being a UIView where we expect a style.
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
#define _dryui_public_style2(styleName, className) _dryui_public_styleMore(styleName, className)
#define _dryui_public_styleMore(styleName, className, styleArgs...) \
    _dryui_style_struct_name(styleName) {}; \
    typedef void (^_DRYUI_applicationBlockForStyle_##styleName) \
        (id _, _DRYUI_VIEW *superview metamacro_comma_if_any_args(styleArgs) _dryui_extract_arguments(styleArgs)); \
    typedef void (^_DRYUI_blockThatGetsPassedByAddStyleToView_##styleName) \
        (_dryui_extract_arguments(styleArgs)); \
    typedef void (^_DRYUI_blockForAddStyleToView_##styleName) \
        (_dryui_style_struct_name(styleName)*, _DRYUI_blockThatGetsPassedByAddStyleToView_##styleName blockFromAddStyleToView); \
    metamacro_if_no_args(styleArgs) ( \
        _dryui_typedefs_no_args(styleName, className) \
    ) ( \
        _dryui_typedefs_some_args(styleName, className , ##styleArgs ) \
    ) \
    FOUNDATION_EXTERN _DRYUI_applicationBlockForStyle_##styleName _dryui_style_application_block_variable_name(styleName); \
    FOUNDATION_EXTERN _DRYUI_blockForStyle_##styleName styleName; \
    static inline void __attribute__((overloadable, unused))  \
    _dryui_add_style_to_view_internal(className *view, _DRYUI_blockForAddStyleToView_##styleName firstLevelBlock) { \
        firstLevelBlock(_dryui_nil_casted_to_instance_of_style_struct(styleName), ^(_dryui_extract_arguments(styleArgs)) { \
            _dryui_style_application_block_variable_name(styleName) ( \
                view, \
               view.superview \
               metamacro_comma_if_any_args(styleArgs) \
               _dryui_extract_variable_names(styleArgs) \
            ); \
        }); \
    } \
    static inline void __attribute__((overloadable, unused)) \
    _dryui_add_style_to_view(className *view, _DRYUI_blockForAddStyleToView_##styleName firstLevelBlock) { \
        _DRYUI_VIEW<DRYUIViewAdditions> *castedView = _dryui_cast_for_additions(view); \
        id oldConstraintMaker = castedView._dryuiConstraintMaker; \
        castedView._dryuiConstraintMaker = [[MASConstraintMaker alloc] initWithView:view]; \
        \
        _dryui_add_style_to_view_internal(view, firstLevelBlock); \
        \
        [castedView._dryuiConstraintMaker install]; \
        castedView._dryuiConstraintMaker = oldConstraintMaker; \
        [((NSMutableArray *)castedView.dryuiStyles) addObject:styleName]; \
        \
    } \
    metamacro_if_no_args(styleArgs)( \
        _dryui_declare_addStyleToView_acceptView_no_args(styleName, className) \
    ) ( \
        _dryui_declare_addStyleToView_acceptView_some_args(styleName, className , ##styleArgs ) \
    ) \


#define _dryui_typedefs_no_args(styleName, className) \
    typedef _dryui_return_type_for_block_for_style(styleName, className)  \
        (^_DRYUI_blockForStyle_##styleName)(_dryui_style_struct_name(styleName)*); \

#define _dryui_typedefs_some_args(styleName, className, styleArgs...) \
    typedef _DRYUI_blockForAddStyleToView_##styleName  \
        (^_DRYUI_blockReturnedByBlockForStyle_##styleName)(_dryui_style_struct_name(styleName)*); \
    typedef _dryui_return_type_for_block_for_style(styleName, className , ##styleArgs )  \
        (^_DRYUI_blockForStyle_##styleName)(_dryui_extract_arguments(styleArgs)); \
    \

#define _dryui_return_type_for_block_for_style(styleName, className, styleArgs...) \
    metamacro_if_no_args(styleArgs) ( \
        _DRYUI_blockForAddStyleToView_##styleName \
    ) ( \
        _DRYUI_blockReturnedByBlockForStyle_##styleName \
    ) \


#define _dryui_declare_addStyleToView_acceptView_no_args(styleName, className) \
    static inline void __attribute__((overloadable, unused)) \
    _dryui_add_style_to_view_acceptView(className *view, _DRYUI_blockForStyle_##styleName firstLevelBlock) { \
        _dryui_add_style_to_view(view, firstLevelBlock(nil));\
    } \
    static inline id __attribute((overloadable, unused)) \
    _dryui_returnGivenViewOrNil(_DRYUI_blockForStyle_##styleName notAView) { \
        return nil; \
    } \
    static inline id __attribute((overloadable, unused)) \
    _dryui_returnGivenStyleOrNil(_DRYUI_blockForStyle_##styleName style) { \
        return style; \
    } \

#define _dryui_declare_addStyleToView_acceptView_some_args(styleName, className, ...) \
    static inline void __attribute__((overloadable, unused))  \
    _dryui_add_style_to_view_acceptView(className *view, _DRYUI_blockReturnedByBlockForStyle_##styleName secondLevelBlock) { \
        _dryui_add_style_to_view(view, secondLevelBlock(nil)); \
    } \
    static inline id __attribute((overloadable, unused))  \
    _dryui_returnGivenViewOrNil(_DRYUI_blockReturnedByBlockForStyle_##styleName notAView) { \
        return nil; \
    } \
    static inline id  __attribute((overloadable, unused))  \
    _dryui_returnGivenStyleOrNil(_DRYUI_blockReturnedByBlockForStyle_##styleName style) { \
        return style(nil); \
    } \


////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of dryui_style
//
// A style is really just a block. If it's a style without arguments, then the style itself is a
// block that takes one argument, an instance of the style's struct. If the style does have arguments,
// then the style is a block that takes the styles arguments and returns a block that takes an
// instance of the style's struct.
//
// The reason the blocks have an instance of the style struct as an argument is for overloaded method
// selection - the compiler chooses which overloaded method to use based on the actual types of the
// arguments - typedef'ed aliases aren't enough to change which method is used.
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
#define _dryui_style2(styleName, className) _dryui_styleMore(styleName, className)
#define _dryui_styleMore(styleName, className, styleArgs...) \
    _dryui_block_for_style(styleName, className , ##styleArgs ); \
    _DRYUI_applicationBlockForStyle_##styleName _dryui_style_application_block_variable_name(styleName) = \
    ^(className *_,  \
      _DRYUI_VIEW *superview \
      metamacro_comma_if_any_args(styleArgs) \
      _dryui_extract_arguments(styleArgs)) \


#define _dryui_block_for_style(styleName, className, styleArgs...) \
    metamacro_if_no_args(styleArgs) ( \
        _dryui_block_for_style_no_args(styleName, className) \
    ) ( \
        _dryui_block_for_style_some_args(styleName, className , ##styleArgs ) \
    )

#define _dryui_block_for_style_no_args(styleName, className) \
    _DRYUI_blockForStyle_##styleName styleName =  \
     ^_DRYUI_blockForAddStyleToView_##styleName(_dryui_style_struct_name(styleName)* bogus) { \
        return ^(_dryui_style_struct_name(styleName) *style,  \
                 _DRYUI_blockThatGetsPassedByAddStyleToView_##styleName blockFromAddStyleToView) { \
            return blockFromAddStyleToView(); \
        }; \
    }; \

#define _dryui_block_for_style_some_args(styleName, className, styleArgs...) \
    _DRYUI_blockForStyle_##styleName styleName =  \
    ^_DRYUI_blockReturnedByBlockForStyle_##styleName(_dryui_extract_arguments(styleArgs)) { \
        return ^_DRYUI_blockForAddStyleToView_##styleName(_dryui_style_struct_name(styleName)* bogus) { \
            return ^(_dryui_style_struct_name(styleName) *style,  \
                     _DRYUI_blockThatGetsPassedByAddStyleToView_##styleName blockFromAddStyleToView) { \
                return blockFromAddStyleToView(_dryui_extract_variable_names(styleArgs)); \
            }; \
        }; \
    }; \


////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementations of dryui_apply_style and dryui_apply_style
#define _dryui_apply_style(view, style) \
    dryui_apply_styles(view, style)

#define _dryui_apply_styles(view, styles...) \
    metamacro_foreach_cxt_recursive(_dryui_apply_styles_iter, , view, styles)

#define _dryui_apply_styles_iter(idx, view, style) \
    _dryui_add_style_to_view(view, style(nil));
