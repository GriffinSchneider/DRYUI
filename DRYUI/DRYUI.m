//
//  DRYUI.m
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//

#import "DRYUI.h"
#import <objc/runtime.h>

_DRYUI_VIEW *_dryui_current_view = nil;
_DRYUI_VIEW *_dryui_current_toplevel_view = nil;


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation DRYUIStyle
@end


static const char dryui_constraintMakerId = 0;
static const char dryui_wrappedAddBlocksId = 0;
static const char dryui_stylesId = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation _DRYUI_VIEW (DRYUI)

- (void)_dryui_build_subviews:(DRYUIViewAndSuperviewBlock)block {
    [self runAddBlock:block];
}

- (MASConstraintMaker *)make {
    NSAssert(self._dryuiConstraintMaker != nil, @"%@.make should only be used inside a call to buildSubviews.", @ metamacro_stringify(_DRYUI_VIEW));
    [self setTranslatesAutoresizingMaskIntoConstraints:NO];
    return self._dryuiConstraintMaker;
}

- (void)_dryuiRunAllWrappedAddBlocks {
    for (void(^block)() in self._dryuiWrappedAddBlocks) {
        block();
    }
    self._dryuiWrappedAddBlocks = nil;
}

- (NSMutableArray *)dryuiStyles {
    NSMutableArray *retVal = objc_getAssociatedObject(self, &dryui_stylesId);
    if (!retVal) {
        retVal = [NSMutableArray array];
        objc_setAssociatedObject(self, &dryui_stylesId, retVal, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    return retVal;
}

- (MASConstraintMaker *)_dryuiConstraintMaker {
    return objc_getAssociatedObject(self, &dryui_constraintMakerId);
}
- (void)set_dryuiConstraintMaker:(MASConstraintMaker *)_dryuiConstraintMaker {
    objc_setAssociatedObject(self, &dryui_constraintMakerId, _dryuiConstraintMaker, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSMutableArray *)_dryuiWrappedAddBlocks {
    return objc_getAssociatedObject(self, &dryui_wrappedAddBlocksId);
}
- (void)set_dryuiWrappedAddBlocks:(NSMutableArray *)_dryuiWrappedAddBlocks {
    objc_setAssociatedObject(self, &dryui_wrappedAddBlocksId, _dryuiWrappedAddBlocks, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (void)runAddBlock:(DRYUIViewAndSuperviewBlock)block {
    self._dryuiConstraintMaker = [[MASConstraintMaker alloc] initWithView:self];
    self._dryuiWrappedAddBlocks = [NSMutableArray array];
    
    block(self, self.superview);
    
    [self._dryuiConstraintMaker install];
    self._dryuiConstraintMaker = nil;
    [self _dryuiRunAllWrappedAddBlocks];
}

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

void _dryui_add_view_from_build_subviews(_DRYUI_VIEW *view, _DRYUI_VIEW *superview, DRYUIViewAndSuperviewBlock block) {
    
    [superview addSubview:view];
    
    // Don't actually need to weakify this reference since the block will get released
    // after it's run, but we get a block-retain-cycle warning without weakification.
    __weak _DRYUI_VIEW<DRYUIViewAdditions>  *weakView = _dryui_cast_for_additions(view);
    
    // Add a block to view's superview's block list that does some setup, calls the given block, and then runs
    // all the blocks in view's block list (in order to run any blocks put there by calls to this method
    // from the given block).
    // view's superview's block list will eventually get run by the call to this method (or buildSubviews) that
    // recursed into this one.
    if (block) {
        [_dryui_cast_for_additions(superview)._dryuiWrappedAddBlocks addObject:^{
            [weakView runAddBlock:block];
        }];
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation NSObject (DRYUI)

+ (instancetype)_dryui_selfOrInstanceOfSelf {
    return [self new];
}
- (instancetype)_dryui_selfOrInstanceOfSelf {
    return self;
}

@end

