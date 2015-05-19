//
//  DRYUI.m
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//

#import "DRYUI.h"
#import <objc/runtime.h>

dryui_style(DRYUIEmptyStyle) {
};

_DRYUI_VIEW *_dryui_current_view = nil;
_DRYUI_VIEW *_dryui_current_toplevel_view = nil;

////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _DRYUI_VIEW (DRYUI_Private)

@property (nonatomic, strong) MASConstraintMaker *constraintMaker;
@property (nonatomic, strong) NSMutableArray *wrappedAddBlocks;

@end

@implementation _DRYUI_VIEW (DRYUI_Private)

static const char dryui_constraintMakerId = 0;
static const char dryui_wrappedAddBlocksId = 0;
static const char dryui_stylesId = 0;

- (MASConstraintMaker *)constraintMaker {
    return objc_getAssociatedObject(self, &dryui_constraintMakerId);
}
- (void)setConstraintMaker:(MASConstraintMaker *)constraintMaker {
    objc_setAssociatedObject(self, &dryui_constraintMakerId, constraintMaker, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSMutableArray *)wrappedAddBlocks {
    return objc_getAssociatedObject(self, &dryui_wrappedAddBlocksId);
}
- (void)setWrappedAddBlocks:(NSMutableArray *)wrappedAddBlocks {
    objc_setAssociatedObject(self, &dryui_wrappedAddBlocksId, wrappedAddBlocks, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSMutableArray *)styles {
    return objc_getAssociatedObject(self, &dryui_stylesId);
}
- (void)setStyles:(NSMutableArray *)styles {
    objc_setAssociatedObject(self, &dryui_stylesId, styles, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (void)runAllWrappedAddBlocks {
    for (void(^block)() in self.wrappedAddBlocks) {
        block();
    }
    self.wrappedAddBlocks = nil;
}

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation DRYUIStyle
@end


////////////////////////////////////////////////////////////////////////////////////////////////////
id _dryui_instantiate_from_encoding(char *encoding) {
    NSString *encodingString = [NSString stringWithUTF8String:encoding];
    
    NSRange braceRange = [encodingString rangeOfString:@"{"];
    NSRange equalsRange = [encodingString rangeOfString:@"="];
    NSString *className = [encodingString substringWithRange:NSMakeRange(braceRange.location+braceRange.length, equalsRange.location-1)];
    
    id instance = [NSClassFromString(className) new];
    
    return instance;
}

id __attribute((overloadable)) _dryui_returnGivenViewOrNil(DRYUIStyle *notAView) {
    return nil;
}

id __attribute((overloadable)) _dryui_returnGivenViewOrNil(_DRYUI_VIEW *view) {
    return view;
}

DRYUIStyle * __attribute((overloadable)) _dryui_returnGivenStyleOrEmptyStyle(DRYUIStyle *style) {
    return style;
}

DRYUIStyle * __attribute((overloadable)) _dryui_returnGivenStyleOrEmptyStyle(_DRYUI_VIEW *notAStyle) {
    return DRYUIEmptyStyle;
}

void _dryui_applyStyle(_DRYUI_VIEW *view, DRYUIStyle *style, id selfForBlock) {
    if (style == DRYUIEmptyStyle) {
        return;
    }
    
    NSCAssert([view isKindOfClass:NSClassFromString([style viewClassName])],
              @"Attempted to apply style %@ to a view of class %@, which isn't a subclass of %@.",
              style.name,
              NSStringFromClass([view class]),
              style.viewClassName);
    
    view.constraintMaker = [[MASConstraintMaker alloc] initWithView:view];
    view.wrappedAddBlocks = [NSMutableArray array];
    
    style.applicationBlock(view, view.superview, ^(DRYUIStyle *parent_style) {
        _dryui_applyStyle(view, parent_style, selfForBlock);
    }, selfForBlock);
    
    [view.constraintMaker install];
    view.constraintMaker = nil;
    [view runAllWrappedAddBlocks];
}


void _dryui_addStyleToView(_DRYUI_VIEW *view, DRYUIStyle *style, id selfForBlock) {
    if (style == DRYUIEmptyStyle) {
        return;
    }
    
    if (!view.styles) {
        view.styles = [NSMutableArray new];
    }
    [((NSMutableArray *)view.styles) addObject:style];
    
    _dryui_applyStyle(view, style, selfForBlock);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation _DRYUI_VIEW (DRYUI)

@dynamic styles;

- (void)runAddBlock:(DRYUIViewAndSuperviewBlock)block {
    self.constraintMaker = [[MASConstraintMaker alloc] initWithView:self];
    self.wrappedAddBlocks = [NSMutableArray array];
    
    block(self, self.superview);
    
    [self.constraintMaker install];
    self.constraintMaker = nil;
    [self runAllWrappedAddBlocks];
}

- (void)_dryui_buildSubviews:(DRYUIViewAndSuperviewBlock)block {
    [self runAddBlock:block];
}

- (id)_dryui_addViewFromBuildSubviews:(_DRYUI_VIEW *)view withSuperview:(_DRYUI_VIEW *)superview andBlock:(DRYUIViewAndSuperviewBlock)block {
    [superview addSubview:view];
    
    // Don't actually need to weakify this reference since the block will get released
    // after it's run, but we get a block-retain-cycle warning without weakification.
    __weak _DRYUI_VIEW *weakView = view;
    
    // Add a block to view's superview's block list that does some setup, calls the given block, and then runs
    // all the blocks in view's block list (in order to run any blocks put there by calls to this method
    // from the given block).
    // view's superview's block list will eventually get run by the call to this method (or buildSubviews) that
    // recursed into this one.
    if (block) {
        [superview.wrappedAddBlocks addObject:^{
            [weakView runAddBlock:block];
        }];
    }
    return view;
}

- (void)applyStyle:(DRYUIStyle *)style {
    _dryui_applyStyle(self, style, nil);
}

- (void)applyStyle:(DRYUIStyle *)style withSelf:(id)selfArg {
    _dryui_applyStyle(self, style, selfArg);
}

#define _DRYUI_VIEW_STRING _DRYUI_VIEW_STRING_HELPER(_DRYUI_VIEW)
#define _DRYUI_VIEW_STRING_HELPER(x) __DRYUI_VIEW_STRING_HELPER(x)
#define __DRYUI_VIEW_STRING_HELPER(x) @#x
- (MASConstraintMaker *)make {
    NSAssert(self.constraintMaker != nil, @"%@.make should only be used inside a call to buildSubviews.", _DRYUI_VIEW_STRING);
    [self setTranslatesAutoresizingMaskIntoConstraints:NO];
    return self.constraintMaker;
}


@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation NSObject (DRYUI)

+ (instancetype)_dryui_selfOrInstanceOfSelf {
    return [self new];
}
- (instancetype)_dryui_selfOrInstanceOfSelf {
    return self;
}

@end

