//
//  DRYUI.m
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//

#import "DRYUI.h"
#import <objc/runtime.h>

_DRYUI_VIEW<DRYUIViewAdditions> *_dryui_current_view = nil;
_DRYUI_VIEW<DRYUIViewAdditions> *_dryui_current_toplevel_view = nil;


static const char dryui_constraintMakerId = 0;
static const char dryui_stylesId = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation _DRYUI_VIEW (DRYUI)

- (void)_dryui_build_subviews:(DRYUIViewAndSuperviewBlock)block {
    [self _dryui_runAddBlock:block];
}

- (MASConstraintMaker *)make {
    NSAssert(self._dryuiConstraintMaker != nil, @"%@.make should only be used inside a call to buildSubviews.", @ metamacro_stringify(_DRYUI_VIEW));
    [self setTranslatesAutoresizingMaskIntoConstraints:NO];
    return self._dryuiConstraintMaker;
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

- (void)_dryui_runAddBlock:(DRYUIViewAndSuperviewBlock)block {
    self._dryuiConstraintMaker = [[MASConstraintMaker alloc] initWithView:self];
    
    !block ?: block(self, self.superview);
    
    [self._dryuiConstraintMaker install];
    self._dryuiConstraintMaker = nil;
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

////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation NSObject (DRYUI)

+ (instancetype)_dryui_selfOrInstanceOfSelf {
    return [self new];
}
- (instancetype)_dryui_selfOrInstanceOfSelf {
    return self;
}

@end

